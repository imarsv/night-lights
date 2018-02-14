/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include <string.h>
#include <bitmap/bitmap.h>
#include <stdio.h>
#include <stdlib.h>
#include "effects/rainbow.h"
#include "effects/running_lights.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void clear(uint8_t *data);
void set(uint8_t *data, uint16_t index, RGB_t *color);
void paint(uint8_t *pData, uint16_t size);
void paintRGB(RGB_t *pData, uint16_t size);
void paintHSV(HSV_t *pData, uint16_t size);
uint32_t bswap_32(uint32_t x);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

#define LED_COUNT (120)
#define LED_DATA_SIZE (LED_COUNT * 9)

HSV_t leds[LED_COUNT];
uint8_t data[LED_DATA_SIZE /*+ (9 * 1)*/ + 4];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  unsigned int effect = 0;

  memset(leds, 0, sizeof(HSV_t) * LED_COUNT);

  union Effect {
	  RainbowCtx rainbow;
	  RunningLightsCtx runningLights;
  };

  union Effect context = {0};

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  srand(HAL_GetTick());

	  unsigned int nextEffect = rand() % 5;
	  if (effect != nextEffect) {
		  memset(&context, 0, sizeof(context));
	  }
	  effect = nextEffect;

	  uint32_t delay = 0;

	  unsigned int iteration = 0;
	  while (iteration < iterationTarget) {
		  switch (effect) {
		  case 0:
			  rainbow(&context.rainbow, 1, leds, LED_COUNT);
			  delay = context.rainbow.delay;
			  break;
		  case 1:
			  rainbow(&context.rainbow, 2, leds, LED_COUNT);
			  delay = context.rainbow.delay;
			  break;
		  case 2:
			  rainbow(&context.rainbow, 3, leds, LED_COUNT);
			  delay = context.rainbow.delay;
			  break;
		  case 3:
		  case 4:
			  running_lights(&context.runningLights, leds, LED_COUNT);
			  delay = context.runningLights.delay;
			  break;
		  default:
			  iteration = UINT16_MAX;
			  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		  }

		  HAL_Delay(delay);
		  paintHSV(leds, LED_COUNT);

		  iteration++;
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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
#define WS2812B_USE_GAMMA_CORRECTION
#define WS2812B_USE_PRECALCULATED_GAMMA_TABLE

#ifdef WS2812B_USE_GAMMA_CORRECTION
#ifdef WS2812B_USE_PRECALCULATED_GAMMA_TABLE
static const uint8_t LEDGammaTable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
    10, 11, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21,
    22, 23, 23, 24, 24, 25, 26, 26, 27, 28, 28, 29, 30, 30, 31, 32, 32, 33, 34, 35, 35, 36, 37, 38,
    38, 39, 40, 41, 42, 42, 43, 44, 45, 46, 47, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 56, 57, 58,
    59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 84,
    85, 86, 87, 88, 89, 91, 92, 93, 94, 95, 97, 98, 99, 100, 102, 103, 104, 105, 107, 108, 109, 111,
    112, 113, 115, 116, 117, 119, 120, 121, 123, 124, 126, 127, 128, 130, 131, 133, 134, 136, 137,
    139, 140, 142, 143, 145, 146, 148, 149, 151, 152, 154, 155, 157, 158, 160, 162, 163, 165, 166,
    168, 170, 171, 173, 175, 176, 178, 180, 181, 183, 185, 186, 188, 190, 192, 193, 195, 197, 199,
    200, 202, 204, 206, 207, 209, 211, 213, 215, 217, 218, 220, 222, 224, 226, 228, 230, 232, 233,
    235, 237, 239, 241, 243, 245, 247, 249, 251, 253, 255 };
#endif
#endif

static inline uint8_t LEDGamma(uint8_t v)
{
#ifdef WS2812B_USE_GAMMA_CORRECTION
#ifdef WS2812B_USE_PRECALCULATED_GAMMA_TABLE
    return LEDGammaTable[v];
#else
    return (v * v + v) >> 8;
#endif
#else
    return v;
#endif
}

void paintHSV(HSV_t *pData, uint16_t size) {

	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
		printf("*");
		HAL_Delay(1);
	}


	for (uint16_t i = 0; i < size; i++) {
		RGB_t rgb;

		HSV2RGB(&pData[i], &rgb);

		set(data, i, &rgb);
	}

//	for (uint16_t i = size; i < size + 1; i++) {
//		*((uint32_t*) (data + i * 9 + 3 * 0)) = 0;
//		*((uint32_t*) (data + i * 9 + 3 * 1)) = 0;
//		*((uint32_t*) (data + i * 9 + 3 * 2)) = 0;
//	}

	paint(data, LED_DATA_SIZE + (9 * 3));
}

void paintRGB(RGB_t *pData, uint16_t size) {

	for (uint16_t i = 0; i < size; i++) {
		set(data, i, &pData[i]);
	}

	paint(data, LED_DATA_SIZE);
}

