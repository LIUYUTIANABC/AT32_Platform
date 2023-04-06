//----------------------------------------------------------------------------------
// File Name: ir_handler.c
// Create Date: 2023-04-01 09:41:21
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#include "m24c16_handler.h"

//----------------------------------------------------------------------------------
// Function name: IrHandleGlobal
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
void M24C16_HandleGlobal(void)
{
    static uint16_t counter = 0;
    static uint8_t i = 0, j = 0, status = 0;
    uint8_t data = 0;
    Usart2Packet_T packet;

    counter++;
    if (counter >= 5000)
    {
        counter = 0;
        i++;
        if (status == 0)
        {
            status = 1;
            if(m24c16_ReadByte(0x0001, &data) == M24C16_OK_READ)
            {
                packet.eCommand = USART2_CMD_QUERY_M24C16_READ;
                packet.u8Data3 = data;
                Usart2Send(packet);
            }
        }
        else
        {
            if(m24c16_WriteByte(0x0001, i) == M24C16_OK_WRITE)
            {
                packet.eCommand = USART2_CMD_QUERY_M24C16_WRITE;
                packet.u8Data3 = i;
                Usart2Send(packet);
            }
            status = 0;
        }
    }
}
