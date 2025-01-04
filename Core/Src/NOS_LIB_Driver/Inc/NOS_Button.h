#ifndef NOS_BUTTON
#define NOS_BUTTON

//#include "NOS_Includes.h"
#include "NOS_Typedefs.h"
#include "NOS_GPIO.h"

void NOS_Button_Init(NOS_Button* button,GPIO_PinState openLvl);

void NOS_Button_TimerHandler(NOS_Button* button);
void NOS_Button_TimerReset(NOS_Button* button);

void NOS_Button_PressedHandler(NOS_Button* button);
void NOS_Button_ReleaseHandler(NOS_Button* button);
void NOS_Button_PressDoneHandler(NOS_Button* button);

bool NOS_Button_CheckPressDone(NOS_Button* button);
bool NOS_Button_isPressed(NOS_Button* button);


#endif
