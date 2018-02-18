/*
 ********************************************************************************
 * File Name          : algorithm.c
 * Description        : This file provides code for various algorithms
 ****************************************************************************** */

#include "algorithm.h"

void rightWallHugger(void)
{
	//No right wall -> turn right
	if (getRightFrontADCValue() <= NO_RIGHT_WALL)
	{
		advanceTicks(8000);
		motorStop();
		rightTurn();
		advanceTicks(5000);
	}
	//Wall on right and wall in front -> Turn left
	else if (getRightFrontADCValue() > NO_RIGHT_WALL && (getLeftADCValue() > 1200 && getRightADCValue() > 1050))
	{
		motorStop();
		leftTurn();
		advanceTicks(5000);
	}
	//Wall on right and no front wall -> go straight
	else if (getRightFrontADCValue() > NO_RIGHT_WALL && (getLeftADCValue() <= 1360 && getRightADCValue() <= 1178))
	{
		wallTracking();
	}
	else{
		wallTracking();
	}
}
