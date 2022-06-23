/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

#include "os.h"

#include "taskLog.h"
#include "taskSystem.h"

/* Declare a variable to hold the created event group. */
EventGroupHandle_t xTestEventGroup;

extern void globalsInit(void);

//Tasks
extern void taskConsoleStart(void * argument);

extern void xPortSysTickHandler(void);

QueueHandle_t controlQueue = NULL;
SemaphoreHandle_t xSerCtrl;

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

void MX_FREERTOS_Init(void)
{
	//TODO: Should we check for scheduler running first?
	bspRegisterSysTickCallback(xPortSysTickHandler);

	// Example:
	//vStartLEDFlashTasks( mainFLASH_TASK_PRIORITY );
	//	xTaskCreate( vCompeteingIntMathTask1, "IntMath1", intgSTACK_SIZE, ( void * ) &( usTaskCheck[ 0 ] ), uxPriority, NULL );

	BaseType_t retVal;

	logQueue = xQueueCreate( 100, sizeof( strMsg_t* ) );
	controlQueue = xQueueCreate( 100, sizeof( gameControlInput_t* ) );

    /* Create a mutex type semaphore. */
    xSerCtrl = xSemaphoreCreateMutex();


    /* Attempt to create the event group. */
    xTestEventGroup = xEventGroupCreate();

    /* Was the event group created successfully? */
    if( xTestEventGroup == NULL )
    {
        /* The event group was not created because there was insufficient
        FreeRTOS heap available. */
		while(1);
    }

	retVal = xTaskCreate( taskLogStart, "log", 128, (void*) 1, tskIDLE_PRIORITY, NULL);
    if (retVal != pdPASS)
	{
		while(1);
	}

	retVal = xTaskCreate( taskConsoleStart, "console", 512, (void*) 1, 1, NULL);
    if (retVal != pdPASS)
	{
		while(1);
	}

	retVal = xTaskCreate( taskSystemStart, "system", 256, (void*) 1, 2, NULL);
    if (retVal != pdPASS)
	{
		while(1);
	}

}

void osInit(void)
{
	// Init other os objects
    globalsInit();

	
	MX_FREERTOS_Init();

	// Start os and don't come back
	vTaskStartScheduler();
}

#ifndef configCHECK_FOR_STACK_OVERFLOW
#error 2
#endif

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
