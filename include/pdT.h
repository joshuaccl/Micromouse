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

// Calibrations for PD controller to keep mouse centered when moving
#define LEFT_BASELINE 370
#define RIGHT_BASELINE 414
#define LW_THRESHOLD 260
#define RW_THRESHOLD 270

extern int leftPositionOldError;
extern int rightPositionOldError;
extern int base_speed;

// How sensitive the controller is

float trackingLeft(void);
float trackingRight(void);
int getPositionL(void);
int getPositionR(void);
void setPositionL(int value);
void setPositionR(int value);
void trackingBothSides();
int getBaseSpeed(void);
void setBaseSpeed(int value);

#endif



