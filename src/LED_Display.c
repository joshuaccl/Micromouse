#include "LED_Display.h"
#include "font_LCD_display.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include "pgmspace.h"

/* Loads bits into Dot Register */
/* Enter character and position: 1-4 */
/* Example: fillDotRegister('A', 1); */
void fillDotRegister(char whatCharacter, uint8_t whatPosition)
{
	// Calculate the starting position in the array
	// Every character has 5 columns made of 8 bits
	uint8_t thisPosition = whatPosition * 5;

	// Copy the appropriate bits into the dot register array
	for(int i = 0; i < 5; i++)
	{
		dotRegister[thisPosition+i] = (pgm_read_byte(&Font5x7[((whatCharacter - 0x20) * 5) + i]));
	}
}

void clearDotRegister(void)
{
	for (int i = 0; i < 160; i++)
	{
		dotRegister[i] = 0x00;
	}
}
