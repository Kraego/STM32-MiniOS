/*
 * debug.h
 *
 * Debugging Helper
 *
 *  Created on: Jan 10, 2021
 *      Author: tkrax
 */

#ifndef UTILS_DEBUG_H_
#define UTILS_DEBUG_H_

#include <stdarg.h>
#include "stm32l4xx_hal.h"

#define DEBUG_PRINT

#ifdef DEBUG_PRINT
void debug_Init(UART_HandleTypeDef *_huart);
void debug_Println(const char *fmt, ...);
	#define DEBUG_INIT(huart) 			debug_Init(huart)
	#define DEBUG_PRINTF(fmt, args...) 	debug_Println(fmt, ## args)
#else
	#define DEBUG_PRINTF(fmt, args...)
	#define DEBUG_INIT(huart)
#endif

#endif /* UTILS_DEBUG_H_ */
