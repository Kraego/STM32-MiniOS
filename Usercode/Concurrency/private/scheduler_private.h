/*
 * scheduler_private.h
 *
 *  Created on: 13 Mar 2021
 *      Author: kraego
 */

#ifndef PRIVATE_SCHEDULER_PRIVATE_H_
#define PRIVATE_SCHEDULER_PRIVATE_H_

typedef uint32_t threadID;

threadID gRunningThread;

void scheduler_blockThread();
void scheduler_unblockThread(threadID thread);

#endif /* PRIVATE_SCHEDULER_PRIVATE_H_ */
