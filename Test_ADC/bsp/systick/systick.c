//----------------------------------------------------------------------------------
// File Name: systick.c
// Create Date: 2023-03-07 15:25:19
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#include "systick.h"

//----------------------------------------------------------------------------------
// define value
//----------------------------------------------------------------------------------
/* delay macros */
#define STEP_DELAY_MS             50

//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
static __IO uint32_t s_u32FacUs;
static __IO uint32_t s_u32FacMs;

//----------------------------------------------------------------------------------
// Function name: initialize delay function
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
void SystickInit(void)
{
    /* configure systick */
    systick_clock_source_config(SYSTICK_CLOCK_SOURCE_AHBCLK_NODIV);
    s_u32FacUs = system_core_clock / (1000000U);
    s_u32FacMs = s_u32FacUs * (1000U);
}

//----------------------------------------------------------------------------------
// Function name: inserts a delay time.
// Input: (_nus) specifies the delay time length, in micro_second.
// Output:
// Comment:
//----------------------------------------------------------------------------------
void SystickDelayUs(uint32_t _nus)
{
    uint32_t temp = 0;
    SysTick->LOAD = (uint32_t)(_nus * s_u32FacUs);
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
}

//----------------------------------------------------------------------------------
// Function name: inserts a delay time.
// Input: (_nms) specifies the delay time length, in milli_seconds.
// Output:
// Comment:
//----------------------------------------------------------------------------------
void SystickDelayMs(uint16_t _nms)
{
    uint32_t temp = 0;
    while (_nms)
    {
        if (_nms > STEP_DELAY_MS)
        {
            SysTick->LOAD = (uint32_t)(STEP_DELAY_MS * s_u32FacMs);
            _nms -= STEP_DELAY_MS;
        }
        else
        {
            SysTick->LOAD = (uint32_t)(_nms * s_u32FacMs);
            _nms = 0;
        }
        SysTick->VAL = 0x00;
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
        do
        {
            temp = SysTick->CTRL;
        } while ((temp & 0x01) && !(temp & (1 << 16)));

        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
        SysTick->VAL = 0x00;
    }
}

//----------------------------------------------------------------------------------
// Function name: inserts a delay time.
// Input: _sec: specifies the delay time, in _seconds.
// Output:
// Comment:
//----------------------------------------------------------------------------------
void SystickDelaySec(uint16_t _sec)
{
    uint16_t index;
    for (index = 0; index < _sec; index++)
    {
        SystickDelayMs(500);
        SystickDelayMs(500);
    }
}
