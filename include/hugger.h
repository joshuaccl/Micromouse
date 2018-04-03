/*
 ******************************************************************************
 * File Name          : hugger.h
 * Description        : This file provides code for various algorithms
 ****************************************************************************** */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HUGGER_H
#define __HUGGER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "motors.h"
#include "adc.h"
#include "pdT.h"
#include "pdV.h"
#include "encoder.h"
#include "lock.h"

#define NO_RIGHT_WALL 255
#define NO_LEFT_WALL 265
#define RIGHT_WALL 240
#define LEFT_WALL 260
#define WALL_IN_FRONT_OPENING_RIGHT 315
#define WALL_IN_FRONT_OPENING_LEFT 330
#define WALL_IN_FRONT_RIGHT_SENSOR 960
#define WALL_IN_FRONT_LEFT_SENSOR 1080
// 1200 1050
#define NO_WALL_IN_FRONT_RIGHT_SENSOR 1200
#define NO_WALL_IN_FRONT_LEFT_SENSOR 1300
#define FLOOD_WALL_IN_FRONT_RIGHT 800
#define FLOOD_WALL_IN_FRONT_LEFT  940
#define FLOOD_RIGHT_WALL 200
#define FLOOD_LEFT_WALL 220
#define ENCODER_TICKS_ONE_CELL 15150
// true encoder value 15800

void rightWallHugger(void);
void leftWallHugger(void);

#endif /* __HUGGER_H */
