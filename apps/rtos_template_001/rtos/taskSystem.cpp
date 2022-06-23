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
#include "semphr.h"

/* Includes -- FreeRTOS app --------------------------------------------------*/
#include "globals.h"
#include "taskLog.h"

/* Includes -- modules -------------------------------------------------------*/
#include "logging.h"

/* References ----------------------------------------------------------------*/
#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
#define localPrintf logSystemLog.printf
Logging logSystemLog;

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif

extern SemaphoreHandle_t xSerCtrl;

extern "C" void taskSystemStart(void * argument)
{
#ifdef USE_LOGGING
	logSystemLog.setStamp("taskSystem", 7);
	logSystemLog.setMode(LOG_MODE_AUTO);
#endif
	while(1)// Smash it?
	{
        if(xSemaphoreTake( xSerCtrl, 1000 )) //try for 10 ticks
        {
			vTaskSuspendAll();
            //MIDI.read();
            //CtrlMIDI.read();
            xSemaphoreGive( xSerCtrl );
			xTaskResumeAll();
        }
        vTaskDelay( 1 );
        
	}
}
