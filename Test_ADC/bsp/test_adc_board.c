//----------------------------------------------------------------------------------
// File Name: test_adc_board.c
// Create Date: 2023-03-06 11:04:17
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#include "test_adc_board.h"

//----------------------------------------------------------------------------------
// Static function (this file)
//----------------------------------------------------------------------------------
static void OutputTypeGpioInit(gpio_type *_gpio_x, uint32_t _gpio_pin);
static void InputTypeGpioPullUp(gpio_type *_gpio_x, uint32_t _gpio_pin);
static void MuxTypeGpioInit(gpio_type *_gpio_x, uint32_t _gpio_pin, gpio_mux_sel_type _gpio_mux);
static void MuxTypeOutputOpenDrainInit(gpio_type *_gpio_x, uint32_t _gpio_pin, gpio_mux_sel_type _gpio_mux);
static void AnalogTypeGpioInit(gpio_type *_gpio_x, uint32_t _gpio_pin);
static void SystemCycle2msTime6Init(void);

//----------------------------------------------------------------------------------
// Function name: TestAdcBoardInit
// Input:
// Output:
// Comment: Init all hardware
//----------------------------------------------------------------------------------
void TestAdcBoardInit(void)
{
    //----------------------------------------------------------------------------------
    // SysTick Init
    //----------------------------------------------------------------------------------
    SystickInit();
    CoreDwtTimestampInit();

    //----------------------------------------------------------------------------------
    // peripheral clock Init
    //----------------------------------------------------------------------------------
    crm_periph_clock_enable(PERIPH_GPIOA, TRUE);
    crm_periph_clock_enable(PERIPH_GPIOB, TRUE);
    crm_periph_clock_enable(PERIPH_GPIOF, TRUE);
    crm_periph_clock_enable(PERIPH_TIMER_6, TRUE);
    crm_periph_clock_enable(PERIPH_USART2, TRUE);
    crm_periph_clock_enable(PERIPH_DMA1, TRUE);
    crm_periph_clock_enable(PERIPH_TIMER_17, TRUE);
    crm_periph_clock_enable(PERIPH_ADC1, TRUE);
    crm_adc_clock_div_set(ADC_CLOCK_DIV_20MHZ);    /* ADC clock is ( 120MHZ(SYS_CLK) / 6 = 20MHZ ) */
    crm_periph_clock_enable(PERIPH_I2C2, TRUE);

    //----------------------------------------------------------------------------------
    // NVIC group
    //----------------------------------------------------------------------------------
    nvic_priority_group_config(NVIC_PRIORITY_GROUP);

    //----------------------------------------------------------------------------------
    // PORT A Init
    //----------------------------------------------------------------------------------
    InputTypeGpioPullUp(GPIOA_KEY_BOARD, GPIOA_PIN0_KEY_BOARD);
    MuxTypeGpioInit(GPIOA_USART2_TX, GPIOA_PIN2_USART2_TX, GPIOA_MUX_USART2_TX);
    MuxTypeGpioInit(GPIOA_USART2_RX, GPIOA_PIN3_USART2_RX, GPIOA_MUX_USART2_RX);
    AnalogTypeGpioInit(GPIOA_ADC_LUX, GPIOA_PIN4_ANALOG_LUX);
    MuxTypeGpioInit(GPIOA_IR_RX, GPIOA_PIN7_IR_RX, GPIOA_MUX_IR_RX);
    OutputTypeGpioInit(GPIOA_LED_BLUE, GPIOA_PIN12_LED_BLUE);

    //----------------------------------------------------------------------------------
    // PORT B Init
    //----------------------------------------------------------------------------------
    MuxTypeOutputOpenDrainInit(GPIOB_M24C16_SCL, GPIOB_PIN10_M24C16_SCL_PIN, GPIOB_MUX_M24C16_SCL);
    MuxTypeOutputOpenDrainInit(GPIOB_M24C16_SDA, GPIOB_PIN11_M24C16_SDA_PIN, GPIOB_MUX_M24C16_SDA);

    //----------------------------------------------------------------------------------
    // PORT F Init
    //----------------------------------------------------------------------------------
    OutputTypeGpioInit(GPIOF_LED_GREEN, GPIOF_PIN6_LED_GREEN);
    OutputTypeGpioInit(GPIOF_LED_RED, GPIOF_PIN7_LED_RED);

    //----------------------------------------------------------------------------------
    // Timer6 Init
    //----------------------------------------------------------------------------------
    SystemCycle2msTime6Init();

    //----------------------------------------------------------------------------------
	// USART2 Init
	//----------------------------------------------------------------------------------
    Usart2DmaInterruptInit(USART2_BAUD_RATE);

    /* Timer17 IR receive Init */
    IrReceiveTimer17Init();

    /* ADC Lux and Vref channels init */
    AdcLuxDmaInterruptInit();

    /* M24C16 and I2C init */
    m24c16_Init();
}

