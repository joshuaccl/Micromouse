/*
 ******************************************************************************
 * File Name          : pdT.c
 * Description        : This file provides code for the PD controller
 *                      for the tracking
 ****************************************************************************** */

#include "pdT.h"

void trackingStart()
{
	// Change these constants to calibrate the controller Kp and Kd
	float Kp=2;
	float Kd=1;
	int error;
	float correction;
	error = getLeftFrontADCValue() - LEFT_BASELINE;
	correction = error * Kp;
	leftMotorPWMChangeForward(correction + 100);
	rightMotorPWMChangeForward(100 - correction);

	error = getRightFrontADCValue() - RIGHT_BASELINE;
	correction = error * Kp;
	rightMotorPWMChangeForward(correction + 100);
	leftMotorPWMChangeForward(100 - correction);
}
