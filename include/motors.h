/*
 ******************************************************************************
 * File Name          : motors.h
 * Description        : This file contains all the functions prototypes for
 *                      the motors
 ****************************************************************************** */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __motors_H
#define __motors_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


void leftMotorPWMChangeForward(float value);
void rightMotorPWMChangeForward(float value);
void leftMotorPWMChangeBackward(float value);
void rightMotorPWMChangeBackward(float value);

void rightTurn(void);
void leftTurn(void);
void backward180Turn(void);
void leftSTurn(void);
#endif /*__motors_H */
