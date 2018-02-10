/*
 ******************************************************************************
 * File Name          : pdT.c
 * Description        : This file provides code for the PD controller
 *                      for the tracking
 ****************************************************************************** */

#include "pdT.h"

int leftPositionOldError;
int rightPositionOldError;

int getPositionL()
{
	return leftPositionOldError;
}

int getPositionR()
{
	return rightPositionOldError;
}

// Used for positional controller, saves the error
void setPositionL( int value )
{
	leftPositionOldError = value;
}

void setPositionR( int value )
{
	rightPositionOldError = value;
}

float trackingLeft()
{
	// Change these constants to calibrate the controller Kp and Kd
	float Kp=2;
	float Kd=17.5;
	int error;
	float correctionP;
	float correctionD;
	int derivative;
	error = getLeftFrontADCValue() - LEFT_BASELINE;
	derivative = error - getPositionL();
	correctionP = error * Kp;
	correctionD = derivative * Kd;
	correctionP += correctionD;
	leftMotorPWMChangeForward(correctionP + BASE_SPEED);
	rightMotorPWMChangeForward(BASE_SPEED - correctionP);
	setPositionL(error);
	return correctionP;

}

float trackingRight()
{
	float Kp=2;
	float Kd=17.5;
	int error;
	float correctionP;
	float correctionD;
	int derivative;
	error = getRightFrontADCValue() - RIGHT_BASELINE;
	derivative = error - getPositionR();
	correctionP = error * Kp;
	correctionD = derivative * Kd;
	correctionP += correctionD;
	rightMotorPWMChangeForward(correctionP + BASE_SPEED);
	leftMotorPWMChangeForward(BASE_SPEED - correctionP);
	setPositionR(error);
	return correctionP;
}
