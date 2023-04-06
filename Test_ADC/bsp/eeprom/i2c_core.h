//----------------------------------------------------------------------------------
// File Name: i2c_core.h
// Create Date: 2023-02-03 15:48:17
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#ifndef __I2C_CORE_H
#define __I2C_CORE_H

//----------------------------------------------------------------------------------
// Include file
//----------------------------------------------------------------------------------
#include "stdio.h"
#include "at32f421.h"

//----------------------------------------------------------------------------------
// Define
//----------------------------------------------------------------------------------
#define I2C_TIMEOUT                      0x3FF // waiting 8us
#define I2Cx_OWN_ADDRESS                 0x50

//----------------------------------------------------------------------------------
// enum; struct; union; typedef;
//----------------------------------------------------------------------------------
typedef enum
{
  I2C_OK = 0,          /*!< no error */
  I2C_ERR_BUSY,        /*!< busy error> */
  I2C_ERR_START,       /*!< start error */
  I2C_ERR_ADDR,        /*!< addr error */
  I2C_ERR_DATA,        /*!< data error */
  I2C_ERR_TIMEOUT,     /*!< timeout error */
} i2c_status_type;

typedef struct
{
  i2c_type                          *i2cx;                  /*!< i2c registers base address      */
  uint8_t                           msb_addr;
  uint8_t                           lsb_addr;
  uint8_t                           data;
} i2c_handle_type;

//----------------------------------------------------------------------------------
// Global variables
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Static variables (this file)
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Function prototypes
//----------------------------------------------------------------------------------
void i2cCoreInit(i2c_type *_i2c_port, uint32_t _i2c_speed);
i2c_status_type i2cWriteByte(i2c_handle_type *_hi2c);
i2c_status_type i2cReadByte(i2c_handle_type *_hi2c);

#endif

/****************************** END OF FILE ***************************************/
