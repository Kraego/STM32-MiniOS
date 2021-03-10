/*
 * scheduler.h
 *
 *  Created on: 24 Feb 2021
 *      Author: tkrax
 */
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>
#include <setjmp.h>

#define THREAD_ID_INVALID	(-1)

typedef uint32_t threadID;

typedef enum {
	THREAD_NEW = 0, THREAD_READY = 1, THREAD_RUNNING = 2, THREAD_SLEEPING = 3, THREAD_BLOCKED = 4, THREAD_DONE = 5
} thread_state;

typedef void (*threadFunc)();

typedef struct {
	thread_state state;		// Threadstate
	jmp_buf context; 		// TCB
	threadFunc threadFunc; 	// Function to execute
	uint32_t sleepCount;	// sleeptime in ms
} t_thread;

typedef struct {
	uint32_t space[128];
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
}t_stackFrame;

void scheduler_init(void);
void scheduler_sleep(uint32_t sleepCount);
void scheduler_startThread(threadFunc tFunc);

#endif /* SCHEDULER_H_ */
