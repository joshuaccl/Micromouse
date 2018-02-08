/*
 ******************************************************************************
 * File Name          : spi.c
 * Description        : This file provides code for the configuration
 *                      of the spi instances.
 ****************************************************************************** */

#include "spi.h"

/* Load dot register and write into display */
void writeDisplay(char character, uint8_t position)
{
	//
	//		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);  //RS
	//		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);  //CE LOW
	//		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);  //DATA_IN
	//
	//		HAL_SPI_Transmit(&hspi3, dotRegister, 1280, HAL_MAX_DELAY);
	//		while(HAL_SPI_GetState(&hspi3) == HAL_SPI_STATE_BUSY);
	//
	//		HAL_Delay(10);
	//		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);  //CLK
	//
	//		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);  //CE

}
