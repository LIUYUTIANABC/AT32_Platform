//----------------------------------------------------------------------------------
// File Name: adc.base.c
// Create Date: 2023-03-18 16:56:18
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#include "adc_base.h"

//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
static uint16_t s_u16AdcDataBuffer[ADC_CHANNEL_LENGTH] = {0};
static AdcFlags_T s_tAdcFlag;
static uint16_t s_u16AdcDataVref = 0;
static uint16_t s_u16AdcDataLux = 0;

//----------------------------------------------------------------------------------
// static functions
//----------------------------------------------------------------------------------
static void AdcLuxDmaInit(void);
static void AdcLuxChannelInit(void);

//----------------------------------------------------------------------------------
// Define public interface
//----------------------------------------------------------------------------------
/* Get Verf voltage mV */
uint32_t GetAdcVerfVoltage_mV(void)
{
    return (uint32_t)(ADC_INTERNAL_VREF_MV * ADC_RESOLUTION / s_u16AdcDataVref);
}
/* Get Lux voltage mV */
uint32_t GetAdcLuxVoltage_mV(void)
{
    uint32_t verf = GetAdcVerfVoltage_mV();
    return (uint32_t)(s_u16AdcDataLux * verf / ADC_RESOLUTION);
}
/*
*  repeat calculate ADC average
*  Must be called by main
*/
void AdcAverageProcess(void)
{
    if(s_tAdcFlag.bits.adcDmaCompleteFlag)
    {
        s_tAdcFlag.bits.adcDmaCompleteFlag = 0;
        adc_ordinary_software_trigger_enable(ADC1, TRUE);
        // Vref average
        s_u16AdcDataVref <<= 3;
        s_u16AdcDataVref -= (s_u16AdcDataVref >> 3);
        s_u16AdcDataVref += s_u16AdcDataBuffer[0];
        s_u16AdcDataVref >>= 3;
        // Lux Adc average
        s_u16AdcDataLux <<= 3;
        s_u16AdcDataLux -= (s_u16AdcDataLux >> 3);
        s_u16AdcDataLux += s_u16AdcDataBuffer[1];
        s_u16AdcDataLux >>= 3;
    }
}
/*
*  the data reception is complete;
*  called by ADC DMA interrupt
*/
void AdcDmaISR(void)
{
    s_tAdcFlag.bits.adcDmaCompleteFlag = 1;
}

//----------------------------------------------------------------------------------
// Function name: AdcLuxDmaInterruptInit
// Input:
// Output:
// Comment: use DMA to get ADC data
//----------------------------------------------------------------------------------
void AdcLuxDmaInterruptInit(void)
{
    AdcLuxDmaInit();
    AdcLuxChannelInit();
}

//----------------------------------------------------------------------------------
// Function name: AdcLuxDmaInit
// Input:
// Output:
// Comment: DMA trigger interrupt if data complete
//----------------------------------------------------------------------------------
static void AdcLuxDmaInit(void)
{
    dma_init_type dma_init_struct;

    dma_reset(DMA1_CHANNEL1);
    dma_default_para_init(&dma_init_struct);
    dma_init_struct.buffer_size = ADC_CHANNEL_LENGTH;
    dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_base_addr = (uint32_t)&s_u16AdcDataBuffer;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
    dma_init_struct.memory_inc_enable = TRUE;
    dma_init_struct.peripheral_base_addr = (uint32_t)&(ADC1->odt);
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_init_struct.loop_mode_enable = TRUE;
    dma_init(DMA1_CHANNEL1, &dma_init_struct);

    dma_interrupt_enable(DMA1_CHANNEL1, DMA_FDT_INT, TRUE);
    nvic_irq_enable(DMA1_Channel1_IRQn, NVIC_PRIO_PREEMPT_ADC_LUX_DMA_COMPLETE, NVIC_NO_PRIO_SUB);
    dma_channel_enable(DMA1_CHANNEL1, TRUE);
}

//----------------------------------------------------------------------------------
// Function name: AdcLuxChannelInit
// Input:
// Output:
// Comment: Channel1 is Vref; Channel2 is Lux adc
//----------------------------------------------------------------------------------
static void AdcLuxChannelInit(void)
{
    adc_base_config_type adc_base_struct;

    adc_base_default_para_init(&adc_base_struct);
    adc_base_struct.sequence_mode = TRUE;
    adc_base_struct.repeat_mode = FALSE;
    adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
    adc_base_struct.ordinary_channel_length = ADC_CHANNEL_LENGTH;
    adc_base_config(ADC1, &adc_base_struct);
    adc_ordinary_channel_set(ADC1, ADC_CHANNEL_VREF, 1, ADC_SAMPLETIME_239_5);
    adc_ordinary_channel_set(ADC1, ADC_CHANNEL_LUX, 2, ADC_SAMPLETIME_239_5);
    adc_ordinary_conversion_trigger_set(ADC1, ADC12_ORDINARY_TRIG_SOFTWARE, TRUE);
    adc_dma_mode_enable(ADC1, TRUE);
    adc_tempersensor_vintrv_enable(TRUE);

    adc_enable(ADC1, TRUE);

    adc_calibration_init(ADC1);
    while(adc_calibration_init_status_get(ADC1));
    adc_calibration_start(ADC1);
    while(adc_calibration_status_get(ADC1));

    // start ADC convert
    adc_ordinary_software_trigger_enable(ADC1, TRUE);
}
