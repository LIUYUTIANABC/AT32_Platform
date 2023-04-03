//----------------------------------------------------------------------------------
// File Name: ir_base.c
// Create Date: 2023-03-23 17:21:47
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#include "ir_base.h"

//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
static volatile IrReceiveStatus_E s_eIrRxStatus = IR_RECEIVE_IDLE;
static volatile IrReceiveFlags_T s_tIrRxFlag;
static volatile uint32_t s_u32IrReceiveData = 0;

//----------------------------------------------------------------------------------
// Static function (this file)
//----------------------------------------------------------------------------------
static uint16_t GetIrWaveTime(uint16_t _time_0, uint16_t _time_1);
static u32 ReverseBits(u32 b);
static u32 ReverseBitsShift(u32 n);

//----------------------------------------------------------------------------------
// Function name: IrReceiveTimer17Init
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
void IrReceiveTimer17Init(void)
{
    tmr_input_config_type tmr_input_config_struct;

    /* tmr17 counter mode configuration */
    tmr_base_init(TMR17, 0xFFFF, IR_RX_TIMER17_DIV_US_10);

    /* configure tmr17 channel1 to get clock signal */
    tmr_input_config_struct.input_channel_select = TMR_SELECT_CHANNEL_1;
    tmr_input_config_struct.input_mapped_select = TMR_CC_CHANNEL_MAPPED_DIRECT;
    tmr_input_config_struct.input_polarity_select = TMR_INPUT_FALLING_EDGE;
    tmr_input_channel_init(TMR17, &tmr_input_config_struct, TMR_CHANNEL_INPUT_DIV_1);

    tmr_interrupt_enable(TMR17, TMR_C1_INT, TRUE);

    /* tmr17 trigger interrupt nvic init */
    nvic_irq_enable(TMR17_GLOBAL_IRQn, NVIC_PRIO_PREEMPT_IR_RECEIVE_TIMER17, NVIC_NO_PRIO_SUB);

    /* enable tmr17 */
    tmr_counter_enable(TMR17, TRUE);
}

//----------------------------------------------------------------------------------
// Function name: GetIrWaveTime
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
static uint16_t GetIrWaveTime(uint16_t _time_0, uint16_t _time_1)
{
    if (_time_1 >= _time_0)
    {
        return ((_time_1 - _time_0) * IR_RX_TIMER17_SAMPLE_TIME_US);
    }
    else
    {
        return (((0xFFFF - _time_0) + _time_1) * IR_RX_TIMER17_SAMPLE_TIME_US);
    }
}

//----------------------------------------------------------------------------------
// Function name: IrReceiveISR
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
void IrReceiveISR(void)
{
    static volatile uint8_t index = 0;
    static volatile uint16_t irTimer17Count_0 = 0;
    static volatile uint16_t irTimer17Count_1 = 0;
    static volatile uint16_t irWaveTime = 0;

    switch (s_eIrRxStatus)
    {
    case IR_RECEIVE_IDLE:
        irTimer17Count_0 = tmr_channel_value_get(TMR17, TMR_SELECT_CHANNEL_1);
        s_eIrRxStatus = IR_RECEIVE_START;
        break;
    case IR_RECEIVE_START:
        irTimer17Count_1 = tmr_channel_value_get(TMR17, TMR_SELECT_CHANNEL_1);
        irWaveTime = GetIrWaveTime(irTimer17Count_0, irTimer17Count_1);
        /* start to receive data */
        if ((irWaveTime > IR_RX_START_TIME_MIN_US) && (irWaveTime < IR_RX_START_TIME_MAX_US))
        {
            s_eIrRxStatus = IR_RECEIVE_DATA;
            s_u32IrReceiveData = 0;
            index = 0;
            irTimer17Count_0 = irTimer17Count_1;
        }
        /* repeat code */
        if ((irWaveTime > IR_RX_REPEAT_TIME_MIN_US) && (irWaveTime < IR_RX_REPEAT_TIME_MAX_US))
        {
            // s_eIrRxStatus = IR_RECEIVE_REPEAT;
            s_tIrRxFlag.bits.irRxRepeatFlag = 1;
            s_eIrRxStatus = IR_RECEIVE_IDLE;
        }
        break;
    case IR_RECEIVE_DATA:
        irTimer17Count_1 = tmr_channel_value_get(TMR17, TMR_SELECT_CHANNEL_1);
        irWaveTime = GetIrWaveTime(irTimer17Count_0, irTimer17Count_1);
        irTimer17Count_0 = irTimer17Count_1;
        if ((irWaveTime > IR_RX_DATA_0_TIME_MIN_US) && (irWaveTime < IR_RX_DATA_0_TIME_MAX_US))
        {
            /* 0 */
            s_u32IrReceiveData = s_u32IrReceiveData << 1;
        }
        else if ((irWaveTime > IR_RX_DATA_1_TIME_MIN_US) && (irWaveTime < IR_RX_DATA_1_TIME_MAX_US))
        {
            /* 1 */
            s_u32IrReceiveData = (s_u32IrReceiveData << 1) + 1;
        }
        else
        {
            s_eIrRxStatus = IR_RECEIVE_IDLE;
        }
        /* 32 bits */
        if (index++ >= 31)
        {
            s_eIrRxStatus = IR_RECEIVE_IDLE;
            s_tIrRxFlag.bits.irRxNewDataFlag = 1;
        }
        break;
    case IR_RECEIVE_REPEAT:
        break;
    default:
        s_eIrRxStatus = IR_RECEIVE_IDLE;
        break;
    }
}

