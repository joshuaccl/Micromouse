/*
 ******************************************************************************
 * File Name          : lock.h
 * Description        : This file provides code for locking the interrupts
 ****************************************************************************** */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LOCK_H
#define __LOCK_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

void lockInterruptDisable_TIM3();
void lockInterruptEnable_TIM3();
void lockInterruptDisable_Gyro_Delay();
void lockInterruptEnable_Gyro_Delay();

#endif /* __LOCK_H */
