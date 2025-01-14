#ifndef NOS_BASECOLORS
#define NOS_BASECOLORS

#include "NOS_Includes.h"

typedef enum NOS_BaseColors_e {BLUE,OCEAN,CYAN,TURQUOISE,GREEN,SGREEN,YELLOW,ORANGE,RED,RASPBERRY,MAGENTA,VIOLET} NOS_BaseColors;

void NOS_PixelColor_SetColor(PixelColor* color,uint8_t R,uint8_t G,uint8_t B);
PixelColor NOS_GetBaseColor(NOS_BaseColors color);


#endif