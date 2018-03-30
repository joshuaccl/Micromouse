/*
 ******************************************************************************
 * File Name          : encoder.h
 * Description        : This file provides code for the functions
 *                      for the encoders.
 ****************************************************************************** */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __encoder_H
#define __encoder_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "tim.h"
#include "pdT.h"
#include "pdV.h"
#include "hugger.h"

#define MAX_ENCODER_VALUE 2147483648

void encoderStart(void);
uint32_t getLeftEncoderValue(void);
uint32_t getRightEncoderValue(void);
void setLeftEncoderValue(uint32_t value);
void setRightEncoderValue(uint32_t value);
void resetLeftEncoder(void);
void resetRightEncoder(void);
void advanceTicks(uint32_t ticks);
void uncontrolledAdvanceTicks(uint32_t ticks);
// left and right encoder values
extern uint32_t leftEncoderValue;
extern uint32_t rightEncoderValue;


#endif
