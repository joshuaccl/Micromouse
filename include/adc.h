/*
 ******************************************************************************
 * File Name          : adc.h
 * Description        : This file provides code for the configuration
 *                      of the ADC instances.
 ****************************************************************************** */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __adc_H
#define __adc_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "dma.h"

/* Calibration for the ADC sensors, lower value -> see further */
/*                                  higher value -> see closer */
/* R_ADC is +10 more then L_ADC
 * LF_ADC is +10 more then RF_ADC
 */
#define L_ADC 30       // 0
#define LF_ADC 60      // 1
#define RF_ADC 50      // 2
#define R_ADC  40      // 3

extern ADC_HandleTypeDef hadc1;
extern uint32_t g_ADCBuffer[];
extern uint32_t IR_values[4];

void MX_ADC1_Init(void);
void ADC_IRQHandler(void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle);
//void ADC_LED_Distance_Tester(uint32_t IR_values[]);
void ADC_LED_Distance_Tester(void);

/* Function to print out ADC value reading to the LEDs using blinking to get sensor value */
void ADC_LED_DEBUG(uint32_t value);

#endif /*__ adc_H */
