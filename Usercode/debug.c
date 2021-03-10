/*
 * debug.c
 *
 *  Created on: Jan 10, 2021
 *      Author: tkrax
 */
#include "debug.h"
#include <string.h>
#include <stdio.h>

static UART_HandleTypeDef *_huart = NULL;

/**
 * Initialze debug facility.
 *
 * @param huart handle to uart using as output
 */
void debug_Init(UART_HandleTypeDef *huart) {
	_huart = huart;
}

/**
 * Print debug information. Can be enabled/disabled via swith in debug.h
 *
 * @param fmt format string
 * @param ... arglist of format string
 */
void debug_Println(const char *fmt, ...) {
	char buffer[256];

	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	HAL_UART_Transmit(_huart, (uint8_t*) buffer, strlen(buffer), 100);
	va_end(args);
	char newline[2] = "\r\n";
	HAL_UART_Transmit(_huart, (uint8_t*) newline, 2, 10);
}
