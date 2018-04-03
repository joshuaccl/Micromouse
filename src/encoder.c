/*
 ******************************************************************************
 * File Name          : encoder.c
 * Description        : This file provides code for the functions involving
 *                      the encoders.
 ****************************************************************************** */

#include "encoder.h"

uint32_t leftEncoderValue;
uint32_t rightEncoderValue;

void encoderStart(void)
{
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
}

uint32_t getLeftEncoderValue()
{
	return leftEncoderValue;
}

uint32_t getRightEncoderValue()
{
	return rightEncoderValue;
}
void setLeftEncoderValue(uint32_t value)
{
	leftEncoderValue = value;
}
void setRightEncoderValue(uint32_t value)
{
	rightEncoderValue = value;
}
void resetLeftEncoder(void) {
	HAL_TIM_Encoder_Stop(&htim2, TIM_CHANNEL_ALL);
	TIM2->CNT = MAX_ENCODER_VALUE;
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
}
void resetRightEncoder(void) {
	HAL_TIM_Encoder_Stop(&htim5, TIM_CHANNEL_ALL);
	TIM5->CNT = MAX_ENCODER_VALUE;
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
}
void advanceTicks(uint32_t ticks) {
	uint32_t encoder_val = MAX_ENCODER_VALUE;
	resetLeftEncoder();
	while(encoder_val > (MAX_ENCODER_VALUE - ticks) ) {
		if (getLeftADCValue() >= WALL_IN_FRONT_LEFT_SENSOR &&
				getRightADCValue() >= WALL_IN_FRONT_RIGHT_SENSOR)
		{
			break;
		}
		setLeftEncoderValue(TIM2->CNT);
		encoder_val = getLeftEncoderValue();
	}
	resetLeftEncoder();
}

void uncontrolledAdvanceTicks(uint32_t ticks) {
	uint32_t encoder_val = MAX_ENCODER_VALUE;
	resetLeftEncoder();
	while(encoder_val > (MAX_ENCODER_VALUE - ticks) ) {
		setLeftEncoderValue(TIM2->CNT);
		encoder_val = getLeftEncoderValue();
	}
}


