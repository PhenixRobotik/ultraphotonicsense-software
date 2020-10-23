// Inspired from FreeRTOS/portable/MemMang/heap_4.c

#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE
#include "FreeRTOS.h"
#include "task.h"
#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#if( configSUPPORT_DYNAMIC_ALLOCATION == 0 )
	#error This file must not be used if configSUPPORT_DYNAMIC_ALLOCATION is 0
#endif

#include <stdint.h>
#include <o1heap.h>

/* Allocate the memory for the heap. */
#if( configAPPLICATION_ALLOCATED_HEAP == 1 )
	/* The application writer has already defined the array used for the RTOS
	heap - probably so it can be placed in a special segment or address. */
	extern uint8_t ucHeap[configTOTAL_HEAP_SIZE];
#else
	static uint8_t ucHeap[configTOTAL_HEAP_SIZE];
#endif /* configAPPLICATION_ALLOCATED_HEAP */


O1HeapInstance *heap_inst = NULL;

void *pvPortMalloc( size_t xWantedSize )
{
    void *pvReturn = NULL;
    
    vTaskSuspendAll();
	{
        if(__builtin_expect(heap_inst == NULL, 0)){
            heap_inst = o1heapInit(ucHeap, sizeof(ucHeap), NULL, NULL);
        }
        
        pvReturn = o1heapAllocate(heap_inst, xWantedSize);
        
        traceMALLOC( pvReturn, xWantedSize );
    }
	( void ) xTaskResumeAll();
    
    #if( configUSE_MALLOC_FAILED_HOOK == 1 )
	{
		if( pvReturn == NULL )
		{
			extern void vApplicationMallocFailedHook( void );
			vApplicationMallocFailedHook();
		}
	}
	#endif

	configASSERT( ( ( ( size_t ) pvReturn ) & ( size_t ) portBYTE_ALIGNMENT_MASK ) == 0 );
	return pvReturn;
}

void vPortFree( void *pv )
{
    vTaskSuspendAll();
    {
        o1heapFree(heap_inst, pv);
    }
	( void ) xTaskResumeAll();
}
