/*
 ******************************************************************************
 * File Name          : pdT.c
 * Description        : This file provides code for the PD controller
 *                      for the tracking
 ****************************************************************************** */

#include "pdT.h"

int leftPositionOldError;
int rightPositionOldError;
int base_speed;

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

// No wall on right so only track off left wall
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
	/* ************************************************************ */
	// Uncomment the next two lines to test positional controller only
	//		rightMotorPWMChangeForward(getBaseSpeed() - correctionP);
	//		leftMotorPWMChangeForward(correctionP + getBaseSpeed());
	/* ************************************************************ */
	return correctionP;
}

// No wall on left so only track off right wall
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
	/* ************************************************************ */
	// Uncomment the next two lines to test positional controller only
	//		rightMotorPWMChangeForward(correctionP + getBaseSpeed());
	//		leftMotorPWMChangeForward(getBaseSpeed() - correctionP);
	/* ************************************************************ */
	return correctionP;
}

// Track off both walls
void trackingBothSides()
{
	float Kp=0.0625;
	float Kd=1.4;

	int errorR;
	float correctionP_R;
	float correctionD_R;
	int derivative_R;

	int errorL;
	float correctionP_L;
	float correctionD_L;
	int derivative_L;

	errorR = getRightFrontADCValue() - RIGHT_BASELINE;
	derivative_R = errorR - getPositionR();
	correctionP_R = errorR * Kp;
	correctionD_R = derivative_R * Kd;
	correctionP_R += correctionD_R;
	setPositionR(errorR);

	errorL = getLeftFrontADCValue() - LEFT_BASELINE;
	derivative_L = errorL - getPositionL();
	correctionP_L = errorL * Kp;
	correctionD_L = derivative_L * Kd;
	correctionP_L += correctionD_L;
	setPositionL(errorL);

	/* ************************************************************ */
	// Uncomment the next two lines to test positional controller only
	//	rightMotorPWMChangeForward(getBaseSpeed() - correctionP_L + correctionP_R);
	//	leftMotorPWMChangeForward(correctionP_L + getBaseSpeed() - correctionP_R);
	/* ************************************************************ */
}

int getBaseSpeed(void)
{
	return base_speed;
}
void setBaseSpeed(int value)
{
	base_speed = value;
}
