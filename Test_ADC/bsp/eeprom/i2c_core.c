//----------------------------------------------------------------------------------
// File Name: i2c_core.c
// Create Date: 2023-02-03 16:12:35
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------

#include "i2c_core.h"

//----------------------------------------------------------------------------------
// Static function declaration
//----------------------------------------------------------------------------------
static i2c_status_type i2cWaitFlag(i2c_handle_type *_hi2c, uint32_t _flag);

//----------------------------------------------------------------------------------
// Function name: i2c_lowlevel_init1
// Input:
// Output:
// Comment: Init i2c in AT32
//----------------------------------------------------------------------------------
void i2cCoreInit(i2c_type *_i2c_port, uint32_t _i2c_speed)
{
    /* reset i2c peripheral */
    i2c_reset(_i2c_port);

    /* i2c peripheral initialization */
    i2c_init(_i2c_port, I2C_FSMODE_DUTY_2_1, _i2c_speed);
    i2c_own_address1_set(_i2c_port, I2C_ADDRESS_MODE_7BIT, I2Cx_OWN_ADDRESS);

    /* i2c peripheral enable */
    i2c_enable(_i2c_port, TRUE);
}

//----------------------------------------------------------------------------------
// Function name: IICWriteOneByte
// Input:
// Output:
// Comment: Write one byte to IIC bus
//----------------------------------------------------------------------------------
i2c_status_type i2cWriteByte(i2c_handle_type *_hi2c)
{
    /* wait for the busy flag to be reset */
    if(i2cWaitFlag(_hi2c, I2C_BUSYF_FLAG) != I2C_OK)
    {
        /* return error */
        return I2C_ERR_BUSY;
    }

    /* ack acts on the current byte */
    i2c_master_receive_ack_set(_hi2c->i2cx, I2C_MASTER_ACK_CURRENT);

    /* generate start condtion */
    i2c_start_generate(_hi2c->i2cx);
    /* wait for the start flag to be set */
    if(i2cWaitFlag(_hi2c, I2C_STARTF_FLAG) != I2C_OK)
    {
        /* generate stop condtion */
        i2c_stop_generate(_hi2c->i2cx);
        /* return error */
        return I2C_ERR_START;
    }

    /* send slave address */
    i2c_7bit_address_send(_hi2c->i2cx, _hi2c->msb_addr, I2C_DIRECTION_TRANSMIT);
    /* wait for the addr7 flag to be set */
    if(i2cWaitFlag(_hi2c, I2C_ADDR7F_FLAG) != I2C_OK)
    {
        /* generate stop condtion */
        i2c_stop_generate(_hi2c->i2cx);
        /* return error */
        return I2C_ERR_ADDR;
    }
    /* clear addr flag */
    i2c_flag_clear(_hi2c->i2cx, I2C_ADDR7F_FLAG);

    /* wait for the tdbe flag to be set */
    if(i2cWaitFlag(_hi2c, I2C_TDBE_FLAG) != I2C_OK)
    {
        /* generate stop condtion */
        i2c_stop_generate(_hi2c->i2cx);
        /* return error */
        return I2C_ERR_DATA;
    }
    /* write low address */
    i2c_data_send(_hi2c->i2cx, _hi2c->lsb_addr);
    /* wait for the tdbe flag to be set */
    if(i2cWaitFlag(_hi2c, I2C_TDBE_FLAG) != I2C_OK)
    {
        /* generate stop condtion */
        i2c_stop_generate(_hi2c->i2cx);
        /* return error */
        return I2C_ERR_DATA;
    }
    /* write data */
    i2c_data_send(_hi2c->i2cx, _hi2c->data);
    /* wait for the tdc flag to be set */
    if(i2cWaitFlag(_hi2c, I2C_TDC_FLAG) != I2C_OK)
    {
        /* generate stop condtion */
        i2c_stop_generate(_hi2c->i2cx);
        /* return error */
        return I2C_ERR_DATA;
    }

    /* generate stop condtion */
    i2c_stop_generate(_hi2c->i2cx);

    return I2C_OK;
}

