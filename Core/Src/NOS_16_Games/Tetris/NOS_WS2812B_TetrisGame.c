#include "NOS_WS2812B_TetrisGame.h"

uint8_t figureI_Sprite[2] = {0b00001111,0x0};
uint8_t figureO_Sprite[2] = {0b00000110,0b00000110};
uint8_t figureT_Sprite[2] = {0b00000111,0b00000010};
uint8_t figureS_Sprite[2] = {0b00000110,0b00000011};
uint8_t figureZ_Sprite[2] = {0b00000011,0b00000110};
uint8_t figureJ_Sprite[2] = {0b00000111,0b00000100};
uint8_t figureL_Sprite[2] = {0b00000111,0b00000001};

figuresNames GetRandomFigure(NOS_Tetris_Game* game)
{
    int rnd = -1;

    while(rnd < 0 || rnd > 6)
    {
        rnd = 3;
        
        if(rnd == game->prevFigure)
        {
            rnd = -2;
        }
    }

    game->prevFigure = game->currFigure;
    game->currFigure = rnd;

    return game->currFigure;
}

void NOS_Tetris_Game_Init(NOS_Tetris_Game* game)
{
    game->direction = UP;
    game->referencePoint.x = 4;
    GetRandomFigure(game);

    for(int y = 0; y < mapSize_Y; y++)
    {
        for(int x = 0; x < mapSize_X; x++)
        {
            NOS_WS2812B_SetPixelColor(&game->map[y][x],0,0,0);
        }
    }

    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            NOS_WS2812B_SetPixelColor(&game->currFigureBitMap[y][x],0,0,0);
        }
    }
}

void NOS_Tetris_Game_DrawFigure(NOS_Tetris_Game* game)
{
    
}

void NOS_Tetris_Game_CheckForBottomCollision(NOS_Tetris_Game* game)
{

}

void NOS_Tetris_Game_TestDraw(NOS_Tetris_Game* game,figuresNames name)
{
    uint8_t currSprite[2] = {0x0,0x0};
    uint8_t currByte = 0x0;
    bool currBit = false;

    for(int y = 0; y < mapSize_Y; y++)
    {
        for(int x = 0; x < mapSize_X; x++)
        {
            NOS_WS2812B_SetPixelColor(&game->map[y][x],0,0,0);
        }
    }

    switch (name)
    {
        case figure_I:
            currSprite[0] = figureI_Sprite[0];
            currSprite[1] = figureI_Sprite[1];
        break;

        case figure_J:
            currSprite[0] = figureJ_Sprite[0];
            currSprite[1] = figureJ_Sprite[1];
        break;

        case figure_L:
            currSprite[0] = figureL_Sprite[0];
            currSprite[1] = figureL_Sprite[1];
        break;

        case figure_O:
            currSprite[0] = figureO_Sprite[0];
            currSprite[1] = figureO_Sprite[1];
        break;

        case figure_S:
            currSprite[0] = figureS_Sprite[0];
            currSprite[1] = figureS_Sprite[1];
        break;

        case figure_T:
            currSprite[0] = figureT_Sprite[0];
            currSprite[1] = figureT_Sprite[1];
        break;

        case figure_Z:
            currSprite[0] = figureZ_Sprite[0];
            currSprite[1] = figureZ_Sprite[1];
        break;

        default:
            //NOS_Error_Handler(&system,"Not valid case name for sprite");
        break;
    }


    switch (game->direction)
    {
        case UP:
            for(int x = 0; x < 2; x++)
            {
                currByte = currSprite[x];

                for(int y = 0; y < 4; y++)
                {
                    currBit = ((1 << currBit) & currByte);

                    if(currBit)
                    {
                        NOS_WS2812B_SetPixelColor(&game->map[game->referencePoint.y + y][game->referencePoint.x + x],0xFF,0,0);
                    }
                    else
                    {
                        NOS_WS2812B_SetPixelColor(&game->map[game->referencePoint.y + y][game->referencePoint.x + x],0,0,0);
                    }
                }
            }
            break;
        
        default:
            break;
    }

}

void NOS_Tetris_Game_DoNextStep(NOS_Tetris_Game* game)
{
    game->referencePoint.y--;
    if(game->referencePoint.y < 1 || game->referencePoint.y > 28)
    {
        game->referencePoint.y = 28;
    }
}

void NOS_Tetris_Game_Draw(WS2812B_Matrix* matrix,NOS_Tetris_Game* game)
{
    PixelColor curr = {0,0,0};
    uint8_t x,y = 1;

    for(y = 1; y < mapSize_Y; y++)
    {
        for(x = 1; x < mapSize_X; x++)
        {
            curr = game->map[y - 1][x - 1];
            NOS_WS2812B_Matrix_SetPixel(matrix,&curr,Test_GetPixelPos(y,x));
        }
    }
}
