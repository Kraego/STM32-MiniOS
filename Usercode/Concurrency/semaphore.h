/*
 * semaphore.h
 *
 *  Created on: 13 Mar 2021
 *      Author: kraego
 */

#ifndef PRIVATE_SEMAPHORE_H_
#define PRIVATE_SEMAPHORE_H_

#define SEM_ID_INVALID ((semID) -1)

typedef enum {
	EMPTY = 0, FULL = 1
} sem_state;

typedef uint32_t semID;

semID semaphore_create(uint32_t count, sem_state state);
void semaphore_take(semID sem);
void semaphore_give(semID sem);

#endif /* PRIVATE_SEMAPHORE_H_ */
