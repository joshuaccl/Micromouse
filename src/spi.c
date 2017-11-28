/*
 ******************************************************************************
 * File Name          : spi.c
 * Description        : This file provides code for the configuration
 *                      of the spi instances.
 ****************************************************************************** */

#include "spi.h"

SPI_HandleTypeDef hspi3;

/* Init SPI3 */
void MX_SPI3_Init(void)
{
	hspi3.Instance = SPI3;
	hspi3.Init.Mode = SPI_MODE_MASTER;
	hspi3.Init.Direction = SPI_DIRECTION_2LINES;
	hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi3.Init.NSS = SPI_NSS_SOFT;
	hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi3.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi3) != HAL_OK)
	{
		Error_Handler();
	}
	__SPI3_CLK_ENABLE();
	__HAL_SPI_ENABLE(&hspi3);
}

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
