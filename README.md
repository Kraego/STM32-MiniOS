<!-- omit in toc -->
# STM32L476_MiniOS

Demonstration: How to implement a preemptive multitasking OS on embedded devices.

**Content:**
- [Development](#development)
- [What is this about?](#what-is-this-about)
- [What is it not?](#what-is-it-not)
- [About](#about)

## Development
* Hardware: STM32L476G-DISCO Board
* For development the STM32Cube IDE is used
* To get a highlevel API the Board BSP from ST is used
* Debug Printouts on UART2:
  * can be activated via switch in *Usercode/Utils/debug.h*
  ```
  #define DEBUG_PRINT
  ```
  * Setting: 
    * Baudrate: 9600
    * Databits: 8
    * Stopbits: 1
    * Parity: None

## What is this about?

* This is a demonstration how to create a preemptive multitasking system which can run threads (using setjmp/longjmp, and a timer for preemptive scheduling)

## What is it not?

* A implementation to use in production. If you need preemptive multithreading use RTOS or something similar.

## About

* OS Implementation:
  * Preemptive Scheduling: Usercode/Concurrency/scheduler.c
  * Demo Usage: Usercode/usercode.c   
* Usage Example - green and red LED should flash in parallel:
``` C
void flashGreenThreadForever() {
	while (true) {
		BSP_LED_Toggle(LED_GREEN);
		scheduler_sleep(50);
	}
}

void flashRedThreadForever() {
	while (true) {
		BSP_LED_Toggle(LED_RED);
		scheduler_sleep(50);
	}
}

void printSome() {
	DEBUG_PRINTF("Test 1 ... ");
	DEBUG_PRINTF("Test 2 ... ");
	DEBUG_PRINTF("Test 3 ... ");
	DEBUG_PRINTF("Test 4 ... ");
	DEBUG_PRINTF("Test 5 ... ");
}

void runExample() {
	scheduler_init();
	scheduler_startThread(&printSome);
	scheduler_startThread(&flashGreenThreadForever);
	scheduler_startThread(&flashRedThreadForever);
}
```
