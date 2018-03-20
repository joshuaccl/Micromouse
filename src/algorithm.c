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
		rightTurn();
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
		advanceTicks(ENCODER_TICKS_ONE_CELL-4600);
	}
	// If there is a wall in front
	else if(getLeftADCValue() >= WALL_IN_FRONT_LEFT_SENSOR &&
			getRightADCValue() >= WALL_IN_FRONT_RIGHT_SENSOR) {

		// If there is no wall on the right -> turn right
		if(getRightFrontADCValue() <= WALL_IN_FRONT_OPENING_RIGHT)
		{
			rightTurn();
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
			advanceTicks(ENCODER_TICKS_ONE_CELL-4600);
		}
		else if(getLeftFrontADCValue() >= LEFT_WALL )
		{
			// Advance further into cell to do the 180
			backward180Turn();
			advanceTicks(ENCODER_TICKS_ONE_CELL-2800);
		}
		else // Turn left
		{
			leftTurn();
			// If no wall in front after turning left then go forward
			if(getLeftADCValue() < NO_WALL_IN_FRONT_LEFT_SENSOR &&
					getRightADCValue() < NO_WALL_IN_FRONT_RIGHT_SENSOR)
			{
				advanceTicks(ENCODER_TICKS_ONE_CELL-4600);
			}
		}
	}
	else advanceTicks(ENCODER_TICKS_ONE_CELL);
}
