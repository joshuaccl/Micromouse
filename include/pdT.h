/*
 ******************************************************************************
 * File Name          : pdT.h
 * Description        : This file contains the headers for the PD controller
 *                      for tracking
 ****************************************************************************** */

#ifndef __pdT_H
#define __pdT_H

#include "stm32f4xx_hal.h"
#include "adc.h"
#include "motors.h"

#define LEFT_BASELINE 40
#define RIGHT_BASELINE 33
#define BASE_SPEED 100
// How sensitive the controller is

int trackingLeft(int leftOldError);
int trackingRight(int rightOldError);

#endif



