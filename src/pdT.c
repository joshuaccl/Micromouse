/*
 ******************************************************************************
 * File Name          : pdT.c
 * Description        : This file provides code for the PD controller
 *                      for the tracking
 ****************************************************************************** */

#include "pdT.h"

int leftOldError;
int rightOldError;

int getL()
{
	return leftOldError;
}

int getR()
{
	return rightOldError;
}

void setL( int value )
{
	leftOldError = value;
}

void setR( int value )
{
	rightOldError = value;
}

void trackingLeft()
{
	// Change these constants to calibrate the controller Kp and Kd
	float Kp=2;
	float Kd=17.5;
	int error;
	float correctionP;
	float correctionD;
	int derivative;
	error = getLeftFrontADCValue() - LEFT_BASELINE;
	derivative = error - getL();
	correctionP = error * Kp;
	correctionD = derivative * Kd;
	correctionP += correctionD;
	leftMotorPWMChangeForward(correctionP + BASE_SPEED);
	rightMotorPWMChangeForward(BASE_SPEED - correctionP);
	setL(error);
}

void trackingRight()
{
	float Kp=2;
	float Kd=17.5;
	int error;
	float correctionP;
	float correctionD;
	int derivative;
	error = getRightFrontADCValue() - RIGHT_BASELINE;
	derivative = error - getR();
	correctionP = error * Kp;
	correctionD = derivative * Kd;
	correctionP += correctionD;
	rightMotorPWMChangeForward(correctionP + BASE_SPEED);
	leftMotorPWMChangeForward(BASE_SPEED - correctionP);
	setR(error);
}
