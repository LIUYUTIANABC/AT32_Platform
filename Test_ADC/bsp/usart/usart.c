//----------------------------------------------------------------------------------
// File Name: usart.c
// Create Date: 2023-03-07 15:24:01
// Developer: Rick Liu
// Version: 1.0.0
// Copyright: 2023. Dongguan Evolt Electronics Co., Ltd. All Rights Reserved
// Comment:
//----------------------------------------------------------------------------------
#include "usart.h"

//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
static uint8_t s_u8Usart2TxBuffer[USART2_PACKET_SIZE] = {0};
static uint8_t s_u8Usart2RxBuffer[USART2_PACKET_SIZE] = {0};
static Usart2Flags_T g_tUsart2Flag;

//----------------------------------------------------------------------------------
// Static function (this file)
//----------------------------------------------------------------------------------
static void Usart2Init(uint32_t _baud_rate);
static void Usart2TxDmaInterruptInit(void);
static void Usart2RxDmaInit(void);

//----------------------------------------------------------------------------------
// Function name: Usart2DmaInterruptInit
// Input:
// Output:
// Comment: USART2 use DMA transmitter and receiver data
//----------------------------------------------------------------------------------
void Usart2DmaInterruptInit(uint32_t _baud_rate)
{
    Usart2Init(_baud_rate);
    Usart2TxDmaInterruptInit();
    Usart2RxDmaInit();
}

