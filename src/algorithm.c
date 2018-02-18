/*
 ********************************************************************************
 * File Name          : algorithm.c
 * Description        : This file provides code for various algorithms
 ****************************************************************************** */

#include "algorithm.h"
#include "motors.h"
#include "adc.h"
#include "pdT.h"
#include "encoder.h"

void rightWallHugger(void)
{
	//No right wall -> turn right
	if (getRightFrontADCValue() <= (NO_RIGHT_WALL-20))
	{
		advanceTicks(8000);
		rightTurn();
		advanceTicks(5000);
	}
	//Wall on right and no front wall -> go straight
	else if (getRightFrontADCValue() > (NO_RIGHT_WALL-20) && (getLeftADCValue() <= 1360 && getRightADCValue() <= 1178))
	{
		wallTracking();
	}
	//Wall on right and wall in front -> Turn left
	else if (getRightFrontADCValue() > (NO_RIGHT_WALL-20) && (getLeftADCValue() > 1360 && getRightADCValue() > 1178))
	{
		leftTurn();
	}
	else{
		wallTracking();
	}
}
