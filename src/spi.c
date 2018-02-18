/*
 ******************************************************************************
 * File Name          : spi.c
 * Description        : This file provides code for the configuration
 *                      of the spi instances.
 ****************************************************************************** */

#include "spi.h"
#include "tim.h"

SPI_HandleTypeDef hspi2;

uint8_t gyro_id_0;
uint8_t gyro_id_1;

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
  HAL_SPI_Init(&hspi2);
}

// Initialize IMU
void Init_IMU(void)
{
	uint8_t ctrl[2] = {0x11 & 0x7F, 0x44};
	HAL_GPIO_WritePin(GPIOC, IMU_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, &ctrl, 2, 1000);
	HAL_GPIO_WritePin(GPIOC, IMU_CS_Pin, GPIO_PIN_SET);
	HAL_TIM_Base_Init(&htim11);
	HAL_TIM_Base_Start_IT(&htim11);
}

// Function to ensure that the MCU can talk to the gyro. Ensure soldered correctly and able to
// receive/transmit with gyro.
void CheckID(void)
{
	//who_am_i
	uint8_t id[2];
	uint8_t who_am_i = 0x0F | 0x80;
	HAL_GPIO_WritePin(GPIOC, IMU_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, &who_am_i, (uint8_t *) &id, 2, 1000);
	gyro_id_0 = id[0];
	gyro_id_1 = id[1];
	HAL_GPIO_WritePin(GPIOC, IMU_CS_Pin, GPIO_PIN_SET);
}

float GetAngle(void)
{
	//Yaw
	uint8_t yaw_H[2];
	uint8_t OUTZ_H_G = 0x27 | 0x80;

	uint8_t yaw_L[2];
	uint8_t OUTZ_L_G = 0x26 | 0x80;

	int16_t yaw;

	//Get Angular rate
	HAL_GPIO_WritePin(GPIOC, IMU_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, &OUTZ_H_G, (uint8_t *) &yaw_H, 2, 1000);
	HAL_GPIO_WritePin(GPIOC, IMU_CS_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOC, IMU_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, &OUTZ_L_G, (uint8_t *) &yaw_L, 2, 1000);
	HAL_GPIO_WritePin(GPIOC, IMU_CS_Pin, GPIO_PIN_SET);

	yaw = (yaw_H[1] << 8) | (yaw_L[1]);
	if(yaw_H[1] & 0x80 == 0x80)
	{
		yaw = -(~yaw + 1);
	}
	return (yaw*0.01);
}

void resetGyroAngle(void) {
	angle = 0;
}
