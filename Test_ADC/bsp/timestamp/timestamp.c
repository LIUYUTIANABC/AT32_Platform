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

/************** Use DWT to get Time ********************

/// @brief This function turns on the free running counter in the system ticks.
///        It is useful for counting the time since last activation or similar.
void ARMSystemInit()
{
    ARM_CM_DEMCR |= 1 << 24;
    ARM_CM_DWT_CYCCNT = 0;
    ARM_CM_DWT_CTRL |= 1 << 0;
}

/// @brief Gets the current count of clock cycles.
/// @return Number of clock cycles since starting. Will auto roll over 0xFFFF -> 0x0
u32 ARMSystemGetSystemCounter()
{
    return ARM_CM_DWT_CYCCNT;
}

/// @brief Gets the difference in clock cycles (can handle 1 roll over of the timer. (u32 - 1) number)
/// @param start The first clock cycle measurement.
/// @param stop The second click cycle measurement.
/// @return the difference between the two clocks as the number of clock cycles.
u32 ARMSystemCalculateClockDifference(u32 *start, u32 *stop)
{

    if ((u32)*start < (u32)*stop)
    {
        return (u32)((u32)*stop - (u32)*start);
    }

    // since the start is greater than the stop assume the counter has rolled over.
    return (u32)((u32)0xFFFFFFFF - (u32)*start + (u32)*stop);
}

/// @brief Calculates the number of micro seconds with a given number of clock cycles.
/// @param clockCycles Number of clock cycles.
/// @return The number of microseconds. (Assuming 120MHz Clock)
u32 ARMSystemCalculateUs(u32 *clockCycles)
{
    // WARNING!!!: This assumes a 120MHz Clock;
    return (u32)(*clockCycles / 120);
}

************** EXIT interrupt Trigger ****************************

void DaliRxISR(void)
{
    u8 currentLevel = DaliReadInput(); // Get GPIO status
    DaliIdleTimer = 0;
    // Local level getting time since last int,
    u32 time = ARMSystemGetSystemCounter();
    DaliTimeSinceLastINT = ARMSystemCalculateClockDifference(&DaliTimeOfLastINT,
                                                                   &time);
    DaliTimeOfLastINT = time;
    DaliTimeSinceLastINT = ARMSystemCalculateUs(&DaliTimeSinceLastINT);

    if (DaliBusState < IDLE)
    {
        if (currentLevel != DaliSetLevel)
        {
            DaliTxCollision = 1;
            DaliBusState = IDLE;
        }
        DaliRxTimer = 0;
        return;
    }

    switch (DaliBusState)
    {
    case WAIT_RX:
    case IDLE: // Listen to the bus.
        if (currentLevel == LOW)
        {
            DaliBusState = RX_START;
        }
        else
        {
            DaliBusState = IDLE;
        }
        break;
    case RX_START:
        if (currentLevel == HIGH)
        {
            DaliRxLength = 0;
            DaliRxMessage = 1;
            DaliBusState = RX_BIT;
        }
        else
        {
            DaliRxLength = 0;
            DaliBusState = RX_STOP;
        }
        break;
    case RX_BIT:
        if (DaliIsDaliTimeValid() == TRUE)
        {
            if (DaliRxLength % 2)
            {
                DaliRxMessage = DaliRxMessage << 1 | currentLevel;
            }
            DaliRxLength++;
        }
        else if (DaliIsDaliTimeValid2() == TRUE)
        {
            DaliRxMessage = DaliRxMessage << 1 | currentLevel;
            DaliRxLength += 2;
        }
        else
        {
            // DaliBusState = RX_STOP;
        }
        break;
    case RX_STOP:
        if (currentLevel == HIGH)
        {
        }
        break;
    }
    DaliRxTimer = 0;
    return;
}

************** Use DWT to get Time ********************/
