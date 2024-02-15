#include "NOS_Lib.h"
#include "NOS_Typedefs.h"
#include "NOS_Math.h"
#include <stdlib.h>

#define MAX_LENGHT 32*8

typedef struct NOS_Snake_Game_t
{
   int lenght;
   PixelColor* headColor;
   PixelColor* bodyColor;
   PixelColor* foodColor;
   PixelColor* foneColor;

   Point food;
   Point head;
   Point body[MAX_LENGHT];
   MoveDirection direction;
   bool alive;
}NOS_Snake_Game;


//Create snake at point 4,4
void NOS_Snake_Game_DefaultInit(NOS_Snake_Game* snake,int _lenght)
{
    snake->direction = LEFT;
    snake->head.x = _lenght + 1;
    snake->head.y = 4;
    snake->lenght = _lenght;
    snake->alive = true;

    int currPoint = _lenght;

    for(int i = 0; i < _lenght; i++)
    {
       snake->body[i].x = currPoint;
       snake->body[i].y = 4;
       currPoint--;
    }
}

void NOS_Snake_Game_Draw(WS2812B_Matrix* matrix, NOS_Snake_Game* snake)
{
    Point currDrawPoint = {0};

    //Initialize
    currDrawPoint.x = snake->head.x;
    currDrawPoint.y = snake->head.y;

    //food Draw
    NOS_WS2812B_Matrix_SetPixel(matrix,snake->foodColor,Test_GetPixelPos(snake->food.x,snake->food.y));

    //Head Draw
    NOS_WS2812B_Matrix_SetPixel(matrix,snake->headColor,Test_GetPixelPos(NOS_Math_NormalizeValue(currDrawPoint.x,matrix->size->col),NOS_Math_NormalizeValue(currDrawPoint.y,matrix->size->row)));

    //Body Draw
    for(int i = 0; i < snake->lenght; i++)
    {
        NOS_WS2812B_Matrix_SetPixel(matrix,snake->bodyColor,Test_GetPixelPos(NOS_Math_NormalizeValue(snake->body[i].x,matrix->size->col),NOS_Math_NormalizeValue(snake->body[i].y,matrix->size->row)));
    }

     NOS_WS2812B_Matrix_SetPixel(matrix,snake->headColor,Test_GetPixelPos(NOS_Math_NormalizeValue(currDrawPoint.x,matrix->size->col),NOS_Math_NormalizeValue(currDrawPoint.y,matrix->size->row)));
}

void NOS_Snake_Game_Move(NOS_Snake_Game* snake,MoveDirection direct)
{
    Point prevHeadPosition = snake->head;

    switch (direct)
    {
    case RIGHT:
        if(snake->direction != LEFT)
        {
            if(snake->head.x > 0)
            {
                snake->head.x--;
            }
            else
            {
                snake->head.x = 32;
            }
        }
        break;
    case LEFT:
        if(snake->direction != RIGHT)
        {
            if(snake->head.x < 32)
            {
                snake->head.x++;
            }
            else
            {
                snake->head.x = 0;
            }
        }
        break;
    case UP:
        if(snake->direction != DOWN)
        {
            if(snake->head.y < 8)
            {
                snake->head.y++;
            }
            else
            {
                snake->head.y = 0;
            }
        }
        break;
    case DOWN:
        if(snake->direction != UP)
        {

            if(snake->head.y > 0 && snake->head.y < 8)
            {
                snake->head.y--;
            }
            else
            {
                snake->head.y = 8;
            }
        }
        break;
    default:
        break;
    }



    if(snake->head.x < 0)
    {
        snake->head.x = 32 + snake->head.x;
    }

        if(snake->head.y > 8)
    {
        snake->head.y = 8 + snake->head.y;
    }

    Point buffer[snake->lenght];
    
    for(int i = 0; i < snake->lenght; i++)
    {
        buffer[i] = snake->body[i];
    }

    for(int i = 1; i < snake->lenght; i++)
    {
        snake->body[i] = buffer[i - 1]; 
    }
    snake->body[0] = prevHeadPosition;
    snake->direction = direct;
}

