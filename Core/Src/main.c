/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define GYRO_DOWN_THRESHOLD   -7000
#define GYRO_UP_THRESHOLD      7000

#define ACCY_STAND_THRESHOLD  11000
#define ACCZ_BOTTOM_THRESHOLD 12000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef enum
{
    STAND,
    GOING_DOWN,
    BOTTOM,
    GOING_UP
} SquatState_t;

SquatState_t squatState = STAND;

uint16_t reps = 0;
char str[32];
uint8_t data[14];
int16_t AccX;
int16_t AccY;
int16_t AccZ;

int16_t GyroX;
int16_t GyroY;
int16_t GyroZ;

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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  ssd1306_Init();

  ssd1306_Fill(Black);
  ssd1306_UpdateScreen();
  HAL_I2C_IsDeviceReady(&hi2c1, 0x68 << 1, 1, 100);

  uint8_t data_init;

  data_init = 0;
  HAL_I2C_Mem_Write(
      &hi2c1,
      0x68 << 1,
      0x6B,
      1,
      &data_init,
      1,
      100);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	HAL_I2C_Mem_Read(
	      &hi2c1,
	      0x68 << 1,
	      0x3B,
	      1,
	      data,
	      14,
	      100);

//	AccX  = (int16_t)(data[0]  << 8 | data[1]);
	AccY  = (int16_t)(data[2]  << 8 | data[3]);
	AccZ  = (int16_t)(data[4]  << 8 | data[5]);
	GyroX = (int16_t)(data[8]  << 8 | data[9]);
//	GyroY = (int16_t)(data[10] << 8 | data[11]);
//	GyroZ = (int16_t)(data[12] << 8 | data[13]);

	switch(squatState)
	{
	    case STAND:

	        if((GyroX < GYRO_DOWN_THRESHOLD) &&
	           (AccY > ACCY_STAND_THRESHOLD))
	        {
	            squatState = GOING_DOWN;
	        }

	        break;


	    case GOING_DOWN:

	        if((AccZ > ACCZ_BOTTOM_THRESHOLD) &&
	           (AccY < ACCY_STAND_THRESHOLD))
	        {
	            squatState = BOTTOM;
	            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	        }

	        break;


	    case BOTTOM:

	        if(GyroX > GYRO_UP_THRESHOLD)
	        {
	            squatState = GOING_UP;
	        }

	        break;


	    case GOING_UP:

	        if(AccY > ACCY_STAND_THRESHOLD)
	        {
	            reps++;
	            squatState = STAND;
	            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	        }

	        break;
	}

	char* stateText;

	switch(squatState)
	{
	    case STAND:
	        stateText = "STAND";
	        break;

	    case GOING_DOWN:
	        stateText = "DOWN";
	        break;

	    case BOTTOM:
	        stateText = "BOTTOM";
	        break;

	    case GOING_UP:
	        stateText = "UP";
	        break;
	}
	ssd1306_Fill(Black);

	sprintf(str, "R:%u", reps);
	ssd1306_SetCursor(0,0);
	ssd1306_WriteString(str, Font_16x26, White);

	ssd1306_SetCursor(0,32);
	ssd1306_WriteString(stateText, Font_7x10, White);

//	sprintf(str, "GYX:%d", GyroX);
//	ssd1306_SetCursor(0,32);
//	ssd1306_WriteString(str, Font_7x10, White);

	sprintf(str, "AY:%d", AccY);
	ssd1306_SetCursor(0,48);
	ssd1306_WriteString(str, Font_7x10, White);


//	sprintf(str, "GX:%d", GyroX);
//	ssd1306_SetCursor(0,0);
//	ssd1306_WriteString(str, Font_7x10, White);
//
//	sprintf(str, "AY:%d", AccY);
//	ssd1306_SetCursor(0,16);
//	ssd1306_WriteString(str, Font_7x10, White);
//
//	sprintf(str, "AZ:%d", AccZ);
//	ssd1306_SetCursor(0,32);
//	ssd1306_WriteString(str, Font_7x10, White);

	ssd1306_UpdateScreen();

	HAL_Delay(25);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
#ifdef USE_FULL_ASSERT
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
