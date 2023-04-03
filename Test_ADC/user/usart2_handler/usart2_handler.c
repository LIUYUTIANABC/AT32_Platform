//----------------------------------------------------------------------------------
// File Name: led.c
// Create Date: 2023-03-07 11:55:26
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#include "usart2_handler.h"

//----------------------------------------------------------------------------------
// Function name: Usart2ReceiveCmd
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
static void Usart2ReceiveCmd(void)
{
    Usart2Packet_T s_tUsartRxPacket;
    s_tUsartRxPacket = Usart2Receive();

    switch (s_tUsartRxPacket.eCommand)
    {
    case USART2_CMD_NONE:
        break;
    case USART2_CMD_QUERY_HELLO:
    	s_tUsartRxPacket.u8Data0++;
        Usart2Send(s_tUsartRxPacket);
        break;
    default:
        break;
    }
}

//----------------------------------------------------------------------------------
// Function name: TMR6_GLOBAL_IRQHandler
// Input:
// Output:
// Comment: Timer 6 interrupt handler
//----------------------------------------------------------------------------------
void Usart2HandleGlobal(void)
{
	static uint32_t counter = 0;
    Usart2Packet_T s_tUsartTxPacket;
	counter++;
	if(counter > 2000)
	{
		counter = 0;
        s_tUsartTxPacket.eCommand = 0xCC;
        s_tUsartTxPacket.u8Data3 = 0x03;
        s_tUsartTxPacket.u8Data2 = 0x02;
        s_tUsartTxPacket.u8Data1 = 0x01;
        s_tUsartTxPacket.u8Data0 = 0x00;
		Usart2Send(s_tUsartTxPacket);
	}
    Usart2ReceiveCmd();

}
