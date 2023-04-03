//----------------------------------------------------------------------------------
// File Name: ir_handler.c
// Create Date: 2023-04-01 09:41:21
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#include "adc_handler.h"

//----------------------------------------------------------------------------------
// Function name: IrHandleGlobal
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
void AdcHandleGlobal(void)
{
    static uint16_t counter = 0;
    uint32_t voltage = 0;
    uint32_t voltage1 = 0;
    Usart2Packet_T packet;

    voltage = GetAdcVerfVoltage_mV();
    voltage1 = GetAdcLuxVoltage_mV();

    counter++;
    if(counter > 1000)
    {
    	counter = 0;
        packet.eCommand = USART2_CMD_QUERY_ADC_LUX;
        packet.u8Data3 = (u8)(voltage >> 8);
        packet.u8Data2 = (u8)(voltage);
        packet.u8Data1 = (u8)(voltage1>>8);
        packet.u8Data0 = (u8)(voltage1);
        Usart2Send(packet);
    }
}
