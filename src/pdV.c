/*
 ******************************************************************************
 * File Name          : pdV.c
 * Description        : This file provides code for the PD controller
 *                      for the velocity
 ****************************************************************************** */

#include "pdV.h"
//error = target velocity - current velocity

float leftVelocityOldError;
float rightVelocityOldError;
float leftIntegral;
float rightIntegral;
//float Kp=0.8;
//float Kd=0.1;
//float Ki=0.0005;
//
//float Kp=0.55;
//float Kd=0.15;
//float Ki=0.0005;

// Working version with I added
//float Kp=0.41;
//float Kd=0.125;
//float Ki=0.000025;

float Kp=0.55;
float Kd=0.15;
float Ki=0.0;
void velocityLeft()
{
	// Kp and Kd are both constants for the proportional and derivative
	// control of the pd controller
	// Kp = 0.85
	int error; // current error
	float correctionP; // correction due to P
	float correctionD; // correction due to D
	float correctionI; // correction due to I
	int derivative;
	// get the current error
	error = trackingLeft();
	// get the derivative by subtracting current error with previous error
	derivative = error - getVelocityL();
	// get the proportional correction
	correctionP = error * Kp;
	// get the derivative correction
	correctionD = derivative * Kd;
	// get the integral correction
	correctionI = error + getIntegralL();
	setIntegralL(correctionI);
	correctionI *= Ki;
	// add the corrections together
	correctionP += (correctionD + correctionI);
	// Change speed of motors
	// getBaseSpeed() MACRO is in pdT.h
	rightMotorPWMChangeForward(getBaseSpeed() - correctionP);
	leftMotorPWMChangeForward(correctionP + getBaseSpeed());
	// save current error value
	setVelocityL(error);
}
void velocityRight()
{
	// Kp and Kd are both constants for the proportional and derivative
	// control of the pd controller
	int error; // current error
	float correctionP; // correction due to P
	float correctionD; // correction due to D
	float correctionI; // correction due to I
	int derivative;
	// get the current error
	error = trackingRight();
	// get the derivative by subtracting current error with previous error
	derivative = error - getVelocityR();
	// get the proportional correction
	correctionP = error * Kp;
	// get the derivative correction
	correctionD = derivative * Kd;
	// get the integral correction
	correctionI = error + getIntegralR();
	setIntegralR(correctionI);
	correctionI *= Ki;
	// add the corrections together
	correctionP += (correctionD + correctionI);
	// Change speed of motors
	rightMotorPWMChangeForward(correctionP + getBaseSpeed());
	leftMotorPWMChangeForward(getBaseSpeed() - correctionP);
	// save current error value
	setVelocityR(error);
}

void velocityBothSides() {

	int error_L; // current error
	float correctionP_L; // correction due to P
	float correctionD_L; // correction due to D
	float correctionI_L; // correction due to I
	int derivative_L;

	int error_R; // current error
	float correctionP_R; // correction due to P
	float correctionD_R; // correction due to D
	float correctionI_R; // correction due to I
	int derivative_R;

	// get the current error
	error_L = trackingLeft();
	// get the derivative by subtracting current error with previous error
	derivative_L = error_L - getVelocityL();
	// get the proportional correction
	correctionP_L = error_L * Kp;
	// get the derivative correction
	correctionD_L = derivative_L * Kd;
	// get the integral correction
	correctionI_L = error_L + getIntegralL();
	setIntegralL(correctionI_L);
	correctionI_L *= Ki;
	// add the corrections together
	correctionP_L += (correctionD_L + correctionI_L);

	// get the current error
	error_R = trackingRight();
	// get the derivative by subtracting current error with previous error
	derivative_R = error_R - getVelocityR();
	// get the proportional correction
	correctionP_R = error_R * Kp;
	// get the derivative correction
	correctionD_R = derivative_R * Kd;
	// get the integral correction
	correctionI_R = error_R + getIntegralR();
	setIntegralR(correctionI_R);
	correctionI_R *= Ki;
	// add the corrections together
	correctionP_R += (correctionD_R + correctionI_R);

	rightMotorPWMChangeForward(getBaseSpeed() - correctionP_L + correctionP_R);
	leftMotorPWMChangeForward(correctionP_L + getBaseSpeed() - correctionP_R);

	setVelocityL(error_L);
	setVelocityR(error_R);
}
float getVelocityL(void)
{
	return leftVelocityOldError;
}
float getVelocityR(void)
{
	return rightVelocityOldError;
}
void setVelocityL(float value)
{
	leftVelocityOldError = value;
}
void setVelocityR(float value)
{
	rightVelocityOldError = value;
}
float getIntegralL(void)
{
	return leftIntegral;
}
float getIntegralR(void)
{
	return rightIntegral;
}
void setIntegralL(float value)
{
	leftIntegral = value;
}
void setIntegralR(float value)
{
	rightIntegral = value;
}

void wallTracking()
{
	int leftWall = getLeftFrontADCValue();
	int rightWall = getRightFrontADCValue();
	// If there is two walls
	if(leftWall >= LW_THRESHOLD && rightWall >= RW_THRESHOLD)
	{
		velocityBothSides();
//		 trackingBothSides();
	}
	// If there is only a right wall
	else if(leftWall < LW_THRESHOLD && rightWall >= RW_THRESHOLD)
	{
		velocityRight();
//		trackingRight();
	}
	// If there is only a left wall
	else if(leftWall >= LW_THRESHOLD && rightWall < RW_THRESHOLD)
	{
		velocityLeft();
//		trackingLeft();
	}
	// No walls
	else
	{
		leftMotorPWMChangeForward(getBaseSpeed());
		rightMotorPWMChangeForward(getBaseSpeed());
	}
}
