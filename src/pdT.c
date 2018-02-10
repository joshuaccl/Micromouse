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
	float Kp=0.008;
	float Kd=0.175;
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
//	leftMotorPWMChangeForward(correctionP + BASE_SPEED);
//	rightMotorPWMChangeForward(BASE_SPEED - correctionP);
	return correctionP;

}

float trackingRight()
{
	float Kp=0.008;
	float Kd=0.175;
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
//	rightMotorPWMChangeForward(correctionP + BASE_SPEED);
//	leftMotorPWMChangeForward(BASE_SPEED - correctionP);
	return correctionP;
}