void paint(uint8_t *pData, uint16_t size) {

	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
		printf("*");
		HAL_Delay(1);
	}

	if (HAL_SPI_Transmit_DMA(&hspi1, pData, size) != HAL_OK) {
		printf("SPI failed! \n");
	}

}

void clear(uint8_t *data) {
  for (int i = 0; i < LED_DATA_SIZE; i += 3) {
	  *((uint32_t*) (data + i)) = 0x244992;
  }
}

void set(uint8_t *data, uint16_t index, RGB_t *color) {

	uint8_t _r = LEDGamma(color->r);
	uint8_t _g = LEDGamma(color->g);
	uint8_t _b = LEDGamma(color->b);

	uint32_t R = ((0x00000004 | (((_r >> 0) & 0x1) << 1)) << (3 * 0 + 8))
			| ((0x00000004 | (((_r >> 1) & 0x1) << 1)) << (3 * 1 + 8))
			| ((0x00000004 | (((_r >> 2) & 0x1) << 1)) << (3 * 2 + 8))
			| ((0x00000004 | (((_r >> 3) & 0x1) << 1)) << (3 * 3 + 8))
			| ((0x00000004 | (((_r >> 4) & 0x1) << 1)) << (3 * 4 + 8))
			| ((0x00000004 | (((_r >> 5) & 0x1) << 1)) << (3 * 5 + 8))
			| ((0x00000004 | (((_r >> 6) & 0x1) << 1)) << (3 * 6 + 8))
			| ((0x00000004 | (((_r >> 7) & 0x1) << 1)) << (3 * 7 + 8));

	uint32_t G = ((0x00000004 | (((_g >> 0) & 0x1) << 1)) << (3 * 0 + 8))
			| ((0x00000004 | (((_g >> 1) & 0x1) << 1)) << (3 * 1 + 8))
			| ((0x00000004 | (((_g >> 2) & 0x1) << 1)) << (3 * 2 + 8))
			| ((0x00000004 | (((_g >> 3) & 0x1) << 1)) << (3 * 3 + 8))
			| ((0x00000004 | (((_g >> 4) & 0x1) << 1)) << (3 * 4 + 8))
			| ((0x00000004 | (((_g >> 5) & 0x1) << 1)) << (3 * 5 + 8))
			| ((0x00000004 | (((_g >> 6) & 0x1) << 1)) << (3 * 6 + 8))
			| ((0x00000004 | (((_g >> 7) & 0x1) << 1)) << (3 * 7 + 8));

	uint32_t B = ((0x00000004 | (((_b >> 0) & 0x1) << 1)) << (3 * 0 + 8))
			| ((0x00000004 | (((_b >> 1) & 0x1) << 1)) << (3 * 1 + 8))
			| ((0x00000004 | (((_b >> 2) & 0x1) << 1)) << (3 * 2 + 8))
			| ((0x00000004 | (((_b >> 3) & 0x1) << 1)) << (3 * 3 + 8))
			| ((0x00000004 | (((_b >> 4) & 0x1) << 1)) << (3 * 4 + 8))
			| ((0x00000004 | (((_b >> 5) & 0x1) << 1)) << (3 * 5 + 8))
			| ((0x00000004 | (((_b >> 6) & 0x1) << 1)) << (3 * 6 + 8))
			| ((0x00000004 | (((_b >> 7) & 0x1) << 1)) << (3 * 7 + 8));

//	*((uint32_t*) (data + index * 9 + 3 * 0)) |= bswap_32(G);
//	*((uint32_t*) (data + index * 9 + 3 * 1)) |= bswap_32(R);
//	*((uint32_t*) (data + index * 9 + 3 * 2)) |= bswap_32(B);

	*((uint32_t*) (data + index * 9 + 3 * 0)) = bswap_32(G);
	*((uint32_t*) (data + index * 9 + 3 * 1)) = bswap_32(R);
	*((uint32_t*) (data + index * 9 + 3 * 2)) = bswap_32(B);
}

uint32_t bswap_32(uint32_t x) {
	uint32_t t = x;
	uint32_t s;
	s = ((((uint32_t) (t) & (uint32_t) 0x000000ffUL) << 24)
			| (((uint32_t) (t) & (uint32_t) 0x0000ff00UL) << 8)
			| (((uint32_t) (t) & (uint32_t) 0x00ff0000UL) >> 8)
			| (((uint32_t) (t) & (uint32_t) 0xff000000UL) >> 24));
	return s;
}

void __io_putchar(uint8_t ch) {
	/**
	 * \brief		__io_putchar - A routine to transmit a single character out the serial port
	 * \return		void
	 * \param[in]	ch - uint8_t the character to transmit
	 * \author		andreichichak
	 * \date		Oct 16, 2015
	 * \details		This routine assumes that we will be using UART2. A timeout value of 1ms (4th parameter)
	 * 				gives a retry if the transmit buffer is full when back to back characters are transmitted,
	 * 				avoiding dropping characters.
	 */

	HAL_UART_Transmit(&huart2, &ch, 1, 1);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
