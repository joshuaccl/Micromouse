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
	double i,j;
	//No right wall -> turn right
	if (getRightFrontADCValue() < NO_RIGHT_WALL)
	{
		i = getRightEncoderValue();
		if( i > 25000)
		{
			j = i - 25000;
			while(getRightEncoderValue() > j) {
				setLeftEncoderValue(TIM2->CNT);
				setRightEncoderValue(TIM5->CNT);
			}
		}
		else{
			HAL_Delay(500);
		}
		rightTurn();
	}
	//Wall on right and no front wall -> go straight
	else if (getRightFrontADCValue() > (NO_RIGHT_WALL-20) && (getLeftADCValue() < 1360 && getRightADCValue() < 1178))
	{
		wallTracking();
	}
	//Wall on right and wall in front -> Turn left
	else if (getRightFrontADCValue() > (NO_RIGHT_WALL-20) && (getLeftADCValue() > 1360 && getRightADCValue() > 1178))
	{
		leftTurn();
		motorStop();

	}
	else{
		wallTracking();
	}


}
