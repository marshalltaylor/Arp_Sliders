/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- FreeRTOS system -----------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

/* Includes -- FreeRTOS app --------------------------------------------------*/
#include "taskLog.h"

/* Includes -- modules -------------------------------------------------------*/
#include "logging.h"

/* References ----------------------------------------------------------------*/
#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
#define localPrintf logTaskLog.printf
Logging logTaskLog;

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif

QueueHandle_t logQueue = NULL;

extern "C" void taskLogStart(void * argument)
{
#ifdef USE_LOGGING
	logTaskLog.setStamp("taskLog", 7);
	logTaskLog.setMode(LOG_MODE_AUTO);
#endif
	while(1)
	{
		strMsg_t * msg = NULL;
		while(xQueueReceive( logQueue, &msg, 0 ) == pdPASS)
		{
			//print output
			switch( msg->id )
			{
				case -1:
				{
					localPrintf("BANG!");
					break;
				}
				case 0:
				{
					localPrintf("msg data: %s\n", (const char *)msg->data);
					break;
				}
				default:
				{
					localPrintf("msg id: 0x%X, data: %s\n", msg->id, (const char *)msg->data);
					break;
				}
			}

			delete msg;
		}
		// Don't smash it!
		vTaskDelay( 50 );
	}
}
