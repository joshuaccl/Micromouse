/*
 ********************************************************************************
 * File Name          : algorithm.c
 * Description        : This file provides code for various algorithms
 ****************************************************************************** */

#include "algorithm.h"
#include "motors.h"
#include "adc.h"
#include "pdT.h"

void rightWallHugger(void)
{
	//No right wall -> turn right
	if (getRightFrontADCValue() <= NO_RIGHT_WALL)
	{
		leftMotorPWMChangeForward(125);
		rightMotorPWMChangeForward(125);
		HAL_Delay(500);
		rightTurn();
		motorStop();
	}
	//Wall on right and no front wall -> go straight
	else if (getRightFrontADCValue() > NO_RIGHT_WALL && (getLeftADCValue() < 35 && getRightADCValue() < 45))
	{
		trackingLeft();
		trackingRight();
	}
	//Wall on right and wall in front -> Turn left
	else if (getRightFrontADCValue() > NO_RIGHT_WALL && (getLeftADCValue() > 60 && getRightADCValue() > 85))
	{
		leftTurn();
		motorStop();

	}
	else{
		trackingLeft();
		trackingRight();
	}


}