//----------------------------------------------------------------------------------
// Function name: GetSystemCoreClock
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
uint32_t GetSystemCoreClock(void)
{
    return system_core_clock;
}

//----------------------------------------------------------------------------------
// Function name: OutputTypeGpioInit
// Input:
// Output:
// Comment: Set this IO with output mode
//----------------------------------------------------------------------------------
static void OutputTypeGpioInit(gpio_type *_gpio_x, uint32_t _gpio_pin)
{
    gpio_init_type gpio_init_struct;

    /* set default parameter */
    gpio_default_para_init(&gpio_init_struct);

    /* configure the led gpio */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_pins = _gpio_pin;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(_gpio_x, &gpio_init_struct);
}

//----------------------------------------------------------------------------------
// Function name: InputTypeGpioPullUp
// Input:
// Output:
// Comment: Set the IO is input mode with pull-up
//----------------------------------------------------------------------------------
static void InputTypeGpioPullUp(gpio_type *_gpio_x, uint32_t _gpio_pin)
{
    gpio_init_type gpio_init_struct;

    /* set default parameter */
    gpio_default_para_init(&gpio_init_struct);

    /* configure the led gpio */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
    gpio_init_struct.gpio_pins = _gpio_pin;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init(_gpio_x, &gpio_init_struct);
}

//----------------------------------------------------------------------------------
// Function name: MuxTypeGpioInit
// Input:
// Output:
// Comment: select GPIO mux function
//----------------------------------------------------------------------------------
static void MuxTypeGpioInit(gpio_type *_gpio_x, uint32_t _gpio_pin, gpio_mux_sel_type _gpio_mux)
{
    gpio_init_type gpio_init_struct;
    gpio_pins_source_type gpio_pins_source;

    /* set default parameter */
    gpio_default_para_init(&gpio_init_struct);

    /* configure the led gpio */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pins = _gpio_pin;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(_gpio_x, &gpio_init_struct);

    /* config iomux */
    switch(_gpio_pin)
    {
    case GPIO_PINS_0: gpio_pins_source = GPIO_PINS_SOURCE0; break;
    case GPIO_PINS_1: gpio_pins_source = GPIO_PINS_SOURCE1; break;
    case GPIO_PINS_2: gpio_pins_source = GPIO_PINS_SOURCE2; break;
    case GPIO_PINS_3: gpio_pins_source = GPIO_PINS_SOURCE3; break;
    case GPIO_PINS_4: gpio_pins_source = GPIO_PINS_SOURCE4; break;
    case GPIO_PINS_5: gpio_pins_source = GPIO_PINS_SOURCE5; break;
    case GPIO_PINS_6: gpio_pins_source = GPIO_PINS_SOURCE6; break;
    case GPIO_PINS_7: gpio_pins_source = GPIO_PINS_SOURCE7; break;
    case GPIO_PINS_8: gpio_pins_source = GPIO_PINS_SOURCE8; break;
    case GPIO_PINS_9: gpio_pins_source = GPIO_PINS_SOURCE9; break;
    case GPIO_PINS_10: gpio_pins_source = GPIO_PINS_SOURCE10; break;
    case GPIO_PINS_11: gpio_pins_source = GPIO_PINS_SOURCE11; break;
    case GPIO_PINS_12: gpio_pins_source = GPIO_PINS_SOURCE12; break;
    case GPIO_PINS_13: gpio_pins_source = GPIO_PINS_SOURCE13; break;
    case GPIO_PINS_14: gpio_pins_source = GPIO_PINS_SOURCE14; break;
    case GPIO_PINS_15: gpio_pins_source = GPIO_PINS_SOURCE15; break;
    }
    gpio_pin_mux_config(_gpio_x, gpio_pins_source, _gpio_mux);
}

