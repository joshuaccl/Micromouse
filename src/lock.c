#include "lock.h"

void lockInterruptDisable_TIM3() {
	/* Peripheral clock enable */
	__HAL_RCC_TIM3_CLK_DISABLE();

	/* TIM3 interrupt DeInit */
	HAL_NVIC_DisableIRQ(TIM3_IRQn);
}
void lockInterruptEnable_TIM3() {
	/* Peripheral clock enable */
	__HAL_RCC_TIM3_CLK_ENABLE();
	/* TIM3 interrupt Init */
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}
void lockInterruptDisable_Gyro_Delay() {
	/* Peripheral clock enable */
	__HAL_RCC_TIM11_CLK_DISABLE();
	__HAL_RCC_TIM10_CLK_DISABLE();


	/* TIM3 interrupt DeInit */
	HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
	HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
}
void lockInterruptEnable_Gyro_Delay() {
	/* Peripheral clock enable */
	__HAL_RCC_TIM10_CLK_ENABLE();
	__HAL_RCC_TIM11_CLK_ENABLE();
	/* TIM3 interrupt Init */
	HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
}
