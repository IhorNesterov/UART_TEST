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
extern uint8_t frameBuffer2[3 * 256];
extern uint8_t frameBuffer3[3 * 256];

NOS_TimeEvent tetrisUpdateEvent = {0};
NOS_TimeEvent screenUpdateEvent = {0};
NOS_TimeEvent uartTestEvent = {0};

WS2812B_Strip stripA = {0};
WS2812B_Strip stripB = {0};
WS2812B_Strip stripC = {0};

PixelColor pixelsA[128];
PixelColor pixelsB[128];
PixelColor pixelsC[128];


NOS_UART_Struct UART2;

int receiveTime = 0;

SinValue bright = {0};

NOS_Button button = {0};

GPIO_PIN PA6 = {0};
GPIO_PIN PA7 = {0};
GPIO_PIN PE3 = {0};
GPIO_PIN PE4 = {0};

int uartPixelCount = 0;

int currColor = 0;
int buttonDelay = 0;

Effect_Struct breatheA = {0};

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
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    NOS_UART_ReceiveHandler(&UART2,huart);
}

  bool tick = false;
/* USER CODE END 0 */
      PixelColor red = {0xFF,0x00,0x00};
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
  //MX_TIM6_Init();
  //MX_SPI1_Init();
  //MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2,UART2.rx_buff,1);
  HAL_UART_Transmit(&huart2,"HELLO",sizeof("HELLO"),1000);
  visInit();

  NOS_WS2812B_Strip_FullInit(&stripA,&frameBuffer1,&pixelsA,128);
    NOS_WS2812B_Strip_FullInit(&stripB,&frameBuffer2,&pixelsB,48);
    NOS_WS2812B_Strip_FullInit(&stripC,&frameBuffer3,&pixelsC,48);

  NOS_TimeEvent_Init(&tetrisUpdateEvent, 50);
  NOS_TimeEvent_Init(&screenUpdateEvent, 20);
  NOS_TimeEvent_Init(&uartTestEvent, 1000);
  //NOS_WS2812B_Strip_TestFill(&stripA);

  NOS_Math_SinValue_Init(&bright,65,75,1);

  NOS_GPIO_PinInit(&PE3,GPIOE,GPIO_PIN_3,Input);
  NOS_GPIO_PinInit(&PE4,GPIOE,GPIO_PIN_4,Input);

  NOS_Button_Init(&button,GPIO_PIN_RESET);

  NOS_GPIO_PinInit(&PA6,GPIOA,GPIO_PIN_6,Output);
  NOS_GPIO_PinInit(&PA7,GPIOA,GPIO_PIN_7,Output);

  NOS_UART_ReceiveAbort(&UART2);

  //NOS_WS2812B_Strip_ColorFill(&stripA,red);

  //addr count1 count0 0x50 speed1 speed0 step1 step0 min1 min0 max1 max0 
  NOS_WS2812B_Strip_Effect_Rainbow_Init(&breatheA,20,5,0,1524);
  NOS_WS2812B_Strip_Effects_AddEffect(&stripA,breatheA);
  NOS_WS2812B_Strip_Effects_AddEffect(&stripB,breatheA);
  NOS_WS2812B_Strip_Effects_AddEffect(&stripC,breatheA);

  NOS_WS2812B_Strip_Update(&stripA);
  NOS_WS2812B_Strip_Update(&stripB);
  NOS_WS2812B_Strip_Update(&stripC);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if(tick)
    {
          NOS_UART_Timer_Handler(&UART2);
          NOS_TimeEvent_TickHandler(&tetrisUpdateEvent);
          NOS_TimeEvent_TickHandler(&screenUpdateEvent);
          //NOS_Button_TimerHandler(&button);
          NOS_WS2812B_Strip_Effects_Handler(&stripA);
          NOS_WS2812B_Strip_Effects_Handler(&stripB);
          NOS_WS2812B_Strip_Effects_Handler(&stripC);
          tick = false;
    }

    if(NOS_UART_CheckReceive(&UART2))
    {
      
      switch (NOS_UART_GetReceivedData(&UART2)[0])
      {
        case 0x64:
          NOS_Strip_Uart_ParseCommand(&stripA,NOS_UART_GetReceivedData(&UART2));
          NOS_Strip_Uart_ParseCommand(&stripB,NOS_UART_GetReceivedData(&UART2));
          NOS_Strip_Uart_ParseCommand(&stripC,NOS_UART_GetReceivedData(&UART2));

          break;

      case 0x65:
              if(NOS_Strip_Uart_ParseCommand(&stripA,NOS_UART_GetReceivedData(&UART2)) == "ready!")
              {
                  HAL_UART_Transmit(&huart2,"ready!",7,1000);
              }
        break;

      case 0x66:
              if(NOS_Strip_Uart_ParseCommand(&stripB,NOS_UART_GetReceivedData(&UART2)) == "ready!")
              {
                  //HAL_UART_Transmit(&huart2,"ready!",7,1000);
              }
              break;

      case 0x67:
              if(NOS_Strip_Uart_ParseCommand(&stripC,NOS_UART_GetReceivedData(&UART2)) == "ready!")
              {
                 //HAL_UART_Transmit(&huart2,"ready!",7,1000);
              }
      break;


      default:
        break;
      }

      NOS_UART_EndReceive(&UART2);
      HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_7);
      //HAL_UART_Transmit(&huart2,"ready!",7,1000);
      NOS_WS2812B_Strip_Update(&stripA);
      NOS_WS2812B_Strip_Update(&stripB);
      NOS_WS2812B_Strip_Update(&stripC);
      visHandle();
    }

    if (NOS_TimeEvent_Check(&tetrisUpdateEvent))
    { 
      //NOS_Math_SinValue_Handler(&bright);

  
      NOS_TimeEvent_FinishEvent(&tetrisUpdateEvent);
    }
    
    if (NOS_TimeEvent_Check(&screenUpdateEvent))
    {
      //NOS_WS2812B_Strip_ColorFill(&stripA,red);
      //stripA.bright = NOS_Math_GetSinValue(&bright);
      NOS_WS2812B_Strip_Update(&stripA);
      NOS_WS2812B_Strip_Update(&stripB);
      NOS_WS2812B_Strip_Update(&stripC);
      visHandle();
      NOS_TimeEvent_FinishEvent(&screenUpdateEvent);
    }

    if(NOS_Button_CheckPressDone(&button))
    {
      if(currColor < 11)
      {
        currColor++;
      }
      else
      {
        currColor = 0;
      }
      NOS_WS2812B_Strip_ColorFill(&stripA,NOS_GetBaseColor(currColor));
      NOS_Button_PressDoneHandler(&button);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
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
tick = true;
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
