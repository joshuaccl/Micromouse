/*
 ******************************************************************************
 * File Name          : encoder.h
 * Description        : This file provides code for the functions
 *                      for the encoders.
 ****************************************************************************** */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __encoder_H
#define __encoder_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "tim.h"

extern uint32_t leftTicks;
extern uint32_t rightTicks;
extern uint32_t prevLeft;
extern uint32_t prevRight;

void encoderStart(void);

#endif
