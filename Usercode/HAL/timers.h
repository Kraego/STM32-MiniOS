/*
 * timers.h
 *
 *  Created on: 25 Nov 2020
 *      Author: kraego
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include <stdint.h>
#include "bitFields.h"

#define RCC_BASE_ADDRESS		(0x40021000)
#define APB2ENR_OFFSET			(0x60)
#define TIM1_BASE_ADDRESS		(0x40012c00)

typedef struct rcc_apb2enr_t {
	union {
			volatile Reg32_1b_t bits;
			volatile uint32_t raw_mode;				// offset 0x0
		};
} rcc_apb2enr_t;

typedef struct tim1_t {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	union {
			volatile Reg16_1b_t DIER_BITS;
			volatile uint32_t DIER_RAW;				// offset 0x0
		};
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint16_t CCMR1_Out;
	volatile uint16_t CCMR1_In;
	volatile uint16_t CCMR2_Out;
	volatile uint16_t CCMR3_In;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t RCR;
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	volatile uint32_t BDTR;
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
	volatile uint32_t OR1;
	volatile uint32_t CCMR3_Output;
	volatile uint32_t CCR5;
	volatile uint32_t CCR6;
	volatile uint32_t OR2;
	volatile uint32_t OR3;
} tim1_t;

#define RCC_APB2_ENR 	((rcc_apb2enr_t *) (RCC_BASE_ADDRESS + APB2ENR_OFFSET))
#define TIM1_REG 		((tim1_t *) TIM1_BASE_ADDRESS)



extern volatile uint32_t gSysTick;

uint32_t timers_init(uint32_t ms);


#endif /* TIMERS_H_ */
