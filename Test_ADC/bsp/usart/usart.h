//----------------------------------------------------------------------------------
// File Name: usart.h
// Create Date: 2023-03-07 15:24:01
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#ifndef __USART_H
#define __USART_H

//----------------------------------------------------------------------------------
// Include file
//----------------------------------------------------------------------------------
#include "..\test_adc_config.h"

//----------------------------------------------------------------------------------
// Define
//----------------------------------------------------------------------------------
#define USART2_PACKET_SIZE 8
#define USART2_PACKET_START_BYTE 0x55
#define USART2_PACKET_END_BYTE 0xAA

//----------------------------------------------------------------------------------
// enum; struct; union; typedef;
//----------------------------------------------------------------------------------
typedef __PACKED_STRUCT
{
    __PACKED_UNION
    {
        volatile uint8_t all;
        __PACKED_STRUCT
        {
            volatile uint8_t usart2TxDmaCompleteFlag : 1;
            volatile uint8_t usart2RxCompleteFlag : 1;
            volatile uint8_t reserved : 6; /* [7:1] */
        } bits;
    };
} Usart2Flags_T;

typedef enum
{
    USART2_CMD_NONE = 0,
    USART2_CMD_QUERY_HELLO,
    USART2_CMD_QUERY_ADC_LUX,
} Usart2Command_E;

typedef __PACKED_STRUCT
{
    volatile Usart2Command_E eCommand;
    volatile uint8_t u8Data3;
    volatile uint8_t u8Data2;
    volatile uint8_t u8Data1;
    volatile uint8_t u8Data0;
} Usart2Packet_T;

//----------------------------------------------------------------------------------
// Global variables
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Static variables (this file)
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Function prototypes
//----------------------------------------------------------------------------------
void Usart2DmaInterruptInit(uint32_t _baud_rate);
void Usart2Send(Usart2Packet_T _t_packet);
Usart2Packet_T Usart2Receive(void);
void Usart2TxDmaISR(void);
void Usart2RxIntIdleISR(void);

#endif
