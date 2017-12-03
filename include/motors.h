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

void leftMotorPWMChange(float value);
void rightMotorPWMChange(float value);

#endif /*__motors_H */
