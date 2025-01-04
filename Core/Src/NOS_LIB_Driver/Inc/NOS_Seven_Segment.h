#include "NOS_Typedefs.h"
#include "stm32f1xx.h"

typedef struct NOS_Seven_Segment_t
{
    GPIO_PIN SEG_A;
    GPIO_PIN SEG_B;
    GPIO_PIN SEG_C;
    GPIO_PIN SEG_D;
    GPIO_PIN SEG_E;
    GPIO_PIN SEG_F;
    GPIO_PIN SEG_G;
    GPIO_PIN SEG_DP;

    GPIO_PIN COM1;
    GPIO_PIN COM2;
    GPIO_PIN COM3;
    GPIO_PIN COM4;

    uint32_t timer;
    uint32_t com_time;

    char digits[4];
    uint8_t pdigits[4];

}NOS_Seven_Segment;

void NOS_SGD_prvPrintDigit(NOS_Seven_Segment* sgd,int pos)
{
        uint8_t s = sgd->pdigits[pos];

        if(s & (1 << 7)) 
        { 
            HAL_GPIO_WritePin(sgd->SEG_A.Port,sgd->SEG_A.Pin,GPIO_PIN_SET);
        }
        else 
        {
            HAL_GPIO_WritePin(sgd->SEG_A.Port,sgd->SEG_A.Pin,GPIO_PIN_RESET);
        }

        if(s & (1 << 6)) 
        { 
            HAL_GPIO_WritePin(sgd->SEG_B.Port,sgd->SEG_B.Pin,GPIO_PIN_SET);
        }
        else 
        {
            HAL_GPIO_WritePin(sgd->SEG_B.Port,sgd->SEG_B.Pin,GPIO_PIN_RESET);
        }

        if(s & (1 << 5)) 
        { 
            HAL_GPIO_WritePin(sgd->SEG_C.Port,sgd->SEG_C.Pin,GPIO_PIN_SET);
        }
        else 
        {
            HAL_GPIO_WritePin(sgd->SEG_C.Port,sgd->SEG_C.Pin,GPIO_PIN_RESET);
        }

        if(s & (1 << 4)) 
        { 
            HAL_GPIO_WritePin(sgd->SEG_D.Port,sgd->SEG_D.Pin,GPIO_PIN_SET);
        }
        else 
        {
            HAL_GPIO_WritePin(sgd->SEG_D.Port,sgd->SEG_D.Pin,GPIO_PIN_RESET);
        }

        if(s & (1 << 3)) 
        { 
            HAL_GPIO_WritePin(sgd->SEG_E.Port,sgd->SEG_E.Pin,GPIO_PIN_SET);
        }
        else 
        {
            HAL_GPIO_WritePin(sgd->SEG_E.Port,sgd->SEG_E.Pin,GPIO_PIN_RESET);
        }

        if(s & (1 << 2)) 
        { 
            HAL_GPIO_WritePin(sgd->SEG_F.Port,sgd->SEG_F.Pin,GPIO_PIN_SET);
        }
        else 
        {
            HAL_GPIO_WritePin(sgd->SEG_F.Port,sgd->SEG_F.Pin,GPIO_PIN_RESET);
        }

        if(s & (1 << 1)) 
        { 
            HAL_GPIO_WritePin(sgd->SEG_G.Port,sgd->SEG_G.Pin,GPIO_PIN_SET);
        }
        else 
        {
            HAL_GPIO_WritePin(sgd->SEG_G.Port,sgd->SEG_G.Pin,GPIO_PIN_RESET);
        }

        if(s & (1 << 0)) 
        { 
            HAL_GPIO_WritePin(sgd->SEG_DP.Port,sgd->SEG_DP.Pin,GPIO_PIN_SET);
        }
        else 
        {
            HAL_GPIO_WritePin(sgd->SEG_DP.Port,sgd->SEG_DP.Pin,GPIO_PIN_RESET);
        }
}


