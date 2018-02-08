/*
 ******************************************************************************
 * File Name          : spi.c
 * Description        : This file provides code for the configuration
 *                      of the spi instances.
 ****************************************************************************** */

#include "spi.h"

SPI_HandleTypeDef hspi2;

/* SPI2 init function */
void MX_SPI2_Init(void)
{

  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler(__FILE_, _LINE_);
  }

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
