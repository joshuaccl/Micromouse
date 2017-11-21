#include "helper.h"

void ADC_LED_DEBUG(uint32_t value)
{
	int ones = value % 10;
	int tens = (value % 100)/10;
	int hundreds = (value % 1000)/100;
	int thousands = value/1000;
	for(;;){
		if(thousands>0) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
		if(hundreds>0) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		if(tens>0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		if(ones>0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_Delay(500);
		thousands--;
		hundreds--;
		tens--;
		ones--;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_Delay(500);
		if(thousands<0 && tens<0 && hundreds<0 && ones<0) break;
	}
}
