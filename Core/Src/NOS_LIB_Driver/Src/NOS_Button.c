#include "NOS_Button.h"


void NOS_Button_Init(NOS_Button* button,GPIO_PinState openLvl)
{
    button->pressed = false;
    button->released = false;
    button->pressDone = false;
    button->openLevel = openLvl;

    button->timer = 0;
    button->pressedTime = 0;
    button->releasedTime = 0;
    button->lastDoneTime = 0;
}

void NOS_Button_TimerHandler(NOS_Button* button)
{
    button->timer++;
    
    if(button->stopped)
    {
        if(button->timer > button->stopTime)
        {
            button->stopped = false;
            button->timer = 0;
        }
    }
    else
    {
        if(HAL_GPIO_ReadPin(button->pin->Port,button->pin->Pin) == GPIO_PIN_RESET && !button->pressed)
        {
            NOS_Button_PressedHandler(button);
        }

        if(HAL_GPIO_ReadPin(button->pin->Port,button->pin->Pin) == GPIO_PIN_SET && button->pressed)
        {
            NOS_Button_ReleaseHandler(button);
        }
    }
}

void NOS_Button_TimerReset(NOS_Button* button)
{
    button->timer = 0;
}

void NOS_Button_PressedHandler(NOS_Button* button)
{
    button->pressed = true;
    button->pressedTime = button->timer;
}

void NOS_Button_ReleaseHandler(NOS_Button* button)
{
    if(button->pressed)
    {
        button->releasedTime = button->timer;
        button->lastDoneTime = button->releasedTime - button->pressedTime;
        button->pressDone = true;
        button->pressed = false;
        button->released = true;
    }
}

bool NOS_Button_CheckPressDone(NOS_Button* button)
{
    return button->pressDone;
}

bool NOS_Button_isPressed(NOS_Button* button)
{
    return button->pressed;
}

void NOS_Button_PressDoneHandler(NOS_Button* button)
{
    button->pressed = false;
    button->released = false;
    button->pressDone = false;
    button->stopped = true;

    button->timer = 0;
    button->pressedTime = 0;
    button->releasedTime = 0;
    button->lastDoneTime = 0;
}

