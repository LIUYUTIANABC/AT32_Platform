//----------------------------------------------------------------------------------
// File Name: led.c
// Create Date: 2023-03-07 11:55:26
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#include "led_base.h"

//----------------------------------------------------------------------------------
// Define public interface
//----------------------------------------------------------------------------------
/* Led2 on */
void LedBlueOn(void)
{
    gpio_bits_set(GPIOA_LED_BLUE, GPIOA_PIN12_LED_BLUE);
}
/* Led2 off */
void LedBlueOff(void)
{
    gpio_bits_reset(GPIOA_LED_BLUE, GPIOA_PIN12_LED_BLUE);
}
/* Led3 on */
void LedGreenOn(void)
{
    gpio_bits_set(GPIOF_LED_GREEN, GPIOF_PIN6_LED_GREEN);
}
/* Led3 on */
void LedGreenOff(void)
{
    gpio_bits_reset(GPIOF_LED_GREEN, GPIOF_PIN6_LED_GREEN);
}
/* Led4 on */
void LedRedOn(void)
{
    gpio_bits_set(GPIOF_LED_RED, GPIOF_PIN7_LED_RED);
}
/* Led4 off */
void LedRedOff(void)
{
    gpio_bits_reset(GPIOF_LED_RED, GPIOF_PIN7_LED_RED);
}
