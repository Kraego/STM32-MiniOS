/*
 * usercode.c
 *
 *  Created on: 24 Feb 2021
 *      Author: kraego
 */
#include <stdio.h>
#include <stdbool.h>
#include "stm32l476g_discovery.h"
#include "debug.h"
#include "semaphore.h"
#include "scheduler.h"

#define SEM_DEMO

#ifdef SEM_DEMO
semID gSem = SEM_ID_INVALID;
#endif

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

/**
 * Just threading demonstration
 */
void runExampleBasic() {
	scheduler_init();
	scheduler_startThread(&printSome);
	scheduler_startThread(&flashGreenThreadForever);
	scheduler_startThread(&flashRedThreadForever);
}

/**
 * Print long - blocking other thread via Semaphore gSem
 */
void printLong(){
	uint32_t repeats = 10;

	while(repeats--)
	{
		DEBUG_PRINTF("Repeat: %d", repeats);
		scheduler_sleep(100);
	}
#ifdef SEM_DEMO
	semaphore_give(gSem);
#endif
}

/**
 * Block until Semaphore gSem is given back
 */
void flashGreenThreadFoverBlocked() {
#ifdef SEM_DEMO
		semaphore_take(gSem);
#endif
	while (true) {
		BSP_LED_Toggle(LED_GREEN);
		scheduler_sleep(250);
	}
}

/**
 *	Threading with sync over semaphores
 *
 *	to see difference change SEM_DEMO:
 *		* if SEM_DEMO is DEFINED: the green LED toggle starts when printVeryLong() is done with printing
 */
void runExample() {
	scheduler_init();

#ifdef SEM_DEMO
	// Semaphore is used as BINARY Semaphore
	gSem = semaphore_create(1, EMPTY);

	if (gSem == SEM_ID_INVALID)
	{
		DEBUG_PRINTF("Unable to create semaphore");
		return;
	}
#endif
	scheduler_startThread(&printLong);
	scheduler_startThread(&flashGreenThreadFoverBlocked);
}
