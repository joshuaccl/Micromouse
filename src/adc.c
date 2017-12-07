/*
 ******************************************************************************
 * File Name          : adc.c
 * Description        : This file provides code for the configuration
 *                      of the ADC instances.
 ****************************************************************************** */

#include "adc.h"
#include "dma.h"
#include "pdT.h"

ADC_HandleTypeDef hadc1;

uint32_t g_ADCBuffer[ADC_BUFFER_LENGTH];
uint32_t IR_values[4];

uint32_t ADC_L;
uint32_t ADC_LF;
uint32_t ADC_RF;
uint32_t ADC_R;

uint32_t startupSensor;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
	HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);

	ADC_ChannelConfTypeDef sConfig;

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = ENABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.NbrOfDiscConversion = 0;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 4;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.EOCSelection = DISABLE;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.Rank = 2;
	sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = 3;
	sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	sConfig.Channel = ADC_CHANNEL_3;
	sConfig.Rank = 4;
	sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}

void ADC_IRQHandler(void)
{
	HAL_ADC_IRQHandler(&hadc1);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
	int i,j,l=0,lf=0,rf=0,r=0;
	for(i = 0; i < ADC_BUFFER_LENGTH; i++){
		j = i % 4;
		switch(j) {
		case 0:
			l += g_ADCBuffer[i];
			break;
		case 1:
			lf += g_ADCBuffer[i];
			break;
		case 2:
			rf += g_ADCBuffer[i];
			break;
		case 3:
			r += g_ADCBuffer[i];
			break;
		}
	}
	IR_values[0] = l/2048;
	IR_values[1] = lf/2048;
	IR_values[2] = rf/2048;
	IR_values[3] = r/2048;

	ADC_L = IR_values[0];
	ADC_LF = IR_values[1];
	ADC_RF = IR_values[2];
	ADC_R = IR_values[3];
	startupSensor = IR_values[0];
}

/* Turns on LEDs based on the distance freom the sensor. Used for calibration */
/*
 * _____________________________
 * |    o     o     o     o    |
 * |___________________________|
 *      L     LF    RF    R
 */
void ADC_LED_Distance_Tester(void)
{
	if (IR_values[0] >= L_ADC)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);  //LED
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);  //LED
	}

	if (IR_values[1] >= LF_ADC)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);  //LED
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);  //LED
	}
	if (IR_values[2] >= RF_ADC)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);  //LED
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);  //LED
	}
	if (IR_values[3] >= R_ADC)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);  //LED
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);  //LED
	}
}

/* Function to print out ADC value reading to the LEDs using blinking to get sensor value */
void ADC_LED_DEBUG(uint32_t value)
{
	value = HAL_ADC_GetValue(&hadc1);
	int ones = value % 10;
	int tens = (value % 100)/10;
	int hundreds = (value % 1000)/100;
	int thousands = value/1000;
	for(;;){
		if(thousands>0) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
		if(hundreds>0) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		if(tens>0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		if(ones>0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_Delay(250);
		thousands--;
		hundreds--;
		tens--;
		ones--;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_Delay(250);
		if(thousands<0 && tens<0 && hundreds<0 && ones<0) break;
	}
}

uint32_t getLeftADCValue()
{
	return IR_values[0];
}

uint32_t getLeftFrontADCValue()
{
	return IR_values[1];
}

uint32_t getRightFrontADCValue()
{
	return IR_values[2];
}

uint32_t getRightADCValue()
{
	return IR_values[3];
}
void mouseStartSensorWave(void)
{
	//Turn on left LED
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	//Constantly spin until hand is waved
	while (1)
	{
		if (startMouseSensor() > 100)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);  //LED
			break;
		}
	}
}

uint32_t startMouseSensor(void)
{
	return startupSensor;
}
