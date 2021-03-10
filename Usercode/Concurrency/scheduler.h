/*
 * scheduler.h
 *
 *  Created on: 24 Feb 2021
 *      Author: tkrax
 */
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>

typedef void (*threadFunc)();

void scheduler_init(void);
void scheduler_sleep(uint32_t sleepCount);
void scheduler_startThread(threadFunc tFunc);

#endif /* SCHEDULER_H_ */
