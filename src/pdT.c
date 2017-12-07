/*
 ******************************************************************************
 * File Name          : pdT.c
 * Description        : This file provides code for the PD controller
 *                      for the tracking
 ****************************************************************************** */

#include "pdT.h"

int trackingLeft(leftOldError)
{
	// Change these constants to calibrate the controller Kp and Kd
	float Kp=2;
	float Kd=1;
	int error;
	float correctionP;
	float correctionD;
	int derivative;
	error = getLeftFrontADCValue() - LEFT_BASELINE;
	derivative = error - leftOldError;
	correctionP = error * Kp;
	correctionD = derivative * Kd;
	correctionP += correctionD;
	leftMotorPWMChangeForward(correctionP + BASE_SPEED);
	rightMotorPWMChangeForward(BASE_SPEED - correctionP);
	return error;
}

int trackingRight(rightOldError)
{
	float Kp=2;
	float Kd=1;
	int error;
	float correctionP;
	float correctionD;
	int derivative;
	error = getRightFrontADCValue() - RIGHT_BASELINE;
	derivative = error - rightOldError;
	correctionP = error * Kp;
	correctionD = derivative * Kd;
	correctionP += correctionD;
	rightMotorPWMChangeForward(correctionP + BASE_SPEED);
	leftMotorPWMChangeForward(BASE_SPEED - correctionP);
	return error;
}
