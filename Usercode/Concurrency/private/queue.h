/*
 * queue.h
 *
 *  Created on: 16 Mar 2021
 *      Author: kraego
 */

#ifndef PRIVATE_QUEUE_H_
#define PRIVATE_QUEUE_H_

#include <stdint.h>

typedef struct {
	uint32_t capacity;
	uint32_t front;
	uint32_t rear;
	uint32_t *queue;
}t_Queue;

uint32_t queue_init(t_Queue **queue, uint32_t capacity);
uint32_t queue_enqueue(t_Queue *queue, uint32_t data);
uint32_t queue_dequeue(t_Queue *queue);
void queue_delete(t_Queue *queue);

#endif /* PRIVATE_QUEUE_H_ */