//----------------------------------------------------------------------------------
// Function name: IrReceiveStatusClear
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
void IrReceiveStatusClear(void)
{
    static uint8_t counter = 0;
    if (s_eIrRxStatus != IR_RECEIVE_IDLE)
    {
        if(counter++ > 100)
        {
            s_eIrRxStatus = IR_RECEIVE_IDLE;
        }
        return;
    }
    counter = 0;
}

//----------------------------------------------------------------------------------
// Function name: GetIrRxNewData
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
IrReceiverPacket_T GetIrRxNewData(void)
{
    uint8_t irRxAddress = 0;
    uint8_t irRxAddressInvert = 0;
    uint8_t irRxCommand = 0;
    uint8_t irRxCommandInvert = 0;
    IrReceiverPacket_T irRxPacket;
    uint32_t irReceiveData = 0;

    /* new data or repeat */
    if (s_tIrRxFlag.bits.irRxNewDataFlag || s_tIrRxFlag.bits.irRxRepeatFlag)
    {
        s_tIrRxFlag.bits.irRxNewDataFlag = 0;
        s_tIrRxFlag.bits.irRxRepeatFlag = 0;
        /* cause IR bits are invert; we need reverse them */
        irReceiveData = ReverseBitsShift(s_u32IrReceiveData);
        /* get each byte from IR packet */
        irRxAddress = (uint8_t)(irReceiveData);
        irRxAddressInvert = (uint8_t)(irReceiveData >> 8);
        irRxCommand = (uint8_t)(irReceiveData >> 16);
        irRxCommandInvert = (uint8_t)(irReceiveData >> 24);
        /* calibrate IR packet */
        if ((irRxAddress == (0xFF - irRxAddressInvert))
            && (irRxCommand == (0xFF - irRxCommandInvert)))
        {
            irRxPacket.bits.irRxNewDataValidFlag = 1;
            irRxPacket.address = irRxAddress;
            irRxPacket.command = irRxCommand;
            return irRxPacket;
        }
    }
    /* have not valid packet */
    irRxPacket.bits.irRxNewDataValidFlag = 0;
    irRxPacket.address = 0;
    irRxPacket.command = 0;
    return irRxPacket;
}

//----------------------------------------------------------------------------------
// Function name: ReverseBits
// Input:
// Output:
// Comment: Get inverse code
//----------------------------------------------------------------------------------
static u32 ReverseBits(u32 b)
{
    u32 result = 0;
    for(int i = 0; i < 32; i++) {
        result = (result << 1) | (b & 1);
        b >>= 1;
    }
    return result;
}

//----------------------------------------------------------------------------------
// Function name: ReverseBitsShift
// Input:
// Output:
// Comment: Get inverse code
//----------------------------------------------------------------------------------
static u32 ReverseBitsShift(u32 n)
{
    n = ((n & 0x55555555) << 1) | ((n & 0xAAAAAAAA) >> 1);
    n = ((n & 0x33333333) << 2) | ((n & 0xCCCCCCCC) >> 2);
    n = ((n & 0x0F0F0F0F) << 4) | ((n & 0xF0F0F0F0) >> 4);
    n = ((n & 0x00FF00FF) << 8) | ((n & 0xFF00FF00) >> 8);
    n = ((n & 0x0000FFFF) << 16) | ((n & 0xFFFF0000) >> 16);
    return n;
}

/****************************** END OF FILE ***************************************/
