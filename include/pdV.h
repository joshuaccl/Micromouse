/*
 ******************************************************************************
 * File Name          : pdV.h
 * Description        : This file contains the headers for the PD controller
 *                      for velocity
 ****************************************************************************** */

#ifndef __pdV_H
#define __pdV_H

// Positional PD controller feeds into the Velocity PD controller
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "motors.h"
#include "pdT.h"
#include "gpio.h"

extern float leftVelocityOldError;
extern float rightVelocityOldError;

void velocityLeft();
void velocityRight();
float getVelocityL(void);
float getVelocityR(void);
void setVelocityL(float value);
void setVelocityR(float value);
void wallTracking(void);
void velocityBothSides();
float getIntegralL(void);
float getIntegralR(void);
void setIntegralL(float value);
void setIntegralR(float value);

#endif



