#ifndef NOS_UART
#define NOS_UART

#include "NOS_Includes.h"

#define UART_ADDRESS 0x65

typedef struct NOS_UART_Struct_t
{
    uint8_t rx_buff[256];
    uint8_t fuckBuff[256];
    uint16_t fuckIndex = 0;
    bool rx_flag = false;
    bool tx_flag = false;
    int currMessageLenght = 0;
    int expectedMessageLenght = 0;
    bool startReceive = false;
    bool endReceive = false;
    bool bufferCommand = false;
    NOS_Short value;

}NOS_UART_Struct;

void NOS_UART_ReceiveReset(NOS_UART_Struct* data,uint8_t* rx_buff_ptr)
{
  rx_buff_ptr = data->rx_buff;
  data->currMessageLenght = 0;
  data->rx_flag = true;
}

void NOS_UART_ContinueReceive(NOS_UART_Struct* data, uint8_t* rx_buff_ptr)
{
  data->rx_buff[data->currMessageLenght] = *rx_buff_ptr;
  ++rx_buff_ptr;
}

void NOS_UART_ReceiveHandler(NOS_UART_Struct* data,UART_HandleTypeDef* uart)
{
    static uint8_t* rx_buff_ptr = data->rx_buff;

    data->fuckBuff[data->fuckIndex] = *rx_buff_ptr;
    data->fuckIndex++;

    if(*rx_buff_ptr == UART_ADDRESS && !data->startReceive)
    {
        data->startReceive = true;
        data->currMessageLenght = 0;

        data->fuckIndex = 1;
        for(int i = 0; i < 1024; i++)
        {
            data->fuckBuff[i] = 0;
        }
        data->fuckBuff[0] = *rx_buff_ptr; 
    }

    if(data->startReceive && data->currMessageLenght == 1 && !data->bufferCommand)
    {
        data->value.bytes[1] = *rx_buff_ptr;
    }

    if(data->startReceive && data->currMessageLenght == 2 && !data->bufferCommand)
    {
        data->value.bytes[0] = *rx_buff_ptr;
        data->expectedMessageLenght = data->value.data;
        data->bufferCommand = true;
    }

    if(data->fuckIndex > 256)
    {
        data->fuckIndex = 0;
    }

    data->currMessageLenght++;

    if(data->startReceive && (data->currMessageLenght == data->expectedMessageLenght) && data->bufferCommand)
    {
        NOS_UART_ReceiveReset(data,rx_buff_ptr);
        data->bufferCommand = false;
        data->startReceive = false;
    }
    else
    {
        NOS_UART_ContinueReceive(data,rx_buff_ptr);
    }
    HAL_UART_Receive_IT (uart, rx_buff_ptr, 1); 
}


#endif