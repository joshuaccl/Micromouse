/*
 ******************************************************************************
 * File Name          : encoder.c
 * Description        : This file provides code for the functions involving
 *                      the encoders.
 ****************************************************************************** */

#include "encoder.h"

uint32_t leftTicks;
uint32_t rightTick;
uint32_t prevLeft;
uint32_t prevRight;

void encoderStart(void)
{
	uint32_t leftTicks=0;
	uint32_t rightTicks=0;
	uint32_t prevLeft=0;
	uint32_t prevRight=0;

	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
}
