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

#define LEFT_BASELINE 370
#define RIGHT_BASELINE 414

// Change this macro to affect the travel speed
#define BASE_SPEED 40

extern int leftPositionOldError;
extern int rightPositionOldError;

// How sensitive the controller is

float trackingLeft(void);
float trackingRight(void);
int getPositionL(void);
int getPositionR(void);
void setPositionL(int value);
void setPositionR(int value);

#endif



