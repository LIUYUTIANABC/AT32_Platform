/**
  **************************************************************************
  * @file     main.c
  * @version  v2.0.8
  * @date     2022-08-16
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#include "at32f421_board.h"
#include "at32f421_clock.h"
#include "system_defines.h"

/** @addtogroup AT32F421_periph_template
  * @{
  */

/** @addtogroup 421_LED_toggle LED_toggle
  * @{
  */

#define DELAY                            100
#define FAST                             1
#define SLOW                             4

uint8_t g_speed = FAST;

void button_exint_init(void);
void button_isr(void);

/**
  * @brief  configure button exint
  * @param  none
  * @retval none
  */
void button_exint_init(void)
{
  exint_init_type exint_init_struct;

  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);

  scfg_exint_line_config(SCFG_PORT_SOURCE_GPIOA, SCFG_PINS_SOURCE0);

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = EXINT_LINE_0;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(EXINT1_0_IRQn, 0, 0);
}

/**
  * @brief  button handler function
  * @param  none
  * @retval none
  */
void button_isr(void)
{
  /* delay 5ms */
  delay_ms(5);

  /* clear interrupt pending bit */
  exint_flag_clear(EXINT_LINE_0);

  /* check input pin state */
  if(SET == gpio_input_data_bit_read(USER_BUTTON_PORT, USER_BUTTON_PIN))
  {
    if(g_speed == SLOW)
      g_speed = FAST;
    else
      g_speed = SLOW;
  }
}

/**
  * @brief  exint0 interrupt handler
  * @param  none
  * @retval none
  */
void EXINT1_0_IRQHandler(void)
{
  button_isr();
}


void SofewareDelay(u32 _delay)
{
    int i = 0, j = 0;
    for (i = 0; i < _delay; i++)
    {
        for ( j = 0; j < 1000; j++)
        {
            ;
        }
    }
}

    u8 flag = 0;
/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

//   at32_board_init();

//   button_exint_init();
    TestAdcBoardInit();

    u8 counter= 0;
    u32 i = 0;
    Usart2Packet_T tUart;
    int size = sizeof(tUart);
    size = sizeof(i);

    uint32_t trm = 0, trm1 = 0;
    double time = 0;

    trm = GetSystemCoreClock();
    trm = GetCoreDwtCurValue();
    SystickDelayUs(10);
    trm1 = GetCoreDwtCurValue();
    time = (trm1-trm) * (1.0/120);
    time = 0;

    trm = 0;
    trm1 = 0;

  while(1)
  {
    while (g_tSystemFlags.bits.timer2msFlag == 0)
    {
        // ADC or power saving here?
        AdcAverageProcess();
    }
    g_tSystemFlags.bits.timer2msFlag = 0;
    // LedRedOn();
    LedGreenOn();

    trm++;
    if(trm % 2 == 0)
    {
    	// if(counter == 1)
		// {
		// 	counter = 0;
		// 	LedRedOn();
		// }
		// else
		// {
		// 	counter = 1;
		// 	LedRedOff();
		// }
    }

    if(GetKeyBoardStatus() == SET)
    {
        LedGreenOn();
    }
    else
    {
        LedGreenOff();
    }
    Usart2HandleGlobal();
    IrHandleGlobal();
    AdcHandleGlobal();
    M24C16_HandleGlobal();
  }
}
