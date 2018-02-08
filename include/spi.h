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

void writeDisplay(char character, uint8_t position);

#endif /* __spi_H */
