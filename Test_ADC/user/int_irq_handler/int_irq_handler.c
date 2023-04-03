//----------------------------------------------------------------------------------
// File Name: led.c
// Create Date: 2023-03-07 11:55:26
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#include "int_irq_handler.h"

//----------------------------------------------------------------------------------
// Function name: TMR6_GLOBAL_IRQHandler
// Input:
// Output:
// Comment: Timer 6 interrupt handler
//----------------------------------------------------------------------------------
void TMR6_GLOBAL_IRQHandler(void)
{
    if (tmr_flag_get(TMR6, TMR_OVF_FLAG) != RESET)
    {
        g_tSystemFlags.bits.timer2msFlag = 1;
        tmr_flag_clear(TMR6, TMR_OVF_FLAG);
    }
}

//----------------------------------------------------------------------------------
// Function name: DMA1_Channel1_IRQHandler
// Input:
// Output:
// Comment: ADC interrupt handler
//----------------------------------------------------------------------------------
void DMA1_Channel1_IRQHandler(void)
{
    if(dma_flag_get(DMA1_FDT1_FLAG) != RESET)
    {
        dma_flag_clear(DMA1_FDT1_FLAG);
        AdcDmaISR();
    }
}

//----------------------------------------------------------------------------------
// Function name: DMA1_Channel5_4_IRQHandler
// Input:
// Output:
// Comment: USART send interrupt handler
//----------------------------------------------------------------------------------
void DMA1_Channel5_4_IRQHandler(void)
{
	if(dma_flag_get(DMA1_FDT4_FLAG))
	{
		dma_flag_clear(DMA1_FDT4_FLAG);
		Usart2TxDmaISR();
	}
}

//----------------------------------------------------------------------------------
// Function name: USART2_IRQHandler
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
void USART2_IRQHandler(void)
{
	if(usart_flag_get(USART2, USART_IDLEF_FLAG) != RESET)
	{
		usart_flag_clear(USART2, USART_IDLEF_FLAG);
		Usart2RxIntIdleISR();
	}
}

//----------------------------------------------------------------------------------
// Function name: TMR17_GLOBAL_IRQHandler
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
void TMR17_GLOBAL_IRQHandler(void)
{
    if(tmr_flag_get(TMR17, TMR_C1_FLAG) != RESET)
    {
        tmr_flag_clear(TMR17, TMR_C1_FLAG);
        IrReceiveISR();
    }
}