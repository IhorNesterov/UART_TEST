#include "NOS_WS2812B_Strip.h"
#include "NOS_BaseColors.h"
#include "WS2812B_Effects.h"

void NOS_WS2812B_Strip_FullInit(WS2812B_Strip* strip,uint8_t* buff,PixelColor* pixels,int lenght)
{
    strip->pixels = pixels;
    strip->frameBuffer = buff;
    strip->pixelCount = lenght;
    strip->bright = 100;
    strip->effectsCounter = 0;
}

void NOS_WS2812B_Strip_CoreSetPixel(WS2812B_Strip* strip,int pixelPos,uint8_t r, uint8_t g, uint8_t b)
{
    if(strip->frameBuffer != NULL && pixelPos <= strip->pixelCount)
    {
        strip->frameBuffer[pixelPos * 3] = r;
        strip->frameBuffer[pixelPos * 3 + 1] = g;
        strip->frameBuffer[pixelPos * 3 + 2] = b;
    }
}

void NOS_WS2812B_Strip_SetPixel(WS2812B_Strip* strip,int pixelPos,uint8_t r, uint8_t g, uint8_t b)
{
    if(strip->pixels != NULL && pixelPos <= strip->pixelCount)
    {
        strip->pixels[pixelPos].R = r;
        strip->pixels[pixelPos].G = g;
        strip->pixels[pixelPos].B = b;
    }         
}

void NOS_WS2812B_Strip_Clear(WS2812B_Strip* strip)
{
    for(int i = 0; i < strip->pixelCount; i++)
    {
        NOS_WS2812B_Strip_SetPixel(strip,i,0,0,0);
    }
}

void NOS_WS2812B_Strip_Update(WS2812B_Strip* strip)
{
    float coef = (float)strip->bright / 100;
    for(int i = 0; i < strip->pixelCount; i++)
    {
        uint8_t currR = (float)strip->pixels[i].R * coef;
        uint8_t currG = (float)strip->pixels[i].G * coef;
        uint8_t currB = (float)strip->pixels[i].B * coef;
        NOS_WS2812B_Strip_CoreSetPixel(strip,i,currR,currG,currB);
    }

}

void NOS_WS2812B_Strip_TestFill(WS2812B_Strip* strip)
{
    int currPixel = 0;
    int currColor = 0;
    
    while(currPixel < strip->pixelCount)
    {
        NOS_WS2812B_Strip_SetPixelByPixelColor(strip,currPixel,NOS_GetBaseColor(currColor));
        
        currColor++;
        if(currColor > 11)
        {
            currColor = 0;
        }

        currPixel++;
    }
}

void NOS_WS2812B_Strip_ColorFill(WS2812B_Strip* strip,PixelColor color)
{
    for(int i = 0; i < strip->pixelCount; i++)
    {
        NOS_WS2812B_Strip_SetPixel(strip,i,color.R,color.G,color.B);
    }
}

void NOS_WS2812B_Strip_SetPixelByRGB(WS2812B_Strip* strip,int pixelPos,uint32_t rgb)
{
    uint8_t R = (rgb & 0x00FF0000) >> 16;
    uint8_t G = (rgb & 0x0000FF00) >> 8;
    uint8_t B = rgb & 0x000000FF;

    NOS_WS2812B_Strip_SetPixel(strip,pixelPos,R,G,B);
}

void NOS_WS2812B_Strip_SetPixelByPixelColor(WS2812B_Strip* strip,int pixelPos, PixelColor color)
{
    NOS_WS2812B_Strip_SetPixel(strip,pixelPos,color.R,color.G,color.B);
}

void NOS_WS2812B_Strip_SetBright(WS2812B_Strip* strip,uint8_t bright)
{
    strip->bright = bright;
}

void NOS_WS2812B_Strip_Effect_Breathe_Init(Effect_Struct* effect,uint16_t speed,uint16_t step,uint16_t minValue,uint16_t maxValue)
{
        NOS_Math_SinValue_Init(&effect->value,minValue,maxValue,step);
        effect->minValue = minValue;
        effect->maxValue = maxValue;
        effect->speed.data = speed;
        effect->step.data = step;
        effect->enabled = true;
        effect->effectId = 0x20;   
}

void NOS_WS2812B_Strip_Effects_AddEffect(WS2812B_Strip* strip,Effect_Struct effect)
{
    if(strip->effectsCounter < 10)
    {
        switch(effect.effectId)
        {
            case 0x20:
                NOS_WS2812B_Strip_Effect_Breathe_Init(&strip->effects[strip->effectsCounter],effect.speed.data,effect.step.data,effect.minValue,effect.maxValue);
                break;
            case 0x21:
                NOS_WS2812B_Strip_Effect_Rainbow_Init(&strip->effects[strip->effectsCounter],effect.speed.data,effect.step.data,effect.minValue,effect.maxValue);
                break;
        }

        strip->effectsCounter++;
    }
}

