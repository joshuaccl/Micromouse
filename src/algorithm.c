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
		/* Disable interrupt before turning to ensure that
		 * the turn will not be interrupted by any other process */
		lockInterruptDisable_TIM3();
		rightTurn();
		lockInterruptEnable_TIM3();
		advanceTicks(ENCODER_TICKS_ONE_CELL);
	}
	//Wall on right and wall in front -> Turn left
	else if (getRightFrontADCValue() > NO_RIGHT_WALL && (getLeftADCValue() >=
	WALL_IN_FRONT_LEFT_SENSOR && getRightADCValue() >= WALL_IN_FRONT_RIGHT_SENSOR))
	{
		/* Disable interrupt before turning to ensure that
		 * the turn will not be interrupted by any other process */
		lockInterruptDisable_TIM3();
		leftTurn();
		lockInterruptEnable_TIM3();
	}
	else advanceTicks(ENCODER_TICKS_ONE_CELL);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);

}
