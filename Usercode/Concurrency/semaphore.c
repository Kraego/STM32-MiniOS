/*
 * semaphore.c
 *
 *  Created on: 13 Mar 2021
 *      Author: kraego
 */
#include <stdint.h>
#include "semaphore.h"
#include "private/atomic.h"
#include "private/scheduler_private.h"

#define MAX_SEMS	(10)

typedef struct {
	semID id;
	uint32_t count;
	uint32_t blockedCount;
	threadID blockedThreads[MAX_SEMS];
} t_semaphore;

static uint32_t gSemcount = 0;
static t_semaphore gSemaphores[MAX_SEMS] = { 0 };

/**
 * Creates an counting semaphore
 *
 * @param count of semaphore
 * @param state full or empty
 * @return returns a semaphore id if successful otherwise SEM_ID_INVALID is returned
 */
semID semaphore_create(uint32_t count, sem_state state) {
	if (gSemcount >= MAX_SEMS) {
		return SEM_ID_INVALID;
	}
	uint32_t semId = gSemcount;
	gSemcount++;
	gSemaphores[semId].id = semId;
	gSemaphores[semId].count = state == FULL ? MAX_SEMS : 0;
	return semId;
}

/**
 * Try to take semaphore, blocks if no semaphore is available
 *
 * @param sem the id of the semaphore
 */
void semaphore_take(semID sem) {
	ATOMIC_START();

	if (gSemaphores[sem].count > 0) {
		gSemaphores[sem].count--;
		ATOMIC_END();
	} else {
		gSemaphores[sem].blockedThreads[gSemaphores[sem].blockedCount] = gRunningThread;
		gSemaphores[sem].blockedCount++;
		ATOMIC_END();
		scheduler_blockThread();
	}
}

/**
 * Give semaphore back
 *
 * @param sem the id of the semaphore
 */
void semaphore_give(semID sem) {
	ATOMIC_START();

	if (gSemaphores[sem].blockedCount > 0) {
		scheduler_unblockThread(gSemaphores[sem].blockedThreads[0]);
		gSemaphores[sem].blockedCount--;
	}
	gSemaphores[sem].count = (gSemaphores[sem].count + 1) % MAX_SEMS;
	ATOMIC_END();
}
