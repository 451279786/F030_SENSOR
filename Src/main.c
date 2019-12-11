/* USER CODE BEGIN Header */
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
  * COPYRIGHT(c) 2019 STMicroelectronics
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "dht11.h"
#include <stdio.h>
#include <string.h>
#include "ch2o.h"
#include "PWMCapture.h"

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint16_t ADC_value = 0;


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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_ADC_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  EnableUartIT();
	OLED_Init();
	OLED_Clear(); 
	
	printf ("system ok!\n");
	uint16_t x = 0,y = 3,i = 0;//温湿度及坐标
	uint8_t  temperature = 0;  	    
	uint8_t  humidity = 0; 
	uint8_t tem[3] ;
	uint8_t hum[3] ;
	
	uint8_t ch2ochar[5] ;     //甲醛及单位
	uint8_t PPB[] = "ug/m3";  
	
	uint8_t smogchar[5] ;     //烟感及定时器
	uint32_t ADCTime = 0;
	
	uint8_t co2char[6] ;
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {		
		DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值
	  
		//Ch2oSensor();//一直不清空buff，会导致死机
		Int_To_Str(temperature,(char *)tem);         //温度数字转字符串
		Int_To_Str(humidity,(char *)hum);            //湿度数字转字符串
		Int_To_Str(Ch2oSensor(),(char *)ch2ochar);   //甲醛数字转字符串
	    Int_To_Str(HAL_ADC_GetValue(&hadc),(char *)smogchar);      //烟感ADC转字符串
		Int_To_Str(PWMCapture(),(char *)co2char);   //CO2

		x = 0;y = 0;
		OLED_ShowCHinese(x+0  ,y,4);//温度显示：
		OLED_ShowCHinese(x+=18,y,5);//
		OLED_ShowCHinese(x+=18,y,6);//
		OLED_ShowString (x+=7 ,y+1,tem,2);

		OLED_ShowCHinese(x+=25,y,7);//湿度显示：
		OLED_ShowCHinese(x+=18,y,8);//
		OLED_ShowCHinese(x+=18,y,9);//
		OLED_ShowString (x+=7 ,y+1,hum,2);

		y = 2;
		OLED_ShowCHinese(i+0 ,y,0);//滚动字体
		OLED_ShowCHinese(i+18,y,1);//
		OLED_ShowCHinese(i+36,y,2);//
		OLED_ShowCHinese(i+54,y,3);//
		i++;//实现滚动
		
		
		x = 0;y = 4;
		OLED_ShowString (x,y,"CO2:",strlen("CO2:"));   //CO2显示位置
		OLED_ShowString (x+=40,y,co2char,strlen((char *)co2char));
		x += (strlen((char *)co2char)*8);
		OLED_ShowString (x,y," ",2);//清除后面的显示
		
		x = 0;y = 5;
		OLED_ShowString (x,y,"Smog:",strlen("Smog:"));   //烟感显示位置
		OLED_ShowString (x+40,y,smogchar,strlen((char *)smogchar));

		x = 0;y = 6;
		OLED_ShowCHinese(x+0  ,y,10);//甲醛：
		OLED_ShowCHinese(x+=18,y,11);//
		OLED_ShowCHinese(x+=18,y,9);//
		OLED_ShowString (x+=7 ,y+1,ch2ochar,strlen((char *)ch2ochar));
		x += (strlen((char *)ch2ochar)*8);
		OLED_ShowString (x,y+1,PPB,strlen((char *)PPB));
		OLED_ShowString (x+=40,y+1," ",strlen((char *)PPB));//清除后面的显示


		if ((HAL_GetTick() - ADCTime) > 2000)
		{
			HAL_ADC_Start_IT(&hadc);//数据接收在ADC回调函数
			
			ADCTime = HAL_GetTick();
			
			PWMCapture();
			
			

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	ADC_value = HAL_ADC_GetValue(hadc);
	
	//printf("ADC_value = %d\n",ADC_value);
}


int fputc(int ch,FILE *f)   //串口重定向
{
	while((USART1->ISR&0x40)==0);
	USART1->TDR=ch;
	return ch;
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
