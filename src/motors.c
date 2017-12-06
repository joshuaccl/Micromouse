/*
 ******************************************************************************
 * File Name          : motors.c
 * Description        : This file provides code for the configuration
 *                      of the motor instances.
 ****************************************************************************** */

#include "motors.h"
#include "tim.h"

/* Motors initialized through rightMotorStart and leftMotorStart functions
 * To change PWM values on the fly, call leftMotorPWMChange or rightMotorPWMChange
 *
 * System clock for stm32f405 is 84 MHz
 *
 * Period = ((system clock/(prescalar +1))/ desired frequency) -1
 *
 * Duty Cycle = Pulse / Period
 *
 */

/* Pulse PWM value change for left motors (CH1/CH2) */
void leftMotorPWMChange(float value)
{
	TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = value;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_Init(&htim4);
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	// PWM is relative based on the two channels so set 2nd channel to 0
	sConfigOC.Pulse = 0;
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
}

/* Pulse PWM value change for right motors (CH3/CH4) */
void rightMotorPWMChange(float value)
{
	TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_Init(&htim4);
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	// PWM is relative based on the two channels so set 2nd channel to 0
	sConfigOC.Pulse = value;
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

