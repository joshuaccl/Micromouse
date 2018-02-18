/*
 ******************************************************************************
 * File Name          : spi.h
 * Description        : This file contains all the function prototypes for
 *                      the spi.c file
 ****************************************************************************** */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __spi_H
#define __spi_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#define IMU_CS_Pin GPIO_PIN_8

void MX_SPI2_Init(void);
void Init_IMU(void);
void CheckID(void);
float GetAngle(void);
void resetGyroAngle(void);

#endif /* __spi_H */
