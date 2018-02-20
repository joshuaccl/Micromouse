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
		advanceTicks(ENCODER_TICKS_ONE_CELL);
	}
	// If there is a wall in front
	else if(getLeftADCValue() >= WALL_IN_FRONT_LEFT_SENSOR &&
			getRightADCValue() >= WALL_IN_FRONT_RIGHT_SENSOR) {
		leftTurn();
	}
	else advanceTicks(ENCODER_TICKS_ONE_CELL);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);

}
