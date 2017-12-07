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
	float Kp=1;
	float Kd=1;
	int error;
	float correction;
	error = getLeftFrontADCValue() - LEFT_BASELINE;
	correction = error * Kp;
	leftMotorPWMChange(correction + 100);
	rightMotorPWMChange(100 - correction);

	error = getRightFrontADCValue() - RIGHT_BASELINE;
	correction = error * Kp;
	rightMotorPWMChange(correction + 100);
	leftMotorPWMChange(100 - correction);
}
