
#ifndef NOS_TYPEDEFS
#define NOS_TYPEDEFS

#include "stdint.h"
#include "stm32f4xx.h"
//#include "stm32f1xx.h"
#include "stdbool.h"
/* Unions begin */

typedef union Short_t //uint16_t data wrap
{ 
    uint8_t bytes[2];
    uint16_t data;
} NOS_Short;

typedef union Float_t //float data wrap
{
    uint8_t bytes[4];
    float data;
} NOS_Float;

typedef union Long_t
{
  uint8_t bytes[4];
  uint32_t data;
} NOS_Long;

/* Unions end */

/* Structures begin */

/*WS2812B Matrix*/
typedef struct Point_t
{
  uint16_t x;
  uint16_t y;
}Point;


typedef struct PixelColor_t
{
   uint8_t R;
   uint8_t G;
   uint8_t B;
} PixelColor;

typedef struct MatrixSize_t
{
  uint8_t col;
  uint8_t row;
} MatrixSize;


typedef struct IdealSymbol_t
{
  uint8_t* data;
  uint8_t lenght;
} IdealSymbol;



typedef struct  Symvol_t
{
  uint8_t data[5];
} Symvol;


typedef struct WS2812B_Matrix_t
{
    uint8_t* buffer;
    uint8_t bright;
    NOS_Short ledsCount;
    MatrixSize* size;
    PixelColor* textColor;
    PixelColor* foneColor;
    Symvol* symvols;
} WS2812B_Matrix;



/*WS2812B Matrix*/

typedef enum TimeFormat_e{Hour24,Hour12} TimeFormat;

typedef struct RealTime_t
{
  uint32_t milliseconds;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  TimeFormat format;
} RealTime;

/// @brief 
typedef struct NOS_TimeEvent_t
{
  uint32_t ticksToEnable;
  bool stopped;
  bool enabled;
  uint32_t ticks;
} NOS_TimeEvent;

typedef struct URE_Detector_t
{
  uint8_t Address;
  NOS_Float value;
  NOS_Float first_danger;
  NOS_Float second_danger;
  uint16_t voltage;
  NOS_Short temperature;
}URE_Detector;

typedef struct URE_TITABLE_t
{
  uint8_t Address;
  uint8_t detector_Address;
 // Language language;
  PixelColor valueColor;
  PixelColor timeColor;
  PixelColor tempColor;
  RealTime time;

}URE_Table;

/*ModBus*/
typedef enum ModBus_State_t{Free,ReceiveFromMaster,ReceiveFromSlave,TransmitToMaster,TransmitToSlave} ModBus_State;

typedef struct ModBus_Master_Command_t
{
    uint8_t Addr;
    uint8_t Command;
    uint8_t Byte_Count;
    NOS_Short Reg_Addr;
    NOS_Short Reg_Count;
    NOS_Short ShortValue;
    NOS_Float FloatValue;
} ModBus_Master_Command;

typedef struct ModBus_Slave_Command_t
{
    uint8_t type;
    uint8_t Addr;
    uint8_t Command;
    uint8_t Byte_Count;
    uint16_t RegAddr;
    NOS_Short ShortValue;
    NOS_Float FloatValue;
} ModBus_Slave_Command;

typedef struct NOS_Modbus_Device_t
{
  uint8_t Descriptor;
  uint8_t Addr;
  uint8_t RegisterCount;
  NOS_Short* RegisterAddress;
} Modbus_Device;

typedef struct GPIO_PIN_t
{
  GPIO_TypeDef* Port;
  uint16_t Pin;
} GPIO_PIN;

typedef struct NOS_ModBus_Struct_t
{
  Modbus_Device devices[10];
  ModBus_Master_Command master;
  ModBus_Slave_Command slave;
  //UART_HandleTypeDef* huart;
  uint8_t* rx_buff;
  GPIO_PIN RW;
  ModBus_State state;
  uint8_t devicesCount;
  uint8_t messageLenght;
  uint8_t currCommand;
  uint8_t currDevice;
  uint8_t expectedMessageLenght;
  bool addressOk;
  bool rx_flag;
  bool tx_flag;
} ModBus_Struct;

/*ModBus*/
//typedef enum TemperatureFormat_t{Celcium,Farengheit} TempFormat;
typedef enum Language_e{English,Ukrainian} Language;
typedef enum MoveDirection_e {UP,DOWN,RIGHT,LEFT} MoveDirection;

typedef enum Signal_Level_e {LOW,HIGH} Signal_Level;
typedef enum Button_State_e
{
    Idle,pressed,released,pressDone
}Button_State;

typedef struct NOS_Button_t
{
    bool pressed;
    bool released;
    bool pressDone;
    bool stopped;
    GPIO_PinState openLevel;

    uint32_t timer;
    uint32_t pressedTime;
    uint32_t releasedTime;
    uint32_t lastDoneTime;
    uint32_t stopTime;

    GPIO_PIN* pin;

}NOS_Button;

typedef struct SinValue_t
{
    int value;

    int minValue;
    int maxValue;
    int increment;
    bool rise;
    bool fall;
}SinValue;

typedef struct Effect_Struct_t
{
  SinValue value;
  NOS_Short speed;
  NOS_Short step;
  uint16_t timer;

  uint16_t minValue;
  uint16_t maxValue;

  bool enabled;
  uint8_t effectId;
}Effect_Struct;




/**
  * @brief  Strip structure.
  * Warning : No Base init avalaible
  * @param frameBuffer pointer on core frame buffer in WS2812B_Core 
  * @param pixels pointer on pixelBuffer
  * @param pixelCount int count of Leds in strip
  * @param bright int (0-100) brightness of led
  * @retval void,in future will be NOS_System_State
  */
typedef struct NOS_WS2812B_STRIP_t
{
    uint8_t* frameBuffer;
    PixelColor* pixels;
    int pixelCount;
    int bright;

    Effect_Struct effects[10];
    uint8_t effectsCounter;
}WS2812B_Strip;

/* STM32 Core structures begin*/



/* STM32 Core structures end*/

/* Structures end */
#endif