void NOS_SGD_PrintDigit(NOS_Seven_Segment* sgd,int pos)
{
    switch (sgd->digits[pos])
    {
    case '0':
        sgd->pdigits[pos] = 252;
        break;
	case '1' : 
		sgd->pdigits[pos] = 0b01100000;
		break;
    case '2':
        sgd->pdigits[pos] = 218;
        break;
    case '3':
        sgd->pdigits[pos] = 242;
        break;
    case '4':
        sgd->pdigits[pos] = 0b01100110;
        break;
    case '5':
        sgd->pdigits[pos] = 182;
        break;
    case '6':
        sgd->pdigits[pos] = 190;
        break;
    case '7':
        sgd->pdigits[pos] = 224;
        break;
    case '8':
        sgd->pdigits[pos] = 254;
        break;
    case '9':
        sgd->pdigits[pos] = 246;
        break;

	case 'A' : 
		sgd->pdigits[pos] = 0b11101110;
		break;
	case 'B' : 
		sgd->pdigits[pos] = 0b11111111;
		break;
	case 'C' : 
		sgd->pdigits[pos] = 0b10011100;
		break;
	case 'D' : 
		sgd->pdigits[pos] = 0b11111101;
		break;
	case 'E' : 
		sgd->pdigits[pos] = 0b10011110;
		break;
	case 'F' : 
		sgd->pdigits[pos] = 0b10001110;
		break;
	case 'G' : 
		sgd->pdigits[pos] = 0b10111111;
		break;
	case 'H' : 
		sgd->pdigits[pos] = 0b01101111;
		break;
	case 'I' : 
		sgd->pdigits[pos] = 0b01100001;
		break;
	case 'J' : 
		sgd->pdigits[pos] = 0b01110001;
		break;
	case 'L' : 
		sgd->pdigits[pos] = 0b00011101;
		break;
	case 'O' : 
		sgd->pdigits[pos] = 0b11111101;
		break;
	case 'P' : 
		sgd->pdigits[pos] = 0b11001111;
		break;
	case 'Q' : 
		sgd->pdigits[pos] = 0b11111101;
		break;
	case 'R' : 
		sgd->pdigits[pos] = 0b11101111;
		break;
	case 'S' : 
		sgd->pdigits[pos] = 0b10110111;
		break;
	case 'U' : 
		sgd->pdigits[pos] = 0b01111101;
		break;
	case 'Y' : 
		sgd->pdigits[pos] = 0b01110111;
		break;
	case 'Z' : 
		sgd->pdigits[pos] = 0b11011011;
		break;


    default:
        sgd->pdigits[pos] = 0b00000000;
        break;
    }

    NOS_SGD_prvPrintDigit(sgd,pos);
}

