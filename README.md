<!-- omit in toc -->
# STM32L476_MiniOS

Demonstration: How to implement a preemptive multitasking OS on embedded devices.

**Contribution:**
- [Contribution](#contribution)
- [Development](#development)
- [What is this about?](#what-is-this-about)
- [What is it not?](#what-is-it-not)
- [How to port this to another platform?](#how-to-port-this-to-another-platform)
- [About](#about)

## Contribution
* If you want to port this implementation fork me
* If you have improvements at your hand send me an PR

## Development
* Hardware: STM32L476G-DISCO Board
* For development the STM32Cube IDE is used
  * gcc 
* To get a highlevel API the Board BSP from ST is used
* Debug Printouts on UART2:
  * can be activated via switch in *Usercode/debug.h* or with compiler define.
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
* Portable to other platforms (Stackframe definition specific for ARM Cortex-M4)

## How to port this to another platform?
1. You need everything in *Usercode/Concurrency*
2. remove or implement methods according to Usercode/debug.c
3. Adaptions in *scheduler.c*
   1. redefine *t_stackFrame* according to your platform
   2. allocate stackframe correct in *allocateStack*
   3. change this to match your platform
      1. if you not using *cmsis_gcc.h* find out how to set/get the stackpointer
      2. yield ... exit pending interrupt
      3. *void TIM1_CC_IRQHandler()* - this is the timer interupt for preemptive scheduling
      4. *timers_init()* - creating a timer which calls the the timer interrupt at a rate of *TIMESLICE_MS*

## About

* OS Implementation:
  * **Preemptive Scheduling:** Usercode/Concurrency/scheduler.c
  * **Synchronisation Mechanism via counting semaphores:** Usercode/Concurrency/semaphore.c
  * Demo Usage: Usercode/usercode.c
    * To enable/disable synchronization change #define SEM_DEMO at start of usercode.c
* Basic Usage Example - printout stuff to console + green and red LED should flash in parallel forever:
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
