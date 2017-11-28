/*
 ******************************************************************************
 * File Name          : LED_Display.h
 * Description        : This file contains all the function prototypes for
 *                      the LED_Display.c file
 ****************************************************************************** */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef LEDDISPLAY_H
#define LEDDISPLAY_H

#include "font_LCD_Display.h"
#include "stm32f4xx_hal.h"

/* 5 bytes per character * maxchars, since 160 bits */
uint8_t dotRegister[LEDDISPLAY_MAXCHARS * 5];

/* Loads bits into Dot Register */
void fillDotRegister(char whatCharacter, uint8_t whatPosition);
void clearDotRegister(void);

#endif
