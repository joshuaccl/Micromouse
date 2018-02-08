/*
 ******************************************************************************
 * File Name          : dma.h
 * Description        : This file contains all the function prototypes for
 *                      the dma.c file
 ****************************************************************************** */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __dma_H
#define __dma_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "adc.h"

/* ADC DMA buffers */
extern DMA_HandleTypeDef g_DmaHandle;
enum { ADC_BUFFER_LENGTH = 8192 };
extern uint32_t g_ADCBuffer[ADC_BUFFER_LENGTH];
extern uint32_t IR_values[4];

void MX_DMA_Init(void);
void DMA2_Stream4_IRQHandler(void);

#endif /* __dma_H */
