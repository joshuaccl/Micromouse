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
