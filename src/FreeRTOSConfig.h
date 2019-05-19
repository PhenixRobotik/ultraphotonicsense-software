/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* Don't forget to include hardware header file for the cpu clock */
#include "hardware.h"
#include <stdint.h>
/* TODO :
 * Process a DEBUG define to enable/disable some debugging
 * options
 */

// Preemptive scheduler
#define configUSE_PREEMPTION			1
// Optimised task selection for the architecture
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
// Low power tickless mode, disable if it causes problems
#define configUSE_TICKLESS_IDLE                 0

#define configCPU_CLOCK_HZ			( SYSTEM_CORE_CLOCK )
//Time resolution and scheduling frequency
#define configTICK_RATE_HZ			(( TickType_t ) SYSTICK_FREQ_HZ )

// Maximum number of available priorities, must be <= 32 for optimised
// task selection
#define configMAX_PRIORITIES			( 5 )
// Stack size, in 4-byte words
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 128 )
#define configMAX_TASK_NAME_LEN			( 16 )
// Useless on a 32bit architecture
#define configUSE_16_BIT_TICKS			0
// Define tasks at the idle priority (lowest) can share their time
// slice with the idle task or not (read the doc)
#define configIDLE_SHOULD_YIELD                 1
// Fast Wait/Notify mechanism, enabled by default
#define configUSE_TASK_NOTIFICATIONS            1
#define configUSE_MUTEXES                       1
// A recursive mutex is a mutex that can be taken multiple times by the
// same task
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
// Set to 0 to disable deprecated queue functions
#define configUSE_ALTERNATIVE_API               0
// Number of maximum registered queues for a RTOS kernel aware
// debugger
#define configQUEUE_REGISTRY_SIZE		8
// Using queue sets enable a task to block on a set of queues
#define configUSE_QUEUE_SETS                    0
// Share the time between tasks of the same priority
#define configUSE_TIME_SLICING                  1
// Add support for reentrant newlib functions
#define configUSE_NEWLIB_REENTRANT              0
// Disable obsolete FreeRTOS names (version < 8.0.0)
#define configENABLE_BACKWARD_COMPATIBILITY     0
// Thread local storage pointers are used for thread's variables, an
// alternative to global variables in a single-threaded program
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 0
// Define the stack word type
//#define configSTACK_DEPTH_TYPE                ( UBaseType_t )
// Define the message buffer length type
//#define configMESSAGE_BUFFER_LENGTH_TYPE        ( uint8_t )


/* Memory Allocation */
// If used, vApplicationGetIdleTaskMemory/TimerTaskMemory must be defined
#define configSUPPORT_STATIC_ALLOCATION         0
#define configSUPPORT_DYNAMIC_ALLOCATION        1
// Maximum usage for the memory allocation schemes (heap*.c), in bytes
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 8 * 1024 ) )
// Can be used to specify the heap size and location manually
#define configAPPLICATION_ALLOCATED_HEAP        0

/* Hook functions */
// Doc : https://www.freertos.org/a00016.html
// To run a function at each cycle of the idle task
#define configUSE_IDLE_HOOK			0
// Call vApplicationTickHook at each tick interrupt
#define configUSE_TICK_HOOK			1
// Check for stack overflow at context switch (causes overhead)
// Call vApplicationStackOverflowHook on detection
#define configCHECK_FOR_STACK_OVERFLOW          2
// Call vApplicationMallocFailedHook if malloc fails
#define configUSE_MALLOC_FAILED_HOOK	        0
// Call vApplicationDaemonTaskStartupHook when RTOS is started
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0


/* Run time and task stats gathering related definitions. */
// Enable runtime statistics of cpu usage by tasks
#define configGENERATE_RUN_TIME_STATS           0
// Include structure members for execution visualisation and tracing
// improvements
#define configUSE_TRACE_FACILITY                0
// Include vTaskList() and vTaskGetRunTimeStats() (goes with
// configUSE_TRACE_FACILITY)
#define configUSE_STATS_FORMATTING_FUNCTIONS    0


/* Co-routine related definitions. */
// Co-routines are lightweight tasks, useful for very small
// processors
#define configUSE_CO_ROUTINES 		        0
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )

/* Software timer related definitions. */
// Enable software timers
#define configUSE_TIMERS		        1
// Should be high enough to process timer requests with correct
// delays.  1 still allow a task to have a higher priority. Delaying a
// request does not mess with timer values.
#define configTIMER_TASK_PRIORITY	        ( 1 )
// Maximum number of unprocessed timer commands at a time
#define configTIMER_QUEUE_LENGTH	        10
// Stack size for the timer task. Callback functions execute on the
// timer task context.
#define configTIMER_TASK_STACK_DEPTH	        ( configMINIMAL_STACK_SIZE )

/* Interrupt nesting behaviour configuration. */
/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4        /* 15 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 0xf

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 	( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5

/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
	
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }	

// Enable vTaskSetApplicationTaskTag
#define configUSE_APPLICATION_TASK_TAG	                0

// Disable priviledged functions (they are unsafe for release)
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet	1
#define INCLUDE_uxTaskPriorityGet	1
#define INCLUDE_vTaskDelete		1
#define INCLUDE_vTaskCleanUpResources	1
#define INCLUDE_vTaskSuspend		1
#define INCLUDE_vTaskDelayUntil		1
#define INCLUDE_vTaskDelay		1






	
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */

