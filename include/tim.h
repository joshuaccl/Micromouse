/*
 ******************************************************************************
 * File Name          : tim.h
 * Description        : This file provides code for the configuration
 *                      of the TIM instances.
 ****************************************************************************** */

#ifndef __tim_H
#define __tim_H

#define TIMER_CLOCK_SPEED 84000000
#define PWM_TIMER_PRESCALE 83

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "encoder.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;
extern int delay_flag;
extern int time_of_delay;

void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);
void MX_TIM5_Init(void);
void MX_TIM10_Init(void);
void MX_TIM11_Init(void);
void leftMotorStart(void);
void rightMotorStart(void);
void custom_delay(int milliseconds);

#endif /*__ tim_H */
