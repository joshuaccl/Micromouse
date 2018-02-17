/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 ** This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * COPYRIGHT(c) 2017 STMicroelectronics
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
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "adc.h"
#include "dma.h"
#include "LED_Display.h"
#include "motors.h"
#include "spi.h"
#include "tim.h"
#include "encoder.h"
#include "pdT.h"
#include "pdV.h"
#include "algorithm.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

uint32_t leftTicks=0;
uint32_t rightTicks=0;

float left_velocity = 0;
float right_velocity = 0;
float angle = 0;

int left_counts = 0;
int left_last_counts = 0;
int right_counts = 0;
int right_last_counts = 0;
float inst_yaw = 0;

/* Main program */
int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();  // Init GPIO
	MX_ADC1_Init();  // Init ADC
	MX_TIM2_Init();  // Init L encoder
	MX_TIM4_Init();  // Init motors
	MX_TIM5_Init();  // Init R encoder

	MX_DMA_Init();   // Init ADC DMA

	/* Enable IR Emitter pins when mouse powers on */
	emitter_Init();

	/* Start mouse by waving hand across L emitter */
	mouseStartSensorWave();

	/* Initially set error for positional PD controller */
	setPositionL(0);
	setPositionR(0);
	leftMotorStart();
	rightMotorStart();

	// Have to start Timer3 interrupts after initializing motors
	MX_TIM3_Init();  // Software timer for algorithims
	MX_TIM11_Init(); // Software timer for gyro
	MX_SPI2_Init();  // SPI for gyro
	Init_IMU();      // Initialize gyro

	encoderStart();
	resetLeftEncoder();
	resetRightEncoder();

//while(1){
//				setLeftEncoderValue(TIM2->CNT);
//				setRightEncoderValue(TIM5->CNT);
//}
	//			motorStop();
	//			setLeftEncoderValue(TIM2->CNT);
	//			setRightEncoderValue(TIM5->CNT);
	//			HAL_Delay(10000);

//	rightTurn();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3){
//		trackingLeft();
//		trackingRight();
		wallTracking(); // used to track in the maze
		//
		//
		//		//
		//		//		rightTurn();
		//		//		motorStop();
		//		//		HAL_Delay(1000);
		//		//		HAL_Delay(10000);
//		rightWallHugger();
	}

	//	//tim 10
	//	else if (htim->Instance == QEI_VELOCITY_TIM_LABEL)
	//	{
	//		left_last_counts = left_counts;
	//		left_counts = QEI_Left_Read();
	//		left_velocity = (((float) (left_counts - left_last_counts))/ENCODERCPR) * ENCSAMPLEHZ * DISTPERREV * CMTOM; // m/s
	//		right_last_counts = right_counts;
	//		right_counts = QEI_Right_Read();
	//		right_velocity = (((float) (right_counts - right_last_counts))/ENCODERCPR) * ENCSAMPLEHZ * DISTPERREV * CMTOM; // m/s
	//	}
	// Interrupt timer for gyroscope
		if (htim->Instance == TIM11)
		{
			inst_yaw = GetAngle();
			if(inst_yaw > 5.5)
			{
				angle += (inst_yaw/100*.7);
			}
			else if(inst_yaw < -9)
			{
				angle += (inst_yaw/100*0.64);
			}
		}
}

/** System Clock Configuration
 */
void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	/**Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void _Error_Handler(char * file, int line)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while(1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */

}

#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
