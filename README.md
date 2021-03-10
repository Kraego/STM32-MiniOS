# STM32L476_MiniOS
Demonstration: Wow to implement a preemptive multitasking OS on embedded devices.

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

* this is a demonstration how to create a preemptive multitasking system which can run threads (using setjmp/longjmp, and a timer for preemptive scheduling)

## What is it not?

* a implementation to use in production. If you need preemptive multithreading use RTOS or something similar.

## About

* OS Implementation:
  * Preemptive Scheduling: Usercode/Concurrency/scheduler.c
  * Demo Usage: Usercode/usercode.c   
