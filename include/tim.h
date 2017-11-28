/*
 ******************************************************************************
 * File Name          : tim.h
 * Description        : This file provides code for the configuration
 *                      of the TIM instances.
 ****************************************************************************** */

#ifndef __tim_H
#define __tim_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

void MX_TIM2_Init(void);
void MX_TIM4_Init(void);
void MX_TIM5_Init(void);

//void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

#endif /*__ tim_H */
