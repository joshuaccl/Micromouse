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

void velocityLeft()
{
	// Kp and Kd are both constants for the proportional and derivative
	// control of the pd controller
	// Kp = 0.85
	float Kp=0.55;
	float Kd=0.15;
	int error; // current error
	float correctionP; // correction due to P
	float correctionD; // correction due to D
	int derivative;
	// get the current error
	error = trackingLeft();
	// get the derivative by subtracting current error with previous error
	derivative = error - getVelocityL();
	// get the proportional correction
	correctionP = error * Kp;
	// get the derivative correction
	correctionD = derivative * Kd;
	// add the corrections together
	correctionP += correctionD;
	// Change speed of motors
	// BASE_SPEED MACRO is in pdT.h
	rightMotorPWMChangeForward(BASE_SPEED - correctionP);
	leftMotorPWMChangeForward(correctionP + BASE_SPEED);
	// save current error value
	setVelocityL(error);
}
void velocityRight()
{
	// Kp and Kd are both constants for the proportional and derivative
	// control of the pd controller
	float Kp=0.55;
	float Kd=0.15;
	int error; // current error
	float correctionP; // correction due to P
	float correctionD; // correction due to D
	int derivative;
	// get the current error
	error = trackingRight();
	// get the derivative by subtracting current error with previous error
	derivative = error - getVelocityR();
	// get the proportional correction
	correctionP = error * Kp;
	// get the derivative correction
	correctionD = derivative * Kd;
	// add the corrections together
	correctionP += correctionD;
	// Change speed of motors
	rightMotorPWMChangeForward(correctionP + BASE_SPEED);
	leftMotorPWMChangeForward(BASE_SPEED - correctionP);
	// save current error value
	setVelocityR(error);
}

void velocityBothSides() {
	float Kp=0.55;
	float Kd=0.15;

	int error_L; // current error
	float correctionP_L; // correction due to P
	float correctionD_L; // correction due to D
	int derivative_L;

	int error_R; // current error
	float correctionP_R; // correction due to P
	float correctionD_R; // correction due to D
	int derivative_R;

	// get the current error
	error_L = trackingLeft();
	// get the derivative by subtracting current error with previous error
	derivative_L = error_L - getVelocityL();
	// get the proportional correction
	correctionP_L = error_L * Kp;
	// get the derivative correction
	correctionD_L = derivative_L * Kd;
	// add the corrections together
	correctionP_L += correctionD_L;

	// get the current error
	error_R = trackingRight();
	// get the derivative by subtracting current error with previous error
	derivative_R = error_R - getVelocityR();
	// get the proportional correction
	correctionP_R = error_R * Kp;
	// get the derivative correction
	correctionD_R = derivative_R * Kd;
	// add the corrections together
	correctionP_R += correctionD_R;

	rightMotorPWMChangeForward((BASE_SPEED - correctionP_L + correctionP_R));
	leftMotorPWMChangeForward((correctionP_L + BASE_SPEED - correctionP_R));

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

void wallTracking()
{
	int leftWall = getLeftFrontADCValue();
	int rightWall = getRightFrontADCValue();
	// Turn off leds
	turnOffLEDS();
	// If there is two walls
	if(leftWall >= LW_THRESHOLD && rightWall >= RW_THRESHOLD)
	{
		velocityBothSides();
		//		trackingRight();
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	}
	// If there is only a right wall
	else if(leftWall < LW_THRESHOLD && rightWall >= RW_THRESHOLD)
	{
		velocityRight();
		//		trackingRight();
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	}
	// If there is only a left wall
	else if(leftWall >= LW_THRESHOLD && rightWall < RW_THRESHOLD)
	{
		velocityLeft();
		//		trackingLeft();
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	}
	// No walls
	else
	{
		leftMotorPWMChangeForward(BASE_SPEED);
		rightMotorPWMChangeForward(BASE_SPEED);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	}
}
