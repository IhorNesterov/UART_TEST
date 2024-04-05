#ifndef NOS_DELEGATE
#define NOS_DELEGATE

#include "NOS_Includes.h"

typedef struct NOS_Delegate_t
{
    void (*handler)(void* object,void* eventArgs);


}NOS_Delegate;

void NOS_Delegate_Init(NOS_Delegate* delegate,void (*func)(void* object,void* eventArgs))
{
    delegate->handler = func;
}

#endif