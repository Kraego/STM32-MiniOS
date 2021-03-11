/*
 * usercode.c
 *
 *  Created on: 24 Feb 2021
 *      Author: tkrax
 */
#include <stdio.h>
#include <stdbool.h>
#include "stm32l476g_discovery.h"
#include "debug.h"
#include "scheduler.h"

void flashGreenThreadForever() {
	while (true) {
		BSP_LED_Toggle(LED_GREEN);
		scheduler_sleep(250);
	}
}

void flashRedThreadForever() {
	while (true) {
		BSP_LED_Toggle(LED_RED);
		scheduler_sleep(250);
	}
}

void printSome() {
	DEBUG_PRINTF("Test 1 ... ");
	DEBUG_PRINTF("Test 2 ... ");
	DEBUG_PRINTF("Test 3 ... ");
	DEBUG_PRINTF("Test 4 ... ");
	DEBUG_PRINTF("Test 5 ... ");
}

void runExample() {
	scheduler_init();
	scheduler_startThread(&printSome);
	scheduler_startThread(&flashGreenThreadForever);
	scheduler_startThread(&flashRedThreadForever);
}
