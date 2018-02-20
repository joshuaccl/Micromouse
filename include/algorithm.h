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
#include "lock.h"

#define NO_RIGHT_WALL 275
#define WALL_IN_FRONT_OPENING_RIGHT 470
#define WALL_IN_FRONT_LEFT_SENSOR 1150
#define WALL_IN_FRONT_RIGHT_SENSOR 975
#define NO_WALL_IN_FRONT_LEFT_SENSOR 1300
#define NO_WALL_IN_FRONT_RIGHT_SENSOR 1200
#define ENCODER_TICKS_ONE_CELL 15100
// true encoder value 15800

void rightWallHugger(void);

#endif /* __ALGORITHM_H */
