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
	float Kp=10;
	float Kd=0.5;
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
	leftMotorPWMChangeForward(correctionP + BASE_SPEED);
	rightMotorPWMChangeForward(BASE_SPEED - correctionP);
	// save current error value
	setVelocityL(error);
}
void velocityRight()
{
	// Kp and Kd are both constants for the proportional and derivative
	// control of the pd controller
	float Kp=10;
	float Kd=0.5;
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
	if(leftWall > LW_THRESHOLD && rightWall > RW_THRESHOLD)
	{
		velocityLeft();
		velocityRight();
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	}
	// If there is only a right wall
	else if(leftWall < LW_THRESHOLD && rightWall > RW_THRESHOLD)
	{
		velocityRight();
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	}
	// If there is only a left wall
	else if(leftWall > LW_THRESHOLD && rightWall < RW_THRESHOLD)
	{
		velocityLeft();
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