//----------------------------------------------------------------------------------
// Function name: Usart2TxDmaInit
// Input:
// Output:
// Comment: USART2 init
//----------------------------------------------------------------------------------
static void Usart2Init(uint32_t _baud_rate)
{
    /* configure usart2 param */
    usart_init(USART2, _baud_rate, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_transmitter_enable(USART2, TRUE);
    usart_receiver_enable(USART2, TRUE);
    /* receive Interrupt */
    nvic_irq_enable(USART2_IRQn, NVIC_PRIO_PREEMPT_USART2_RX_INT, NVIC_NO_PRIO_SUB);
    usart_interrupt_enable(USART2, USART_IDLE_INT, TRUE);
    /* DMA  */
    usart_dma_transmitter_enable(USART2, TRUE);
    usart_dma_receiver_enable(USART2, TRUE);
    usart_enable(USART2, TRUE);
}

//----------------------------------------------------------------------------------
// Function name: Usart2TxDmaInit
// Input:
// Output:
// Comment: USART2 use DMA transmitter and receiver data
//----------------------------------------------------------------------------------
static void Usart2TxDmaInterruptInit(void)
{
    dma_init_type dma_init_struct;

    /* dma1 channel4 for usart2 tx configuration */
    dma_reset(DMA1_CHANNEL4);
    dma_default_para_init(&dma_init_struct);
    dma_init_struct.buffer_size = USART2_PACKET_SIZE;
    dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_base_addr = (uint32_t)s_u8Usart2TxBuffer;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
    dma_init_struct.memory_inc_enable = TRUE;
    dma_init_struct.peripheral_base_addr = (uint32_t)&USART2->dt;
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
    dma_init_struct.loop_mode_enable = FALSE;
    dma_init(DMA1_CHANNEL4, &dma_init_struct);

    /* enable transfer full data intterrupt */
    dma_interrupt_enable(DMA1_CHANNEL4, DMA_FDT_INT, TRUE);

    /* dma1 channel4 interrupt nvic init */
    nvic_irq_enable(DMA1_Channel5_4_IRQn, NVIC_PRIO_PREEMPT_USART2_TX_DMA_COMPLETE, NVIC_NO_PRIO_SUB);
}

//----------------------------------------------------------------------------------
// Function name: Usart2RxDmaInit
// Input:
// Output:
// Comment: USART2 use DMA transmitter and receiver data
//----------------------------------------------------------------------------------
static void Usart2RxDmaInit(void)
{
    dma_init_type dma_init_struct;

    /* dma1 channel5 for usart2 rx configuration */
    dma_reset(DMA1_CHANNEL5);
    dma_default_para_init(&dma_init_struct);
    dma_init_struct.buffer_size = USART2_PACKET_SIZE;
    dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_base_addr = (uint32_t)s_u8Usart2RxBuffer;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
    dma_init_struct.memory_inc_enable = TRUE;
    dma_init_struct.peripheral_base_addr = (uint32_t)&USART2->dt;
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
    dma_init_struct.loop_mode_enable = FALSE;
    dma_init(DMA1_CHANNEL5, &dma_init_struct);

    dma_channel_enable(DMA1_CHANNEL5, TRUE);
}

//----------------------------------------------------------------------------------
// Function name: Usart2Send
// Input:
// Output:
// Comment: USART2 send data
//----------------------------------------------------------------------------------
void Usart2Send(Usart2Packet_T _t_packet)
{
    /* Usart2 TX DMA is busy  */
    if(g_tUsart2Flag.bits.usart2TxDmaCompleteFlag)
    {
        return;
    }

    /* Set Usart2 TX DMA busy  */
    g_tUsart2Flag.bits.usart2TxDmaCompleteFlag = 1;
    s_u8Usart2TxBuffer[0] = USART2_PACKET_START_BYTE;
    s_u8Usart2TxBuffer[1] = _t_packet.eCommand;
    s_u8Usart2TxBuffer[2] = _t_packet.u8Data3;
    s_u8Usart2TxBuffer[3] = _t_packet.u8Data2;
    s_u8Usart2TxBuffer[4] = _t_packet.u8Data1;
    s_u8Usart2TxBuffer[5] = _t_packet.u8Data0;
    // calibration byte
    s_u8Usart2TxBuffer[6] = _t_packet.eCommand ^ _t_packet.u8Data3 ^ _t_packet.u8Data2
                            ^ _t_packet.u8Data1 ^ _t_packet.u8Data0;
    s_u8Usart2TxBuffer[7] = USART2_PACKET_END_BYTE;

    /* restart DMA */
    Usart2TxDmaInterruptInit();
    //    dma_data_number_set(DMA1_CHANNEL4, USART2_PACKET_SIZE);
    /* usart2 Tx begin dma send */
    dma_channel_enable(DMA1_CHANNEL4, TRUE);
}

//----------------------------------------------------------------------------------
// Function name: Usart2Receive
// Input:
// Output:
// Comment: USART2 receive data
//----------------------------------------------------------------------------------
Usart2Packet_T Usart2Receive(void)
{
    Usart2Packet_T tPacket;
    /* Usart2 TX DMA is busy  */
    if(g_tUsart2Flag.bits.usart2RxCompleteFlag)
    {
        g_tUsart2Flag.bits.usart2RxCompleteFlag = 0;

        if((s_u8Usart2RxBuffer[0] == USART2_PACKET_START_BYTE)
            && (s_u8Usart2RxBuffer[USART2_PACKET_SIZE - 1] == USART2_PACKET_END_BYTE)
            && (s_u8Usart2RxBuffer[1] ^ s_u8Usart2RxBuffer[2] ^ s_u8Usart2RxBuffer[3]
                ^ s_u8Usart2RxBuffer[4] ^ s_u8Usart2RxBuffer[5] == s_u8Usart2RxBuffer[6]))
        {
            tPacket.eCommand = s_u8Usart2RxBuffer[1];
            tPacket.u8Data3 = s_u8Usart2RxBuffer[2];
            tPacket.u8Data2 = s_u8Usart2RxBuffer[3];
            tPacket.u8Data1 = s_u8Usart2RxBuffer[4];
            tPacket.u8Data0 = s_u8Usart2RxBuffer[5];
        }
        /* Enable DMA Receive */
        usart_receiver_enable(USART2, TRUE);
        usart_dma_receiver_enable(USART2, TRUE);
        Usart2RxDmaInit();

        return tPacket;
    }
    tPacket.eCommand = USART2_CMD_NONE;
    return tPacket;
}

//----------------------------------------------------------------------------------
// Function name: Usart2Send
// Input:
// Output:
// Comment: USART2 send data
//----------------------------------------------------------------------------------
void Usart2TxDmaISR(void)
{
    g_tUsart2Flag.bits.usart2TxDmaCompleteFlag = 0;
    dma_channel_enable(DMA1_CHANNEL4, FALSE);
}

//----------------------------------------------------------------------------------
// Function name: Usart2RxIntISR
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
void Usart2RxIntIdleISR(void)
{
    g_tUsart2Flag.bits.usart2RxCompleteFlag = 1;
    /* Disable DMA */
    dma_channel_enable(DMA1_CHANNEL5, FALSE);
    usart_receiver_enable(USART2, FALSE);
    usart_dma_receiver_enable(USART2, FALSE);
}

