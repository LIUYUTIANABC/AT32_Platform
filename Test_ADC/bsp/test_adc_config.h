//----------------------------------------------------------------------------------
// File Name: test_adc_config.h
// Create Date: 2023-03-06 11:01:19
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#ifndef __TEST_ADC_CONFIG_H
#define __TEST_ADC_CONFIG_H

//----------------------------------------------------------------------------------
// Include file
//----------------------------------------------------------------------------------
#include "stdio.h"
#include "at32f421.h"

//----------------------------------------------------------------------------------
// Define
//----------------------------------------------------------------------------------
/* Cortex-Mx core  */
#define CORE_SYSTICK
#define CORE_DWT_TIMESTAMP
#define NVIC_PRIORITY_GROUP NVIC_PRIORITY_GROUP_4
#define SYSTEM_CLOCK (120000000)

/*
* which peripheral you used
*/
#define PERIPH_GPIOA     CRM_GPIOA_PERIPH_CLOCK
// #define PERIPH_GPIOB  CRM_GPIOB_PERIPH_CLOCK
#define PERIPH_GPIOF     CRM_GPIOF_PERIPH_CLOCK
#define PERIPH_TIMER_6   CRM_TMR6_PERIPH_CLOCK
#define PERIPH_USART2    CRM_USART2_PERIPH_CLOCK
#define PERIPH_DMA1      CRM_DMA1_PERIPH_CLOCK
#define PERIPH_TIMER_17  CRM_TMR17_PERIPH_CLOCK
#define PERIPH_ADC1      CRM_ADC1_PERIPH_CLOCK

/*
* NVIC_PRIORITY_GROUP_4; it is no priority_sub
*/
#define NVIC_NO_PRIO_SUB 0
#define NVIC_PRIO_PREEMPT_SYSTEM_CYCLE_TIMER6 0
#define NVIC_PRIO_PREEMPT_USART2_TX_DMA_COMPLETE 4
#define NVIC_PRIO_PREEMPT_USART2_RX_INT 4
#define NVIC_PRIO_PREEMPT_IR_RECEIVE_TIMER17 0
#define NVIC_PRIO_PREEMPT_ADC_LUX_DMA_COMPLETE 1

/*
* GPIOA Map
*/
#define GPIOA_PIN0_KEY_BOARD              GPIO_PINS_0
#define GPIOA_KEY_BOARD                   GPIOA
#define GPIOA_PIN2_USART2_TX              GPIO_PINS_2
#define GPIOA_MUX_USART2_TX               GPIO_MUX_1
#define GPIOA_USART2_TX                   GPIOA
#define GPIOA_PIN3_USART2_RX              GPIO_PINS_3
#define GPIOA_MUX_USART2_RX               GPIO_MUX_1
#define GPIOA_USART2_RX                   GPIOA
#define GPIOA_PIN4_ANALOG_LUX             GPIO_PINS_4
#define GPIOA_ADC_LUX                     GPIOA
#define GPIOA_PIN7_IR_RX                  GPIO_PINS_7
#define GPIOA_MUX_IR_RX                   GPIO_MUX_5
#define GPIOA_IR_RX                       GPIOA
#define GPIOA_PIN12_LED_BLUE              GPIO_PINS_12
#define GPIOA_LED_BLUE                    GPIOA

/*
* GPIOB Map
*/
// #define GPIOB_PIN11_LED4                  GPIO_PINS_11
// #define GPIOB_LED4                        GPIOB

/*
* GPIOF Map
*/
#define GPIOF_PIN6_LED_GREEN              GPIO_PINS_6
#define GPIOF_LED_GREEN                   GPIOF
#define GPIOF_PIN7_LED_RED                GPIO_PINS_7
#define GPIOF_LED_RED                     GPIOF

/*
* System Cycle -- Timer6
*/
#define SYSTEM_CYCLE_TIMER TMR6
#define SYSTEM_CYCLE_DIV (12000)
#define SYSTEM_CYCLE_2MS (SYSTEM_CLOCK / SYSTEM_CYCLE_DIV / 500)  // 2ms means 500HZ

/*
* USART2 configure
*/
#define USART2_MODE (8N1)
#define USART2_BAUD_RATE (115200)

/*
* IR configure
*/
#define IR_PROTOCOL (NEC)

/*
* ADC configure
*/
#define ADC_DMA_MODE_ENABLE
#define ADC_TRIGGER_SOFTWARE
#define ADC_CLOCK_DIV_20MHZ CRM_ADC_DIV_6
#define ADC_CHANNEL_VREF ADC_CHANNEL_17
#define ADC_CHANNEL_LUX ADC_CHANNEL_4
#define ADC_CHANNEL_LENGTH 2

//----------------------------------------------------------------------------------
// enum; struct; union; typedef;
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Global variables
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Static variables (this file)
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Function prototypes
//----------------------------------------------------------------------------------

#endif

/****************************** END OF FILE ***************************************/

