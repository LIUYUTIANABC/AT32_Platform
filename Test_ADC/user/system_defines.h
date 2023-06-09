//----------------------------------------------------------------------------------
// File Name: system_defines.h
// Create Date: 2023-03-06 11:13:18
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#ifndef __SYSTEM_DEFINES_H
#define __SYSTEM_DEFINES_H

//----------------------------------------------------------------------------------
// Include file
//----------------------------------------------------------------------------------
#include "..\bsp\test_adc_board.h"
#include ".\int_irq_handler\int_irq_handler.h"
#include ".\usart2_handler\usart2_handler.h"
#include ".\ir_handler\ir_handler.h"
#include ".\adc_handler\adc_handler.h"
#include ".\m24c16_handler\m24c16_handler.h"

//----------------------------------------------------------------------------------
// Define
//----------------------------------------------------------------------------------
#ifdef GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif

//----------------------------------------------------------------------------------
// enum; struct; union; typedef;
//----------------------------------------------------------------------------------
typedef __PACKED_STRUCT
{
    __PACKED_UNION
    {
        volatile uint32_t all;
        __PACKED_STRUCT
        {
            volatile u32 timer2msFlag : 1;
            volatile u32 reserved : 31; /* [31:1] */
        } bits;
    };
} SystemFlags_T;

//----------------------------------------------------------------------------------
// Global variables
//----------------------------------------------------------------------------------
EXTERN SystemFlags_T g_tSystemFlags;

//----------------------------------------------------------------------------------
// Static variables (this file)
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Function prototypes
//----------------------------------------------------------------------------------

#endif

/****************************** END OF FILE ***************************************/
