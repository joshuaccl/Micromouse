/*
 ******************************************************************************
 * File Name          : motors.c
 * Description        : This file provides code for the configuration
 *                      of the motor instances.
 ****************************************************************************** */

#include "motors.h"
#include "pdT.h"
#include "tim.h"
#include "encoder.h"
#include "main.h"

float leftDutyCycle;
float rightDutyCycle;

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
void leftMotorPWMChangeForward(float value)
{
	TIM_OC_InitTypeDef sConfigOC;
	leftDutyCycle = value;

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
void rightMotorPWMChangeForward(float value)
{
	TIM_OC_InitTypeDef sConfigOC;
	rightDutyCycle = value;

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

void leftMotorPWMChangeBackward(float value)
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
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	// PWM is relative based on the two channels so set 2nd channel to 0
	sConfigOC.Pulse = value;
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
}

void rightMotorPWMChangeBackward(float value)
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
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	// PWM is relative based on the two channels so set 2nd channel to 0
	sConfigOC.Pulse = 0;
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

void rightTurn(void)
{
	/* Disable interrupt before turning to ensure that
	 * the turn will not be interrupted by any other process */
	lockInterruptDisable_TIM3();
	motorStop();
	resetGyroAngle();
	resetLeftEncoder();
	leftMotorPWMChangeForward(300);
	rightMotorPWMChangeBackward(300);
	uint32_t encoder_value = MAX_ENCODER_VALUE;
	// Decrease absolute value of angle to turn less
	while(angle > -28.5 || encoder_value > (MAX_ENCODER_VALUE - 6650)) {
		setLeftEncoderValue(TIM2->CNT);
		encoder_value = getLeftEncoderValue();
	}
	motorStop();
	HAL_Delay(300);
	lockInterruptEnable_TIM3();
}

void leftTurn(void)
{
	/* Disable interrupt before turning to ensure that
	 * the turn will not be interrupted by any other process */
	lockInterruptDisable_TIM3();
	motorStop();
	resetGyroAngle();
	resetRightEncoder();
	rightMotorPWMChangeForward(300);
	leftMotorPWMChangeBackward(300);
	uint32_t encoder_value = MAX_ENCODER_VALUE;
	// Increase value of angle to turn more
	while( angle < 32.5 || encoder_value > (MAX_ENCODER_VALUE - 6700) ) {
		setRightEncoderValue(TIM5->CNT);
		encoder_value = getRightEncoderValue();
	}
	motorStop();
	HAL_Delay(300);
	lockInterruptEnable_TIM3();
}
void backward180Turn(void)
{
	lockInterruptDisable_TIM3();
	motorStop();
	resetGyroAngle();
	resetLeftEncoder();
	leftMotorPWMChangeForward(450);
	rightMotorPWMChangeBackward(450);
	uint32_t encoder_value = MAX_ENCODER_VALUE;
	// Decrease absolute value of angle to turn less
	// angle > -43
	// encoder_value > (MAX_ENCODER_VALUE - 13750
	while(angle > -43 || encoder_value > (MAX_ENCODER_VALUE - 10250)) {
		setLeftEncoderValue(TIM2->CNT);
		encoder_value = getLeftEncoderValue();
	}
	motorStop();
	HAL_Delay(300);
	lockInterruptEnable_TIM3();
	/* This delay needed to ensure that the ADC sensors will poll
	 * after turning */
}
void rightStillTurn(void)
{
	/* Disable interrupt before turning to ensure that
	 * the turn will not be interrupted by any other process */
	lockInterruptDisable_TIM3();
	motorStop();
	resetGyroAngle();
	resetLeftEncoder();
	leftMotorPWMChangeForward(300);
	rightMotorPWMChangeBackward(300);
	// Decrease absolute value of angle to turn less
	while(angle > -1320 ) {
	}
	motorStop();
	custom_delay(500);
	lockInterruptEnable_TIM3();
	resetLeftEncoder();
}

void leftStillTurn(void)
{
	/* Disable interrupt before turning to ensure that
	 * the turn will not be interrupted by any other process */
	lockInterruptDisable_TIM3();
	motorStop();
	resetGyroAngle();
	rightMotorPWMChangeForward(300);
	leftMotorPWMChangeBackward(300);
	// Increase value of angle to turn more
	while( angle < 1420 ) {
	}
	motorStop();
	custom_delay(500);
	lockInterruptEnable_TIM3();
	resetLeftEncoder();
}
void backward180StillTurn(void)
{
	lockInterruptDisable_TIM3();
	motorStop();
	resetGyroAngle();
	resetLeftEncoder();
	leftMotorPWMChangeForward(450);
	rightMotorPWMChangeBackward(450);
	// Decrease absolute value of angle to turn less
	// angle > -43
	// encoder_value > (MAX_ENCODER_VALUE - 13750
	while(angle > -1800 ) {
	}
	motorStop();
	custom_delay(500);
	lockInterruptEnable_TIM3();
	resetLeftEncoder();
	/* This delay needed to ensure that the ADC sensors will poll
	 * after turning */
}

void motorStop(void)
{
	rightMotorPWMChangeBackward(0);
	leftMotorPWMChangeBackward(0);
	rightMotorPWMChangeForward(0);
	leftMotorPWMChangeForward(0);
}
void motorAbruptStop(void)
{
	rightMotorPWMChangeBackward(1000);
	leftMotorPWMChangeBackward(1000);
	motorStop();
}
