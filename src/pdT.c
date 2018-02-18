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
	// kp = .0625, kd=1.4
	// Change these constants to calibrate the controller Kp and Kd
	float Kp=0.0625;
	float Kd=1.4;
	int error;
	float correctionP;
	float correctionD;
	int derivative;
	error = getLeftFrontADCValue() - LEFT_BASELINE;
	derivative = error - getPositionL();
	correctionP = error * Kp;
	correctionD = derivative * Kd;
	correctionP += correctionD;
	setPositionL(error);
	// Uncomment the next two lines to test positional controller only
//	leftMotorPWMChangeForward(correctionP + BASE_SPEED);
//	rightMotorPWMChangeForward(BASE_SPEED - correctionP);
	return correctionP;
}

float trackingRight()
{
	float Kp=0.0625;
	float Kd=1.4;
	int error;
	float correctionP;
	float correctionD;
	int derivative;
	error = getRightFrontADCValue() - RIGHT_BASELINE;
	derivative = error - getPositionR();
	correctionP = error * Kp;
	correctionD = derivative * Kd;
	correctionP += correctionD;
	setPositionR(error);
	// Uncomment the next two lines to test positional controller only
//	rightMotorPWMChangeForward(correctionP + BASE_SPEED);
//	leftMotorPWMChangeForward(BASE_SPEED - correctionP);
	return correctionP;
}
