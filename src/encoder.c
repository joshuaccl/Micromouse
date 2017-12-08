/*
 ******************************************************************************
 * File Name          : encoder.c
 * Description        : This file provides code for the functions involving
 *                      the encoders.
 ****************************************************************************** */

#include "encoder.h"
#include "stm32f4xx_hal.h"
#include "tim.h"
#include "stm32f4xx_hal_tim.h"

long leftEncoderValue;
long rightEncoderValue;

void encoderStart(void)
{
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
}

long getLeftEncoderValue()
{
	return leftEncoderValue;
}

long getRightEncoderValue()
{
	return rightEncoderValue;
}
void setLeftEncoderValue(long value)
{
	leftEncoderValue = value;
}
void setRightEncoderValue(long value)
{
	rightEncoderValue = value;
}