void NOS_SGD_COMMON_SWITCH(NOS_Seven_Segment* sgd,int pos)
{
    switch (pos)
    {
    case 0:
        HAL_GPIO_WritePin(sgd->COM1.Port,sgd->COM1.Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(sgd->COM2.Port,sgd->COM2.Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(sgd->COM3.Port,sgd->COM3.Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(sgd->COM4.Port,sgd->COM4.Pin,GPIO_PIN_SET);
        break;
    case 1:
        HAL_GPIO_WritePin(sgd->COM1.Port,sgd->COM1.Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(sgd->COM2.Port,sgd->COM2.Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(sgd->COM3.Port,sgd->COM3.Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(sgd->COM4.Port,sgd->COM4.Pin,GPIO_PIN_SET);
        break;
    
    case 2:
        HAL_GPIO_WritePin(sgd->COM1.Port,sgd->COM1.Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(sgd->COM2.Port,sgd->COM2.Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(sgd->COM3.Port,sgd->COM3.Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(sgd->COM4.Port,sgd->COM4.Pin,GPIO_PIN_SET);
        break;

    case 3:
        HAL_GPIO_WritePin(sgd->COM1.Port,sgd->COM1.Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(sgd->COM2.Port,sgd->COM2.Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(sgd->COM3.Port,sgd->COM3.Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(sgd->COM4.Port,sgd->COM4.Pin,GPIO_PIN_RESET);
        break;

    default:
        HAL_GPIO_WritePin(sgd->COM1.Port,sgd->COM1.Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(sgd->COM2.Port,sgd->COM2.Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(sgd->COM3.Port,sgd->COM3.Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(sgd->COM4.Port,sgd->COM4.Pin,GPIO_PIN_SET);
        break;
    }
}

void NOS_SGD_Timer_Handler(NOS_Seven_Segment* sgd)
{
    if(sgd->timer < sgd->com_time)
    {
        NOS_SGD_PrintDigit(sgd,0);
        NOS_SGD_COMMON_SWITCH(sgd,0);
    }
    else if(sgd->timer > sgd->com_time && sgd->timer <= sgd->com_time * 2)
    {
        NOS_SGD_PrintDigit(sgd,1);
        NOS_SGD_COMMON_SWITCH(sgd,1);
    }
    else if(sgd->timer > sgd->com_time * 2 && sgd->timer <= sgd->com_time * 3)
    {
        NOS_SGD_PrintDigit(sgd,2);
        NOS_SGD_COMMON_SWITCH(sgd,2);
    }
    else if(sgd->timer > sgd->com_time * 3 && sgd->timer <= sgd->com_time * 4)
    {
        NOS_SGD_PrintDigit(sgd,3);
        NOS_SGD_COMMON_SWITCH(sgd,3);
    }
    else if(sgd->timer > sgd->com_time * 4)
    {
        sgd->timer = 0;
    }

    sgd->timer++;
}

void NOS_SGD_SetIntData(NOS_Seven_Segment* sgd,int number)
{
    sgd->digits[0] = number / 1000 + 48;
    sgd->digits[1] = ((number % 1000) / 100) + 48;
    sgd->digits[2] = (((number % 1000) % 100) / 10) + 48;
    sgd->digits[3] = (((number % 1000) % 100) % 10) + 48;
}

char NOS_SGD_prvConvertHex(int num)
{
    switch (num)
    {
    case 0:
        return '0';
        break;
    case 1:
        return '1';
        break;
    case 2:
        return '2';
        break;
    case 3:
        return '3';
        break;
    case 4:
        return '4';
        break;
    case 5:
        return '5';
        break;
    case 6:
        return '6';
        break;
    case 7:
        return '7';
        break;
    case 8:
        return '8';
        break;
    case 9:
        return '9';
        break;
    case 10:
        return 'A';
        break;
    case 11:
        return 'B';
        break;
    case 12:
        return 'C';
        break;
    case 13:
        return 'D';
        break;
    case 14:
        return 'E';
        break;
    case 15:
        return 'F';
        break;
    default:
        return 0;
        break;
    }

}

void NOS_SGD_SetHexData(NOS_Seven_Segment* sgd,int number)
{
    int first = 16*16*16;
    int second = 16*16;
    int third = 16;
    int num = number / first;
    int curr = num;

    sgd->digits[0] = NOS_SGD_prvConvertHex(num);
    curr = number - (num * first); 

    num = curr / second;
    sgd->digits[1] = NOS_SGD_prvConvertHex(num);
    curr = curr - (num * second);

    num = curr / third;
    sgd->digits[2] = NOS_SGD_prvConvertHex(num);
    curr = curr - (num * third);

    sgd->digits[3] = NOS_SGD_prvConvertHex(curr);   
}

void NOS_SGD_SetStringData(NOS_Seven_Segment* sgd,char* string)
{
    sgd->digits[0] = string[0];
    sgd->digits[1] = string[1];
    sgd->digits[2] = string[2];
    sgd->digits[3] = string[3];
}

void NOS_SGD_Test(NOS_Seven_Segment* sgd)
{  
    NOS_SGD_SetIntData(sgd,8888);
}