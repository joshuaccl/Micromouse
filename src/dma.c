/*
 ******************************************************************************
 * File Name          : dma.c
 * Description        : This file provides code for the configuration
 *                      of all the requested memory to memory DMA transfers.
 ****************************************************************************** */

#include "dma.h"

DMA_HandleTypeDef g_DmaHandle;

void MX_DMA_Init(void)
{
	__DMA2_CLK_ENABLE();
	g_DmaHandle.Instance = DMA2_Stream4;

	g_DmaHandle.Init.Channel  = DMA_CHANNEL_0;
	g_DmaHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	g_DmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
	g_DmaHandle.Init.MemInc = DMA_MINC_ENABLE;
	g_DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	g_DmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	g_DmaHandle.Init.Mode = DMA_CIRCULAR;
	g_DmaHandle.Init.Priority = DMA_PRIORITY_HIGH;
	g_DmaHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	g_DmaHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	g_DmaHandle.Init.MemBurst = DMA_MBURST_SINGLE;
	g_DmaHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;

	HAL_DMA_Init(&g_DmaHandle);

	__HAL_LINKDMA(&hadc1, DMA_Handle, g_DmaHandle);

	HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);

	HAL_ADC_Start_DMA(&hadc1, g_ADCBuffer, ADC_BUFFER_LENGTH);
}

void DMA2_Stream4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&g_DmaHandle);
}

