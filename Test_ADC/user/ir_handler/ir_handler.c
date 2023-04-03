//----------------------------------------------------------------------------------
// File Name: ir_handler.c
// Create Date: 2023-04-01 09:41:21
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#include "ir_handler.h"

//----------------------------------------------------------------------------------
// Function name: IrHandleGlobal
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
void IrHandleGlobal(void)
{
    IrReceiverPacket_T irRxPacket;
    static uint8_t counter = 0;

    IrReceiveStatusClear();
    irRxPacket = GetIrRxNewData();
    if (irRxPacket.bits.irRxNewDataValidFlag == 0)
    {
        /* invalid */
        return;
    }
    if (irRxPacket.address != 0x00)
    {
        /* invalid */
        return;
    }
    /* valid */
    switch (irRxPacket.command)
    {
    case 0x5C:
        if(counter == 1)
		{
			counter = 0;
			LedRedOn();
		}
		else
		{
			counter = 1;
			LedRedOff();
		}
        break;
    default:
        break;
    }
}
