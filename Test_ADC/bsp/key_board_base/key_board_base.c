//----------------------------------------------------------------------------------
// File Name: led.c
// Create Date: 2023-03-07 11:55:26
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#include "key_board_base.h"

//----------------------------------------------------------------------------------
// Define public interface
//----------------------------------------------------------------------------------
flag_status GetKeyBoardStatus(void)
{
    return gpio_input_data_bit_read(GPIOA_KEY_BOARD, GPIOA_PIN0_KEY_BOARD);
}
