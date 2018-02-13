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
void setLeftEncoderValue(double value);
void setRightEncoderValue(double value);
void resetLeftEncoder(void);
void resetRightEncoder(void);
// left and right encoder values
extern double leftEncoderValue;
extern double rightEncoderValue;

#endif
