/*
 ******************************************************************************
 * File Name          : pdT.c
 * Description        : This file provides code for the PD controller
 *                      for the tracking
 ****************************************************************************** */

#include "pdT.h"

void trackingStart()
{
	float Kp=1;
	float Kd=1;
	int error;
	float correction;
	error = getLeftFrontADCValue() - LEFT_BASELINE;
	if(error > CENTER_THRESHOLD || error < -CENTER_THRESHOLD){
		correction = Kp * error;
		correction /= 2;
		correction += getLeftDutyCycle();
		leftMotorPWMChange(correction);
	}
	error = getRightFrontADCValue() - RIGHT_BASELINE;
	if(error > CENTER_THRESHOLD || error < -CENTER_THRESHOLD){
		correction = Kp * error;
		correction /= 2;
		correction += getRightDutyCycle();
		rightMotorPWMChange(correction);
	}
}
