//----------------------------------------------------------------------------------
// File Name: m24c16_api.c
// Create Date: 2023-02-03 16:14:31
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------

#include "m24c16_api.h"

//----------------------------------------------------------------------------------
// Function name: m24c16_Init
// Input:
// Output:
// Comment: Use IIC communication with M24C16
//----------------------------------------------------------------------------------
void m24c16_Init(void)
{
    /* i2c */
    i2cCoreInit(M24C16_I2Cx_PORT, M24C16_I2C_SPEED);
}

//----------------------------------------------------------------------------------
// Function name: m24c16_WriteOneByte
// Input:
// Output:
// Comment: Write one byte to M24C16
//----------------------------------------------------------------------------------
m24c16_status_type m24c16_WriteByte(uint16_t _addr, uint8_t _data)
{
    i2c_handle_type hi2cx;
    uint8_t rxData = 0;

    /* Address overflow */
    if(_addr > M24C16_ADDR_BYTE_MAX)
    {
        return M24C16_ERR_WRITE;
    }
    /* Check the byte data if the same before */
    if(m24c16_ReadByte(_addr, &rxData) != M24C16_OK_READ)
    {
        return M24C16_ERR_WRITE;
    }
    /* The data is the same with M24C16 */
    if(_data == rxData)
    {
        return M24C16_OK_WRITE;
    }
    /* Init i2c handle struct */
    hi2cx.i2cx = M24C16_I2Cx_PORT;
    hi2cx.msb_addr = (uint8_t)(M24C16_I2C_ADDRESS + ((_addr >> 7) & 0x00FF));
    hi2cx.lsb_addr = (uint8_t)(_addr & 0x00FF);
    hi2cx.data = _data;

    /* write a byte */
    if(i2cWriteByte(&hi2cx) != I2C_OK)
    {
        return M24C16_ERR_WRITE;
    }

    return M24C16_OK_WRITE;
}

//----------------------------------------------------------------------------------
// Function name: m24c16_ReadByte
// Input:
// Output:
// Comment: Read one byte from M24C16
//----------------------------------------------------------------------------------
m24c16_status_type m24c16_ReadByte(uint16_t _addr, uint8_t *_data_buff)
{
    i2c_handle_type hi2cx;

    /* Init i2c handle struct */
    hi2cx.i2cx = M24C16_I2Cx_PORT;
    hi2cx.msb_addr = (uint8_t)(M24C16_I2C_ADDRESS + ((_addr >> 7) & 0x00FF));
    hi2cx.lsb_addr = (uint8_t)(_addr & 0x00FF);
    hi2cx.data = 0;

    /* write a byte */
    if(i2cReadByte(&hi2cx) != I2C_OK)
    {
        return M24C16_ERR_READ;
    }

    /* Get the byte read from M24C16 */
    *_data_buff = hi2cx.data;
    return M24C16_OK_READ;
}
