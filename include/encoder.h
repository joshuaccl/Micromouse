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

void encoderStart(void);
long getLeftEncoderValue(void);
long getRightEncoderValue(void);
void setLeftEncoderValue(long value);
void setRightEncoderValue(long value);

// left and right encoder values
extern long leftEncoderValue;
extern long rightEncoderValue;

#endif
