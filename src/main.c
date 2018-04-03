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
#include "hugger.h"
#include "lock.h"
#include "flood.h"

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

// choose which algorithm to use in the beginning of the run
int algorithm;
struct dist_maze distances;
struct wall_maze cell_walls_info;
struct stack update_stack;
struct stack move_queue;

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
	MX_TIM10_Init();
	MX_DMA_Init();   // Init ADC DMA

	/* Enable IR Emitter pins when mouse powers on */
	emitter_Init();

	/* Start mouse by waving hand across L emitter */
	algorithm = mouseStartSensorWave();
	custom_delay(1000);

	/* Initially set error for positional PD controller */
	setPositionL(0);
	setPositionR(0);
	setIntegralL(0);
	setIntegralR(0);
	leftMotorStart();
	rightMotorStart();

	MX_TIM11_Init(); // Software timer for gyro
	MX_SPI2_Init();  // SPI for gyro
	Init_IMU();      // Initialize gyro

	encoderStart();
	resetLeftEncoder();
	resetRightEncoder();

	// Floodfill
	if(algorithm == 2)
	{
		algorithm = wallFavor();
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);

		HAL_Delay(1000);
		// initialize all the structs that we need for this to work

		// change this coordinate for testing of different
		// targets for floodfill
		struct coor target;
		init_coor(&target, 8, 7);

		// to flood to center set third parameter to 1
		init_distance_maze(&distances, &target, 1);

		// initialize the walls
		init_wall_maze(&cell_walls_info);

		// set east, south, west wall of start cell to true
		cell_walls_info.cells[0][0].walls[EAST] = 1;
		cell_walls_info.cells[0][0].walls[SOUTH] = 1;
		cell_walls_info.cells[0][0].walls[WEST] = 1;

		struct coor c;
		init_coor(&c, 0, 0);

		MX_TIM3_Init();  // Software timer for tracking
		int direction = NORTH;
		update_stack.index = 0;
		direction = floodFill(&distances, &c, &cell_walls_info, algorithm, direction, &update_stack);

		direction = centerMovement(&cell_walls_info, &c, direction);
		// ONCE IT REACHES HERE, IT HAS REACHED THE CENTER OF THE MAZE
		// Mouse has made it to center, so flood back to start
		init_coor(&target, 0, 0);
		init_distance_maze(&distances, &target, 0);

		// center to start
		logicalFlood(&distances, &c, &cell_walls_info, direction, direction, &update_stack);
		direction = floodFill(&distances, &c, &cell_walls_info, direction, direction, &update_stack);

		int difference = direction - NORTH;
		switch(difference)
		{
		case -3:
			leftStillTurn();
			break;
		case -2:
			backward180StillTurn();
			break;
		case -1:
			rightStillTurn();
			break;
		case 0:
			break;
		case 1:
			leftStillTurn();
			break;
		case 2:
			backward180StillTurn();
			break;
		case 3:
			rightStillTurn();
			break;
		default:
			turnOnLEDS();
			break;
		}
		direction = NORTH;

		// start to center in "shortest path"
		init_distance_maze(&distances, &c, 1);
		logicalFlood(&distances, &c, &cell_walls_info, direction, direction, &update_stack);
		lockInterruptDisable_TIM3();
		leftMotorPWMChangeBackward(200);
		rightMotorPWMChangeBackward(200);
		custom_delay(2000);
		motorStop();
		wallFavor();
		custom_delay(1000);
		shortestPath(&distances, &c, &cell_walls_info, direction, direction, &update_stack);
		motorStop();
		turnOnLEDS();
		HAL_Delay(3000);
	}
	// Right wall hugger
	if(algorithm == 1)
	{
		MX_TIM3_Init();  // Software timer for tracking
		while(1)
		{
			setBaseSpeed(40);
			rightWallHugger();
		}
	}
	// Left wall hugger
	if(algorithm == 0)
	{
		MX_TIM3_Init();  // Software timer for tracking
		while(1)
		{
			setBaseSpeed(40);
			leftWallHugger();
		}
	}
}

// Timers are automatically called when mouse is on according to the polling rate set for each timer
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// Timer for wall tracking
	if(htim->Instance==TIM3){
		// Track off walls of maze to keep mouse in center and prevent crashing into walls
		wallTracking();
	}
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
	// Custom delay
	if (htim->Instance == TIM10)
	{
		time_of_delay++;
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
