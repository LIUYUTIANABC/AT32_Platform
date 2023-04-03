//----------------------------------------------------------------------------------
// File Name: ir_base.h
// Create Date: 2023-03-23 17:20:23
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#ifndef __IR_BASE_H
#define __IR_BASE_H

//----------------------------------------------------------------------------------
// Include file
//----------------------------------------------------------------------------------
#include "..\test_adc_config.h"

//----------------------------------------------------------------------------------
// Define
//----------------------------------------------------------------------------------
#define IR_RX_TIMER17_SAMPLE_TIME_US (10)
#define IR_RX_TIMER17_DIV_US_10 (SYSTEM_CLOCK / 100000)

#define IR_RX_START_TOLERANCE 1000
#define IR_RX_DATA_TOLERANCE 200
#define IR_RX_START_TIME_US (9000 + 4500)
#define IR_RX_REPEAT_TIME_US (9000 + 2250)
#define IR_RX_DATA_0_TIME_US (562 + 563)
#define IR_RX_DATA_1_TIME_US (562 + 1648)
#define IR_RX_START_TIME_MIN_US (IR_RX_START_TIME_US - IR_RX_START_TOLERANCE)
#define IR_RX_START_TIME_MAX_US (IR_RX_START_TIME_US + IR_RX_START_TOLERANCE)
#define IR_RX_REPEAT_TIME_MIN_US (IR_RX_REPEAT_TIME_US - IR_RX_START_TOLERANCE)
#define IR_RX_REPEAT_TIME_MAX_US (IR_RX_REPEAT_TIME_US + IR_RX_START_TOLERANCE)
#define IR_RX_DATA_0_TIME_MIN_US (IR_RX_DATA_0_TIME_US - IR_RX_DATA_TOLERANCE)
#define IR_RX_DATA_0_TIME_MAX_US (IR_RX_DATA_0_TIME_US + IR_RX_DATA_TOLERANCE)
#define IR_RX_DATA_1_TIME_MIN_US (IR_RX_DATA_1_TIME_US - IR_RX_DATA_TOLERANCE)
#define IR_RX_DATA_1_TIME_MAX_US (IR_RX_DATA_1_TIME_US + IR_RX_DATA_TOLERANCE)

//----------------------------------------------------------------------------------
// enum; struct; union; typedef;
//----------------------------------------------------------------------------------
typedef enum
{
    IR_RECEIVE_IDLE = 0,
    IR_RECEIVE_START,
    IR_RECEIVE_DATA,
    IR_RECEIVE_REPEAT,
} IrReceiveStatus_E;

typedef __PACKED_UNION
{
    volatile uint8_t all;
    __PACKED_STRUCT
    {
        volatile uint8_t irRxNewDataFlag : 1;
        volatile uint8_t irRxRepeatFlag : 1;
        volatile uint8_t reserved : 6; /* [7:1] */
    } bits;
} IrReceiveFlags_T;

typedef __PACKED_STRUCT
{
    __PACKED_UNION
    {
        volatile uint8_t all;
        __PACKED_STRUCT
        {
            uint8_t irRxNewDataValidFlag : 1;
            uint8_t reserved : 7;
        } bits;
    };
    volatile uint8_t address;
    volatile uint8_t command;
} IrReceiverPacket_T;

//----------------------------------------------------------------------------------
// Global variables
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Static variables (this file)
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Function prototypes
//----------------------------------------------------------------------------------
void IrReceiveTimer17Init(void);
void IrReceiveISR(void);
void IrReceiveStatusClear(void);
IrReceiverPacket_T GetIrRxNewData(void);

#endif

/****************************** END OF FILE ***************************************/