void NOS_Snake_Game_IncreaseLenght(NOS_Snake_Game* snake)
{
    
    Point buffer[snake->lenght];
    
    for(int i = 0; i < snake->lenght; i++)
    {
        buffer[i] = snake->body[i];
    }

    snake->body[0] = snake->head;
    
    for(int i = 1; i < snake->lenght + 1; i++)
    {
        snake->body[i] = buffer[i - 1];
    }

    switch (snake->direction)
    {
    case RIGHT:
        snake->head.x--;

        break;
    case LEFT:
        snake->head.x++;

        break;
    case UP:
        snake->head.y++;
        

        
        break;
    case DOWN:
        snake->head.y--;
        

        
        break;
    default:
        break;
    }


    snake->lenght++;

    if(snake->lenght > MAX_LENGHT)
    {
        snake->lenght = 4;
    }
/*
    if(snake->lenght>6)
    {
        snake->lenght = 3;
    }
*/
}

void NOS_Snake_GenerateFood(NOS_Snake_Game* snake,WS2812B_Matrix* matrix)
{
    NOS_WS2812B_Matrix_SetPixel(matrix,snake->foneColor,Test_GetPixelPos(snake->food.x,snake->food.y));

    Point foodPoint = {1000,1000};

    while(foodPoint.x == 1000 && foodPoint.y == 1000)
    {
        bool normVal = false;
        uint8_t _x = NOS_Math_NormalizeValue((uint16_t)rand(),30);
        uint8_t _y = NOS_Math_NormalizeValue((uint16_t)rand(),6);

        if(snake->head.x == _x && snake->head.y == _y)
        {
            normVal = false;
        }

        for(int i = 0; i < snake->lenght; i++)
        {
            if(snake->body[i].x == _x && snake->body[i].y == _y)
            {
                normVal = false;
                break;
            }
            normVal = true;
        }

        if(normVal)
        {
            foodPoint.x = _x; 
            foodPoint.y = _y;
        }

    }

    snake->food = foodPoint;
    NOS_WS2812B_Matrix_SetPixel(matrix,snake->foodColor,Test_GetPixelPos(foodPoint.x,foodPoint.y));

}

void NOS_Snake_CheckCollisionWfood(NOS_Snake_Game* snake,WS2812B_Matrix* matrix)
{
    if(snake->head.x == snake->food.x && snake->head.y == snake->food.y)
    {
        NOS_Snake_Game_IncreaseLenght(snake);
        NOS_Snake_GenerateFood(snake,matrix);
    }
}

bool NOS_Snake_CheckSelfCollision(NOS_Snake_Game* snake)
{
    for(int i = 0; i < snake->lenght; i++)
    {
        if(snake->head.x == snake->body[i].x && snake->head.y == snake->body[i].y)
        {
            return true;
        }
    }
    return false;
}



void NOS_Snake_AI_Move(NOS_Snake_Game* snake)
{
    int dx = snake->food.x - snake->head.x;
    int dy = snake->food.y - snake->head.y;

    if(dx != 0)
    {
        if(dx > 0)
        {
            NOS_Snake_Game_Move(snake,LEFT);
            if(NOS_Snake_CheckSelfCollision(snake))
            {

            }
            return;
        }
        else
        {
            NOS_Snake_Game_Move(snake,RIGHT);
            return;
        }
    }

    if(dy != 0)
    {
        if(dy > 0)
        {
            NOS_Snake_Game_Move(snake,UP);
            return;
        }
        else
        {
            NOS_Snake_Game_Move(snake,DOWN);
            return;
        }
    }

}

void NOS_Snake_ChangeByUART(NOS_Snake_Game* snake,uint8_t input)
{
    switch(input)
    {
        case 100:
            NOS_Snake_Game_Move(&snake,UP);
        break;

        case 101:
            NOS_Snake_Game_Move(&snake,DOWN);
        break;

        case 102:
            NOS_Snake_Game_Move(&snake,LEFT);
        break;

        case 103:
            NOS_Snake_Game_Move(&snake,RIGHT);
        break;
    }
}