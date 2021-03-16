/*
 * queue.c
 *
 *  Created on: 16 Mar 2021
 *      Author: kraego
 */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/**
 * Initialize a new queue on heap
 *
 * @param (out) queue the queue
 * @param capacity of the queue
 * @return 0 if successful, otherwise -1 or -2 if memory problems occur
 */
uint32_t queue_init(t_Queue **queue, uint32_t capacity) {
	*queue = (t_Queue *) malloc(sizeof(t_Queue));

	if (*queue == NULL){
		return -1;
	}

	(*queue)->capacity = capacity;
	(*queue)->front = 0;
	(*queue)->rear = 0;
	(*queue)->queue = calloc(capacity, sizeof(uint32_t));
	return ((*queue)->queue == NULL) ? -2 : 0;
}

/**
 * Append data to rear of queue
 *
 * @param data to append
 * @return 0 if successful, -1 otherwise (overflow)
 */
uint32_t queue_enqueue(t_Queue *queue, uint32_t data) {
	if (queue->capacity == queue->rear) {
		return -1;
	} else {
		queue->queue[queue->rear] = data;
		queue->rear++;
	}
	return 0;
}

/**
 * Remove first
 *
 * @return 0 if successful, -1 otherwise (empty queue)
 */
uint32_t queue_dequeue(t_Queue *queue) {
	if (queue->front == queue->rear) {
		return -1;
	} else {
		for (uint32_t i = 0; i < queue->rear - 1; i++) {
			queue->queue[i] = queue->queue[i + 1];
		}
		queue->rear--;
	}
	return 0;
}

/**
 * Delete queue
 *
 * @param queue to delete
 */
void queue_delete(t_Queue *queue) {
	free(queue->queue);
	free(queue);
}