//----------------------------------------------------------------------------------
// Function name: OutputOpenDrainTypeInit
// Input:
// Output:
// Comment: Used on the I2C bus or in other bus
//----------------------------------------------------------------------------------
static void MuxTypeOutputOpenDrainInit(gpio_type *_gpio_x, uint32_t _gpio_pin, gpio_mux_sel_type _gpio_mux)
{
    gpio_init_type gpio_init_struct;
    gpio_pins_source_type gpio_pins_source;

    /* set default parameter */
    gpio_default_para_init(&gpio_init_struct);

    /* gpio configuration */
    gpio_init_struct.gpio_out_type       = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_init_struct.gpio_pull           = GPIO_PULL_UP;
    gpio_init_struct.gpio_mode           = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pins           = _gpio_pin;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init(_gpio_x, &gpio_init_struct);

    /* config iomux */
    switch(_gpio_pin)
    {
    case GPIO_PINS_0: gpio_pins_source = GPIO_PINS_SOURCE0; break;
    case GPIO_PINS_1: gpio_pins_source = GPIO_PINS_SOURCE1; break;
    case GPIO_PINS_2: gpio_pins_source = GPIO_PINS_SOURCE2; break;
    case GPIO_PINS_3: gpio_pins_source = GPIO_PINS_SOURCE3; break;
    case GPIO_PINS_4: gpio_pins_source = GPIO_PINS_SOURCE4; break;
    case GPIO_PINS_5: gpio_pins_source = GPIO_PINS_SOURCE5; break;
    case GPIO_PINS_6: gpio_pins_source = GPIO_PINS_SOURCE6; break;
    case GPIO_PINS_7: gpio_pins_source = GPIO_PINS_SOURCE7; break;
    case GPIO_PINS_8: gpio_pins_source = GPIO_PINS_SOURCE8; break;
    case GPIO_PINS_9: gpio_pins_source = GPIO_PINS_SOURCE9; break;
    case GPIO_PINS_10: gpio_pins_source = GPIO_PINS_SOURCE10; break;
    case GPIO_PINS_11: gpio_pins_source = GPIO_PINS_SOURCE11; break;
    case GPIO_PINS_12: gpio_pins_source = GPIO_PINS_SOURCE12; break;
    case GPIO_PINS_13: gpio_pins_source = GPIO_PINS_SOURCE13; break;
    case GPIO_PINS_14: gpio_pins_source = GPIO_PINS_SOURCE14; break;
    case GPIO_PINS_15: gpio_pins_source = GPIO_PINS_SOURCE15; break;
    }
    gpio_pin_mux_config(_gpio_x, gpio_pins_source, _gpio_mux);
}

//----------------------------------------------------------------------------------
// Function name: AnalogTypeGpioInit
// Input:
// Output:
// Comment: Set GPIO is analog mode
//----------------------------------------------------------------------------------
static void AnalogTypeGpioInit(gpio_type *_gpio_x, uint32_t _gpio_pin)
{
    gpio_init_type gpio_initstructure;

    gpio_default_para_init(&gpio_initstructure);
    gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
    gpio_initstructure.gpio_pins = _gpio_pin;
    gpio_init(_gpio_x, &gpio_initstructure);
}

//----------------------------------------------------------------------------------
// Function name: SystemCycle2msTime6Init
// Input:
// Output:
// Comment: system cycle is 2ms
//----------------------------------------------------------------------------------
static void SystemCycle2msTime6Init(void)
{
    /* tmr6 configuration */
    tmr_base_init(SYSTEM_CYCLE_TIMER, SYSTEM_CYCLE_2MS - 1, SYSTEM_CYCLE_DIV - 1);

    /* overflow interrupt enable */
    tmr_interrupt_enable(SYSTEM_CYCLE_TIMER, TMR_OVF_INT, TRUE);

    /* tmr6 overflow interrupt nvic init */
    nvic_irq_enable(TMR6_GLOBAL_IRQn, NVIC_PRIO_PREEMPT_SYSTEM_CYCLE_TIMER6, NVIC_NO_PRIO_SUB);

    /* enable tmr6 */
    tmr_counter_enable(SYSTEM_CYCLE_TIMER, TRUE);
}