//----------------------------------------------------------------------------------
// Function name: IICReadOneByte
// Input:
// Output:
// Comment: Read one byte from IIC bus
//----------------------------------------------------------------------------------
i2c_status_type i2cReadByte(i2c_handle_type *_hi2c)
{
    /* wait for the busy flag to be reset */
    if(i2cWaitFlag(_hi2c, I2C_BUSYF_FLAG) != I2C_OK)
    {
        /* return error */
        return I2C_ERR_BUSY;
    }

    /* ack acts on the current byte */
    i2c_master_receive_ack_set(_hi2c->i2cx, I2C_MASTER_ACK_CURRENT);

    /* enable ack */
    i2c_ack_enable(_hi2c->i2cx, TRUE);

    /* generate start condtion */
    i2c_start_generate(_hi2c->i2cx);
    /* wait for the start flag to be set */
    if(i2cWaitFlag(_hi2c, I2C_STARTF_FLAG) != I2C_OK)
    {
        /* generate stop condtion */
        i2c_stop_generate(_hi2c->i2cx);
        /* return error */
        return I2C_ERR_START;
    }

    /* send slave address */
    i2c_7bit_address_send(_hi2c->i2cx, _hi2c->msb_addr, I2C_DIRECTION_TRANSMIT);
    /* wait for the addr7 flag to be set */
    if(i2cWaitFlag(_hi2c, I2C_ADDR7F_FLAG) != I2C_OK)
    {
        /* generate stop condtion */
        i2c_stop_generate(_hi2c->i2cx);
        /* return error */
        return I2C_ERR_ADDR;
    }
    /* clear addr flag */
    i2c_flag_clear(_hi2c->i2cx, I2C_ADDR7F_FLAG);

    /* wait for the tdbe flag to be set */
    if(i2cWaitFlag(_hi2c, I2C_TDBE_FLAG) != I2C_OK)
    {
        /* generate stop condtion */
        i2c_stop_generate(_hi2c->i2cx);
        /* return error */
        return I2C_ERR_DATA;
    }
    /* write low address */
    i2c_data_send(_hi2c->i2cx, _hi2c->lsb_addr);
    /* wait for the tdc flag to be set */
    if(i2cWaitFlag(_hi2c, I2C_TDC_FLAG) != I2C_OK)
    {
        /* generate stop condtion */
        i2c_stop_generate(_hi2c->i2cx);
        /* return error */
        return I2C_ERR_DATA;
    }

    /* generate start condtion */
    i2c_start_generate(_hi2c->i2cx);
    /* wait for the start flag to be set */
    if(i2cWaitFlag(_hi2c, I2C_STARTF_FLAG) != I2C_OK)
    {
        /* generate stop condtion */
        i2c_stop_generate(_hi2c->i2cx);
        /* return error */
        return I2C_ERR_START;
    }

    /* send slave address */
    i2c_7bit_address_send(_hi2c->i2cx, _hi2c->msb_addr, I2C_DIRECTION_RECEIVE);
    /* wait for the addr7 flag to be set */
    if(i2cWaitFlag(_hi2c, I2C_ADDR7F_FLAG) != I2C_OK)
    {
        /* generate stop condtion */
        i2c_stop_generate(_hi2c->i2cx);
        /* return error */
        return I2C_ERR_ADDR;
    }
    /* clear addr flag */
    i2c_flag_clear(_hi2c->i2cx, I2C_ADDR7F_FLAG);

    /* disable ack */
    i2c_ack_enable(_hi2c->i2cx, FALSE);

    /* generate stop condtion */
    i2c_stop_generate(_hi2c->i2cx);

    /* wait for the tdbe flag to be set */
    if(i2cWaitFlag(_hi2c, I2C_RDBF_FLAG) != I2C_OK)
    {
        /* generate stop condtion */
        i2c_stop_generate(_hi2c->i2cx);
        /* return error */
        return I2C_ERR_DATA;
    }

    /* read data */
    _hi2c->data = i2c_data_receive(_hi2c->i2cx);

    return I2C_OK;
}

//----------------------------------------------------------------------------------
// Function name: i2cWaitFlag
// Input:
// Output:
// Comment: Check flag if it is set or
//----------------------------------------------------------------------------------
static i2c_status_type i2cWaitFlag(i2c_handle_type *_hi2c, uint32_t _flag)
{
    uint16_t timeout = I2C_TIMEOUT;

    if(_flag == I2C_BUSYF_FLAG)
    {
        /* wait for the busy flag to be reset */
        while(i2c_flag_get(_hi2c->i2cx, _flag))
        {
            /* check timeout */
            if((timeout--) == 0)
            {
                /* return error */
                return I2C_ERR_TIMEOUT;
            }
        }
    }
    else
    {
        while(i2c_flag_get(_hi2c->i2cx, _flag) == RESET)
        {
            /* check timeout */
            if((timeout--) == 0)
            {
                /* return error */
                return I2C_ERR_TIMEOUT;
            }
        }
    }

    return I2C_OK;
}
