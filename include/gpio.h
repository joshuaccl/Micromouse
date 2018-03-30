/*
 ******************************************************************************
 * File Name          : gpio.h
 * Description        : This file contains all the functions prototypes for
 *                      the gpio
 ****************************************************************************** */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

void MX_GPIO_Init(void);
void emitter_Init(void);
void emitter_Off(void);
void turnOnLEDS(void);
void turnOffLEDS(void);

#endif /*__gpio_H */
