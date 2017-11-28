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

void user_pwm_setValue_LeftMotors(uint16_t value);
void user_pwm_setValue_RightMotors(uint16_t value);

#endif /*__motors_H */
