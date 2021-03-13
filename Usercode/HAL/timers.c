/*
 * timers.c
 *
 *  Created on: 25 Nov 2020
 *      Author: kraego
 */
#include "timers.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"

static void configure_tim1(uint32_t ms) {
	uint16_t arr = (ms == 1) ? 1 : (ms - 1);

	/*
	 * Fires on start:
	 * 	set arr back to 0 after given ms, capture compare matches 0
	 * 	--> TIM1_CC_IRQHandler is called via interrupt
	 */
	tim1_t *timerConfig = TIM1_REG;
	timerConfig->PSC = HAL_RCC_GetPCLK1Freq() / 1000; 		 // prescaler: 1 tick / millisecond ( / 1000)
	timerConfig->ARR = arr;									 // auto reload register: max value of counter before restarting at 0
	timerConfig->CCR1 = 0;  								 // capture compare Register: Interrupt when counter value reaches this value
	timerConfig->DIER_BITS.val1 |= 0x1; 			 		 // enable capture compare 1 interrupt
	timerConfig->CR1 = 0x1; 								 // enable Timer - CEN is bit 0
	timerConfig->SR &= ~(1 << (1));							 // clear eventual pending

	HAL_NVIC_SetPriority(TIM1_CC_IRQn, 14, 14);
	HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
}

static void enable_tim1() {
	// enable Timer1 - TIM1EN - see: reference manual: 280
	rcc_apb2enr_t *rcc_apb_enr = RCC_APB2_ENR;
	rcc_apb_enr->bits.val11 |= 0x01;
}

/**
 * Create timer with x ms.
 *
 * @param ms timer time in ms
 * @returns: 0 in case of success, < 0 in case of failure
 */
uint32_t timers_init(uint32_t ms) {
	if (((ms * 10) - 1) >= UINT16_MAX) {
		return -1;
	}

	enable_tim1();
	configure_tim1(ms);
	return 0;
}
