/*
 * scheduler.c
 *
 *  Created on: 24 Feb 2021
 *      Author: tkrax
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <cmsis_gcc.h>
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include "timers.h"
#include "scheduler.h"

#define MAX_THREADS			(4)
#define TIMESLICE_MS		(10)

threadID gRunningThread = (MAX_THREADS - 1);
t_thread gThreads[MAX_THREADS] = { };
static t_stackFrame gStackMap[MAX_THREADS] = { };

static void ATOMIC_START() {
	__disable_irq();
}

static void ATOMIC_END() {
	__enable_irq();
}

static void scheduler_UpdateThreads() {
	threadID index = MAX_THREADS;

	while (index--) {
		if (gThreads[index].state == THREAD_SLEEPING) {
			gThreads[index].sleepCount--;
			if (gThreads[index].sleepCount == 0) {
				gThreads[index].state = THREAD_READY;
			}
		}
	}
}

/**
 * Determine next Thread (round Robin)
 *
 * @return threadID of next Thread to run
 */
static threadID scheduler_nextThread() {
	scheduler_UpdateThreads();
	threadID currentId = gRunningThread;

	while (gThreads[currentId].state != THREAD_READY) {
		currentId = (currentId + 1) % MAX_THREADS;
		if (currentId == gRunningThread) {
			break;
		}
	}

	return currentId;
}

/**
 * Get free thread slot if one is free
 *
 * @param newThread slot for new thread
 * @return 0 if succeeded, -1 if there is no free slot
 */
static threadID scheduler_getFreeThreadSlot(threadID *newThread) {
	threadID id = MAX_THREADS;

	while (id--) {
		if (gThreads[id].state == THREAD_DONE) {
			*newThread = id;
			return 0;
		}
		if (gThreads[id].state == THREAD_NEW) {
			*newThread = id;
			return 0;
		}
	}
	return -1;
}

/**
 * Called by timer interrupt (Time slice)
 */
static void scheduler_runNextThread() {
	ATOMIC_START();
	threadID nextThread = scheduler_nextThread();

	switch (gThreads[nextThread].state) {
	case THREAD_RUNNING:
		break; // already running
	case THREAD_READY:
		if (setjmp(gThreads[gRunningThread].context) == 0) { // jump out if longjump called with 1
			//	saved current execution state
			if (gThreads[gRunningThread].state == THREAD_RUNNING) {
				gThreads[gRunningThread].state = THREAD_READY;
			}
			gRunningThread = nextThread;
			gThreads[gRunningThread].state = THREAD_RUNNING;
			longjmp(gThreads[gRunningThread].context, 1); // load context of next thread
		}
		break;
	default:
		break; // no thread to run --> sleep no atomic
	}
	ATOMIC_END();
}

__attribute__((always_inline)) inline void scheduler_yield() {
	NVIC_SetPendingIRQ(TIM1_CC_IRQn);
}

static void scheduler_killThread() {
	gThreads[gRunningThread].state = THREAD_DONE;
	scheduler_yield();
}

static uint32_t* scheduler_allocateStack(threadID id, void *threadFunction) {
	t_stackFrame *stack = &gStackMap[id];
	stack->xpsr = (1 << 24);
	stack->pc = (uint32_t) threadFunction;
	stack->lr = (uint32_t) scheduler_killThread;
	stack->r12 = 0x0000000C;
	stack->r3 = 0x00000003;
	stack->r2 = 0x00000002;
	stack->r1 = 0x00000001;
	stack->r0 = 0x0;

	stack->lr_irq_dummy = 0xFFFFFFF9; //LR
	stack->dummy = 0xDEADBEEF;
	return &stack->dummy;
}

void TIM1_CC_IRQHandler() {
	// Clear pending bit
	tim1_t *timerConfig = TIM1_REG;
	timerConfig->SR &= ~(1 << (1));
	scheduler_runNextThread();
}

/**
 * Sleep for given ms
 *
 * @param sleepCount multiples of 10 ms
 */
void scheduler_sleep(uint32_t sleepCount) {
	ATOMIC_START();
	gThreads[gRunningThread].sleepCount = sleepCount;
	gThreads[gRunningThread].state = THREAD_SLEEPING;
	ATOMIC_END();
	scheduler_yield();
}

void scheduler_startThread(threadFunc tFunc) {
	ATOMIC_START();
	threadID newThread = THREAD_ID_INVALID;
	uint32_t ret = scheduler_getFreeThreadSlot(&newThread);

	if (ret != 0) {
		printf("Thread init failed already %i threads defined\n", MAX_THREADS);
		return;
	}

	gThreads[newThread].state = THREAD_READY;
	gThreads[newThread].threadFunc = tFunc;
	uint32_t msp = __get_MSP();

	__set_MSP((uint32_t) scheduler_allocateStack(newThread, tFunc));

	if (setjmp(gThreads[newThread].context) == 0) { // current context saved jump out
		__set_MSP(msp); // keep old stack Pointer
	} else { // running here as active Thread
		__ASM volatile ("add r7, r13, #0"); 	//r13 = sp
		__ASM volatile ("sub r7, #32");
	}
	ATOMIC_END();
}

static void idle() {
	while (true)
		;
}

/**
 * Initialise scheduler
 *
 * @return 0 if init done, 1 if returning from thread
 */
void scheduler_init(void) {
	scheduler_startThread(&idle);
	gThreads[MAX_THREADS - 1].state = THREAD_RUNNING;
	timers_init(TIMESLICE_MS);
}
