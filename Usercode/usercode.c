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
		scheduler_sleep(50);
	}
}

void flashRedThreadForever() {
	while (true) {
		BSP_LED_Toggle(LED_RED);
		scheduler_sleep(50);
	}
}

void printSome() {
	DEBUG_PRINTF("Print 1 ... ");
	DEBUG_PRINTF("Print 2 ... ");
	DEBUG_PRINTF("Print 3 ... ");
	DEBUG_PRINTF("Print 4 ... ");
	DEBUG_PRINTF("Print 5 ... ");
}

void runExample() {
	DEBUG_PRINTF("Start scheduler!");
	DEBUG_PRINTF("Start Main thread");
	scheduler_init();
	// scheduler_startThread(&printSome);
	scheduler_startThread(&flashGreenThreadForever);
	scheduler_startThread(&flashRedThreadForever);
}
