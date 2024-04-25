#ifndef NOS_UART
#define NOS_UART

#include "NOS_Includes.h"

#define UART_ADDRESS 0x65
#define BUFFER_SIZE 1024

typedef struct NOS_UART_Struct_t
{
    uint8_t rx_buff[BUFFER_SIZE];
    uint8_t fuckBuff[BUFFER_SIZE];
    int index;
    uint16_t fuckIndex;
    bool rx_flag;
    bool tx_flag;
    int currMessageLenght;
    int expectedMessageLenght;
    bool startReceive;
    bool endReceive;
    bool bufferCommand;
    NOS_Short value;
    uint8_t* rx_buff_ptr;

}NOS_UART_Struct;

NOS_UART_Struct* NOS_UART_ReceiveReset(NOS_UART_Struct* data)
{
  data->rx_buff_ptr = data->rx_buff;
  data->currMessageLenght = 0;
  data->rx_flag = true;
  return data;
}

NOS_UART_Struct* NOS_UART_ContinueReceive(NOS_UART_Struct* data)
{
    data->rx_buff[data->currMessageLenght] = *data->rx_buff_ptr;
    ++data->rx_buff_ptr;
    return data;
}

void NOS_UART_ReceiveAbort(NOS_UART_Struct* data)
{
    for(int i = 0; i < BUFFER_SIZE; i++)
    {
        data->rx_buff[i] = 0;
    }

    data->index = 0;
    data->currMessageLenght = 0;
    data->expectedMessageLenght = 0;
    data->startReceive = false;
    data->endReceive = false;
    data->bufferCommand = false;
    data->rx_flag = false;
}

void NOS_UART_ReceiveHandler(NOS_UART_Struct* data,UART_HandleTypeDef* uart)
{
    data->rx_buff_ptr = &data->rx_buff[data->currMessageLenght];
    data->fuckBuff[data->fuckIndex] = *data->rx_buff_ptr;
    data->fuckIndex++;

    if(*data->rx_buff_ptr == UART_ADDRESS && !data->startReceive)
    {
        data->startReceive = true;
        data->currMessageLenght = 0;

        data->fuckIndex = 1;
        for(int i = 0; i < BUFFER_SIZE; i++)
        {
            data->fuckBuff[i] = 0;
        }
        data->fuckBuff[0] = *data->rx_buff_ptr; 
    }

    if(data->startReceive && data->currMessageLenght == 1 && !data->bufferCommand)
    {
        data->value.bytes[1] = *data->rx_buff_ptr;
    }

    if(data->startReceive && data->currMessageLenght == 2 && !data->bufferCommand)
    {
        data->value.bytes[0] = *data->rx_buff_ptr;
        data->expectedMessageLenght = data->value.data;
        data->bufferCommand = true;
    }

    if(data->fuckIndex > BUFFER_SIZE)
    {
        data->fuckIndex = 0;
    }

    data->currMessageLenght++;

    if(data->startReceive && (data->currMessageLenght == data->expectedMessageLenght) && data->bufferCommand)
    {
        data = NOS_UART_ReceiveReset(data);
        data->bufferCommand = false;
        data->startReceive = false;
    }
    else
    {
        data = NOS_UART_ContinueReceive(data);
    }
    HAL_UART_Receive_IT (uart, data->rx_buff_ptr, 1); 
}

bool NOS_UART_CheckReceive(NOS_UART_Struct* data)
{
    return data->rx_flag;   
}

uint8_t* NOS_UART_GetReceivedData(NOS_UART_Struct* data)
{
    return data->fuckBuff;
}

void NOS_UART_EndReceive(NOS_UART_Struct* data)
{
    data->rx_flag = false;
}


#endif