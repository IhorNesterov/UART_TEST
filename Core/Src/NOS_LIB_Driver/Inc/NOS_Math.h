#ifndef NOS_MATH
#define NOS_MATH
#include "NOS_Includes.h"

typedef struct SinValue_t
{
    int value;

    int minValue;
    int maxValue;
    int increment;
    bool rise;
    bool fall;
}SinValue;

int NOS_Math_GetSinValue(SinValue* value);
void NOS_Math_SinValue_Handler(SinValue* value);
void NOS_Math_SinValue_Init(SinValue* value,int minValue,int maxValue,int increment);
int NOS_Math_NormalizeValue(int value,int maxVal);
int NOS_Math_GetNumberPower(int value);

#endif