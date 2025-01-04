#ifndef NOS_STRIP_UART
#define NOS_STRIP_UART

#include "NOS_Includes.h"

#define UART_ADDRESS 0x65

//commands
#define CHECK_CONNECTION 0x20
#define SET_ONE_PIXEL_COLOR_COMMAND 0x30
#define SET_ROW_PIXEL_COLOR_COMMAND 0x31
#define SET_ALL_PIXEL_ONE_COLOR_COMMAND 0x32
#define SET_BRIGHT_COMMAND 0x33
#define SET_BREATHE_EFFECT_COMMAND 0x34
#define SET_ALL_PIXEL_COMMAND 0x35

const char* NOS_Strip_Uart_ParseCommand(WS2812B_Strip* strip,uint8_t* command);

#endif