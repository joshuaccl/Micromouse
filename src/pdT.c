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
	if(error > CENTER_THRESHOLD){
//		correction = Kp * error;
//		correction += getLeftDutyCycle();
//		leftMotorPWMChange(correction);
//		correction = getRightDutyCycle() - correction;
//		rightMotorPWMChange(correction);
		rightMotorPWMChange(0);
		leftMotorPWMChange(100);
	}
	else{
		error = getRightFrontADCValue() - RIGHT_BASELINE;
		if(error > CENTER_THRESHOLD){
//			correction = Kp * error;
//			correction += getRightDutyCycle();
//			rightMotorPWMChange(correction);
//			correction = getLeftDutyCycle() - correction;
//			rightMotorPWMChange(correction);
			rightMotorPWMChange(100);
			leftMotorPWMChange(0);
		}
	}
}
