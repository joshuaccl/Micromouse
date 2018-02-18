/*
 ******************************************************************************
 * File Name          : algorithm.h
 * Description        : This file provides code for various algorithms
 ****************************************************************************** */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ALGORITHM_H
#define __ALGORITHM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "motors.h"
#include "adc.h"
#include "pdT.h"
#include "pdV.h"
#include "encoder.h"

#define NO_RIGHT_WALL 210

void rightWallHugger(void);

#endif /* __ALGORITHM_H */