void NOS_WS2812B_Strip_Effects_Handler(WS2812B_Strip* strip)
{
    for(int i = 0; i < strip->effectsCounter; i++)
    {
        switch(strip->effects[i].effectId)
        {
            case 0x20:
                NOS_WS2812B_Strip_Effect_Breathe_Handler(strip,&strip->effects[i]);
                break;
            case 0x21:
                NOS_WS2812B_Strip_Effect_Rainbow_Handler(strip,&strip->effects[i]);
                break;
        }
    }
}

void NOS_WS2812B_Strip_Effect_Breathe_Handler(WS2812B_Strip* strip,Effect_Struct* effect)
{
    if(strip != NULL)
    {
        if(effect != NULL)
        {
            if(effect->enabled)
            {
                if(effect->timer > effect->speed.data)
                {
                    NOS_Math_SinValue_Handler(&effect->value);
                    NOS_WS2812B_Strip_SetBright(strip,effect->value.value);
                    NOS_WS2812B_Strip_Update(strip);
                    effect->timer = 0;
                }
                effect->timer++;
            }
        }
    }
}

void NOS_WS2812B_Strip_Effects_UpdateEffect(WS2812B_Strip* strip,Effect_Struct effect,uint8_t pos)
{
    if(pos < 10)
    {
        switch(effect.effectId)
        {
            case 0x20:
                NOS_WS2812B_Strip_Effect_Breathe_Init(&strip->effects[pos],effect.speed.data,effect.step.data,effect.minValue,effect.maxValue);
                break;
            case 0x21:
                NOS_WS2812B_Strip_Effect_Rainbow_Init(&strip->effects[pos],effect.speed.data,effect.step.data,effect.minValue,effect.maxValue);
                break;
        }
    }
}

void NOS_WS2812B_Strip_Effect_Rainbow_Init(Effect_Struct* effect,uint16_t speed,uint16_t step,uint16_t minValue,uint16_t maxValue)
{
        NOS_Math_SinValue_Init(&effect->value,minValue,maxValue,step);
        effect->minValue = minValue;
        effect->maxValue = maxValue;
        effect->speed.data = speed;
        effect->step.data = step;
        effect->enabled = true;
        effect->effectId = 0x21;  
}

void NOS_WS2812B_Strip_Effect_Rainbow_Handler(WS2812B_Strip* strip,Effect_Struct* effect)
{
    if(strip != NULL)
    {
        if(effect != NULL)
        {
            if(effect->enabled)
            {
                if(effect->timer > effect->speed.data)
                {
                    NOS_Math_SinValue_Handler(&effect->value);

                    uint16_t number = NOS_Math_GetSinValue(&effect->value);
                    uint8_t iteration = number / 0x7F;

                    PixelColor curr = { 0,0,0 };

                    switch (iteration)
                    {
                        case 0:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        curr.G = number;
                        break;

                        case 1:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        number = number - 0x7F * iteration;
                        curr.G = 0x7F + number;
                        break;

                        case 2:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        number = number - 0x7F * iteration;
                        curr.B = 0xFF - number;
                        break;

                        case 3:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        number = number - 0x7F * iteration;
                        curr.B = 0x7F - number;
                        break;

                        case 4:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        number = number - 0x7F * iteration;
                        curr.R = number;
                        break;

                        case 5:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        number = number - 0x7F * iteration;
                        curr.R = 0x7F + number;
                        break;

                        case 6:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        number = number - 0x7F * iteration;
                        curr.G = 0xFF - number;
                        break;

                        case 7:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        number = number - 0x7F * iteration;
                        curr.G = 0x7F - number;
                        break;

                        case 8:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        number = number - 0x7F * iteration;
                        curr.B = number;
                        break;

                        case 9:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        number = number - 0x7F * iteration;
                        curr.B = 0x7F + number;
                        break;

                        case 10:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        number = number - 0x7F * iteration;
                        curr.R = 0xFF - number;
                        break;

                        case 11:
                        curr = NOS_GetBaseColor((NOS_BaseColors)iteration);
                        number = number - 0x7F * iteration;
                        curr.R = 0x7F - number;
                        break;

                        default:
                        break;
                    }
                    

                    NOS_WS2812B_Strip_ColorFill(strip,curr);
                    NOS_WS2812B_Strip_Update(strip);
                    effect->timer = 0;
                }
                effect->timer++;
            }
        }
    }
}




