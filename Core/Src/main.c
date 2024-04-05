/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <../Src/NOS_LIB_Driver/Inc/NOS_Lib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart2;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
extern uint8_t frameBuffer1[3 * 256];
extern uint8_t frameBuffer2[3 * 8 * 64];
extern uint8_t frameBuffer3[3 * 8 * 32];

NOS_TimeEvent tetrisUpdateEvent = {0};
NOS_TimeEvent screenUpdateEvent = {0};
NOS_TimeEvent uartTestEvent = {0};

WS2812B_Strip stripA = {0};
PixelColor pixelsA[128];

const PixelColor red = {255,0,0};
const PixelColor green = {0,255,0};
const PixelColor blue = {0,0,255};
const PixelColor white = {255,255,255};
const PixelColor yellow = {255,255,0};
const PixelColor purple = {255,0,255};
const PixelColor cyan = {0,255,255};

PixelColor colors[7] = {red,green,blue,white,yellow,purple,cyan};


uint8_t rx_buff[256];
uint8_t fuckBuff[1024];
uint16_t fuckIndex = 0;
bool rx_flag = false;
bool tx_flag = false;
int currMessageLenght = 0;
int expectedMessageLenght = 0;
bool startReceive = false;
bool endReceive = false;
int receiveTime = 0;

SinValue bright = {0};

NOS_Button button = {0};

GPIO_PIN PA6 = {0};
GPIO_PIN PA7 = {0};


int uartPixelCount = 0;
bool bufferCommand = false;

int currColor = 0;
int buttonDelay = 0;

int receiveTimeAbort = 100;
bool receiveAbort = false;
bool receiveCheck = false;

NOS_Short value;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM6_Init(void);
static void MX_SPI1_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void ReceiveReset(uint8_t* rx_buff_ptr)
{
  rx_buff_ptr = rx_buff;
  currMessageLenght = 0;
  rx_flag = true;
}

void ContinueReceive(uint8_t* rx_buff_ptr)
{
  rx_buff[currMessageLenght] = *rx_buff_ptr;
  ++rx_buff_ptr;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {

static uint8_t* rx_buff_ptr = rx_buff;

fuckBuff[fuckIndex] = *rx_buff_ptr;
fuckIndex++;

if(*rx_buff_ptr == UART_ADDRESS && !startReceive)
{
  startReceive = true;
  currMessageLenght = 0;

  fuckIndex = 1;
  for(int i = 0; i < 1024; i++)
  {
    fuckBuff[i] = 0;
  }
  fuckBuff[0] = *rx_buff_ptr; 
}

if(startReceive && currMessageLenght == 1 && !bufferCommand)
{
  value.bytes[1] = *rx_buff_ptr;
}

if(startReceive && currMessageLenght == 2 && !bufferCommand)
{
  value.bytes[0] = *rx_buff_ptr;
  expectedMessageLenght = value.data;
  bufferCommand = true;
}

if(fuckIndex > 1023)
{
  fuckIndex = 0;
}

currMessageLenght++;

if(startReceive && (currMessageLenght == expectedMessageLenght) && bufferCommand)
{
  ReceiveReset(rx_buff_ptr);
  bufferCommand = false;
  startReceive = false;
}
else
{
  ContinueReceive(rx_buff_ptr);
}

    HAL_UART_Receive_IT (&huart2, rx_buff_ptr, 1); 
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  //MX_SPI1_Init();
  //MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2,rx_buff,1);
  HAL_UART_Transmit(&huart2,"HELLO",sizeof("HELLO"),1000);
  visInit();

  NOS_WS2812B_Strip_FullInit(&stripA,&frameBuffer1,&pixelsA,128);

  NOS_TimeEvent_Init(&tetrisUpdateEvent, 50);
  NOS_TimeEvent_Init(&screenUpdateEvent, 20);
  NOS_TimeEvent_Init(&uartTestEvent, 1000);
  //NOS_WS2812B_Strip_TestFill(&stripA);

  NOS_Math_SinValue_Init(&bright,65,75,1);

  NOS_Button_Init(&button);

  NOS_GPIO_PinInit(&PA6,GPIOA,GPIO_PIN_6,0);
  NOS_GPIO_PinInit(&PA7,GPIOA,GPIO_PIN_7,0);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    if(rx_flag)
    {
      char mess[] = "Recieved!";
      uint32_t buff = 0; 

      if(NOS_Strip_Uart_ParseCommand(&stripA,fuckBuff) == "ready!")
      {
        HAL_UART_Transmit(&huart2,"ready!",7,1000);
      }
      
      rx_flag = false;
    }

    if (NOS_TimeEvent_Check(&tetrisUpdateEvent))
    { 
      NOS_Math_SinValue_Handler(&bright);
      NOS_TimeEvent_FinishEvent(&tetrisUpdateEvent);
    }
    
    if (NOS_TimeEvent_Check(&screenUpdateEvent))
    {
      //stripA.bright = NOS_Math_GetSinValue(&bright);
      NOS_WS2812B_Strip_Update(&stripA);
      visHandle();

      NOS_TimeEvent_FinishEvent(&screenUpdateEvent);
    }

    if(NOS_Button_CheckPressDone(&button))
    {
      if(currColor < 6)
      {
        currColor++;
      }
      else
      {
        currColor = 0;
      }

      NOS_WS2812B_Strip_ColorFill(&stripA,colors[currColor]);
      NOS_Button_PressDoneHandler(&button);
      HAL_GPIO_TogglePin(PA6.Port,PA6.Pin);
    }

    if(NOS_Button_isPressed(&button))
    {
      HAL_GPIO_WritePin(PA7.Port,PA7.Pin,0);
    }
    else
    {
      HAL_GPIO_WritePin(PA7.Port,PA7.Pin,1);
    }



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 0;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 65535;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PE3 PE4 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  NOS_TimeEvent_TickHandler(&tetrisUpdateEvent);
  NOS_TimeEvent_TickHandler(&screenUpdateEvent);
  NOS_TimeEvent_TickHandler(&uartTestEvent);
  buttonDelay++;
  if(buttonDelay > 200)
  {
      NOS_Button_TimerHandler(&button);
      buttonDelay = 0;
  }

  receiveTime++;
  /* USER CODE END SysTick_IRQn 1 */
}

void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI3_IRQn 1 */


  /* USER CODE END EXTI3_IRQn 1 */
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
