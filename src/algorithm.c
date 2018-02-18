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
	}
	//Wall on right and wall in front -> Turn left
	else if (getRightFrontADCValue() > NO_RIGHT_WALL && (getLeftADCValue() > 1200 && getRightADCValue() > 1050))
	{
		/* Disable interrupt before turning to ensure that
		 * the turn will not be interrupted by any other process */
		lockInterruptDisable_TIM3();
		leftTurn();
		lockInterruptEnable_TIM3();
	}
	else advanceTicks(12000);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);

}
