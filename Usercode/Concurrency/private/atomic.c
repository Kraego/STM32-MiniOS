/*
 * atomic.c
 *
 *  Created on: 13 Mar 2021
 *      Author: kraego
 */
#include <stdint.h>
#include <cmsis_gcc.h>

void ATOMIC_START() {
	__disable_irq();
}

void ATOMIC_END() {
	__enable_irq();
}
