/*
 * scheduler.c
 *
 *  Created on: 24 Feb 2021
 *      Author: kraego
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <cmsis_gcc.h> // for intrinsics (__x functions)
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include "debug.h"
#include "timers.h"
#include "scheduler.h"
#include "private/atomic.h"
#include "private/scheduler_private.h"

#define THREAD_ID_INVALID	(-1)
#define MAX_THREADS			(20)
#define TIMESLICE_MS		(10)

// local types
typedef enum {
	THREAD_NEW = 0, THREAD_READY = 1, THREAD_RUNNING = 2, THREAD_SLEEPING = 3, THREAD_BLOCKED = 4, THREAD_DONE = 5
} thread_state;

typedef struct {
	thread_state state;		// threadstate
	jmp_buf context; 		// TCB
	threadFunc threadFunc; 	// function to execute
	uint32_t sleepCount;	// sleeptime in ms
} t_thread;

/**
 * type for thread stack frame
 */
typedef struct {
	uint32_t space[256];
	uint32_t dummy;
	uint32_t lr_irq_dummy;

	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r12;
	uint32_t lr;
	uint32_t pc;
	uint32_t xpsr;
} t_stackFrame;

// Global stuff
static t_thread gThreads[MAX_THREADS] = { 0 };
static t_stackFrame gStackMap[MAX_THREADS] = { 0 };

static void updateThreads() {
	threadID index = MAX_THREADS;

	while (index--) {
		if (gThreads[index].state == THREAD_SLEEPING) {
			gThreads[index].sleepCount -= TIMESLICE_MS;
			if (gThreads[index].sleepCount == 0) {
				gThreads[index].state = THREAD_READY;
			}
		}
	}
}

static threadID getNextThread() {
	updateThreads();
	threadID currentId = gRunningThread;

	while (gThreads[currentId].state != THREAD_READY) {
		currentId = (currentId + 1) % MAX_THREADS;
		if (currentId == gRunningThread) {
			break;
		}
	}

	return currentId;
}

static uint32_t getFreeThreadSlot(threadID *newThread) {
	threadID id = MAX_THREADS;

	while (id--) {
		if (gThreads[id].state == THREAD_DONE || gThreads[id].state == THREAD_NEW) {
			*newThread = id;
			return 0;
		}
	}
	return -1;
}

static void scheduler_runNextThread() {
	ATOMIC_START();
	threadID nextThread = getNextThread();

	switch (gThreads[nextThread].state) {
	case THREAD_RUNNING:
		break; // already running
	case THREAD_READY:
		if (setjmp(gThreads[gRunningThread].context) == 0) { // jump out if longjmp called with 1
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

__attribute__((always_inline)) inline static void yield() {
	NVIC_SetPendingIRQ(TIM1_CC_IRQn);
}

static void killThread() {
	ATOMIC_START();
	DEBUG_PRINTF("Finishing Thread with id: %d", gRunningThread);
	gThreads[gRunningThread].state = THREAD_DONE;
	yield();
	ATOMIC_END();
}

static uint32_t* allocateStack(threadID id, void *threadFunction) {
	t_stackFrame *stack = &gStackMap[id];
	stack->xpsr = (1 << 24);				// special function register: value reverse engineered with debugger
	stack->pc = (uint32_t) threadFunction;	// set program counter to thread function
	stack->lr = (uint32_t) killThread; 		// function called at end of thread
	stack->r12 = 0x0000000C;
	stack->r3 = 0x00000003;
	stack->r2 = 0x00000002;
	stack->r1 = 0x00000001;
	stack->r0 = 0x0;

	stack->lr_irq_dummy = 0xFFFFFFF9; //LR - Return to Thread mode using MSP...main stack pointer
	stack->dummy = 0xDEADBEEF;
	return &stack->dummy;
}

static void idle() {
	while (true)
		;
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
 * @param duration_ms time in ms
 */
void scheduler_sleep(uint32_t duration_ms) {
	ATOMIC_START();
	gThreads[gRunningThread].sleepCount = duration_ms;
	gThreads[gRunningThread].state = THREAD_SLEEPING;
	ATOMIC_END();
	yield();
}

/**
 * Start given function as own thread
 *
 * @param tFunc function to run as thread
 */
void scheduler_startThread(threadFunc tFunc) {
	ATOMIC_START();
	threadID newThread = THREAD_ID_INVALID;
	uint32_t ret = getFreeThreadSlot(&newThread);

	if (ret != 0) {
		DEBUG_PRINTF("Thread init failed already %i threads defined\n", MAX_THREADS);
		return;
	}
	DEBUG_PRINTF("Started Thread with id: %d", newThread);
	gThreads[newThread].state = THREAD_READY;
	gThreads[newThread].threadFunc = tFunc;
	uint32_t msp = __get_MSP(); // backup main stack pointer

	__set_MSP((uint32_t) allocateStack(newThread, tFunc));

	if (setjmp(gThreads[newThread].context) == 0) {
		/*
		 * current context saved jump out
		 * keep old stack Pointer
		 */
		__set_MSP(msp);
	} else {
		/*
		 * running here as active Thread
		 * r13 = SP (Stack pointer)
		 */
		__ASM volatile ("add r7, r13, #0");
		__ASM volatile ("sub r7, #32");
	}
	ATOMIC_END();
}

/**
 * Initialise scheduler
 */
void scheduler_init(void) {
	DEBUG_PRINTF("Start scheduler!");
	gRunningThread = (MAX_THREADS - 1);
	scheduler_startThread(&idle);
	/*
	 * set to running to resume main thread when entering
	 * runNextThread
	 */
	gThreads[MAX_THREADS - 1].state = THREAD_RUNNING;
	timers_init(TIMESLICE_MS);
}

/**
 * Block current Thread
 */
void scheduler_blockThread(){
	gThreads[gRunningThread].state = THREAD_BLOCKED;
	yield();
}

/**
 * Resume thread
 *
 * @param thread to resume
 */
void scheduler_unblockThread(threadID thread){
	gThreads[thread].state = THREAD_READY;
}
