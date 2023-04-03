//----------------------------------------------------------------------------------
// File Name: timestamp.c
// Create Date: 2023-03-22 11:59:56
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#include "timestamp.h"

//----------------------------------------------------------------------------------
// define value
//----------------------------------------------------------------------------------
/* REGISTERS */
#define  DWT_CR      *(uint32_t *)0xE0001000
#define  DWT_CYCCNT  *(uint32_t *)0xE0001004
#define  DEM_CR      *(uint32_t *)0xE000EDFC
#define  DBGMCU_CR   *(uint32_t *)0xE0042004

/* REGISTER BITS */
#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)

//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Function name: CoreDwtTimestampInit
// Input:
// Output:
// Comment: config DWT to record timestamp
//----------------------------------------------------------------------------------
void CoreDwtTimestampInit(void)
{
    /* Enable DWT */
    DEM_CR         |= (uint32_t)DEM_CR_TRCENA;
    /* Clear DWT CYCCNT */
    DWT_CYCCNT      = (uint32_t)0u;
    /* Open Cortex-M3 DWT CYCCNT */
    DWT_CR         |= (uint32_t)DWT_CR_CYCCNTENA;
}

//----------------------------------------------------------------------------------
// Function name: GetCoreDwtCurValue
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
uint32_t GetCoreDwtCurValue(void)
{
    return ((uint32_t)DWT_CYCCNT);
}
