#ifndef LEDDISPLAY_H
#define LEDDISPLAY_H

#include "font_LCD_Display.h"
#include "stm32f4xx_hal.h"

/* 5 bytes per character * maxchars, since 160 bits */
uint8_t dotRegister[LEDDISPLAY_MAXCHARS * 5];

/* Loads bits into Dot Register */
void fillDotRegister(char whatCharacter, uint8_t whatPosition);

#endif
