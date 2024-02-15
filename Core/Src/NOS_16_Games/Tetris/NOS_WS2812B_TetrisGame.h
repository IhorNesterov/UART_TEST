#include "NOS_Includes.h"
#include "stdlib.h"
#include "stdint.h"
//#include "NOS_BaseColors.h"

typedef enum figuresNames_t{figure_I,figure_O,figure_T,figure_S,figure_Z,figure_J,figure_L} figuresNames;

#define mapSize_X 8
#define mapSize_Y 32

typedef struct NOS_Tetris_Game_t
{
    figuresNames currFigure;
    figuresNames prevFigure;

    MoveDirection direction;

    PixelColor map[mapSize_Y][mapSize_X];
    PixelColor currFigureBitMap[4][4];
    PixelColor currFigureColor;
    Point referencePoint;

}NOS_Tetris_Game;

figuresNames GetRandomFigure(NOS_Tetris_Game* game);

void NOS_Tetris_Game_Init(NOS_Tetris_Game* game);

void NOS_Tetris_Game_DrawFigure(NOS_Tetris_Game* game);

void NOS_Tetris_Game_CheckForBottomCollision(NOS_Tetris_Game* game);

void NOS_Tetris_Game_TestDraw(NOS_Tetris_Game* game,figuresNames name);

void NOS_Tetris_Game_DoNextStep(NOS_Tetris_Game* game);

void NOS_Tetris_Game_Draw(WS2812B_Matrix* matrix,NOS_Tetris_Game* game);