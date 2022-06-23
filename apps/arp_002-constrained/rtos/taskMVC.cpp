/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- FreeRTOS system -----------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "os.h"
#include "semphr.h"

/* Includes -- FreeRTOS app --------------------------------------------------*/
#include "taskLog.h"
#include "taskSystem.h"
#include "globals.h"
#include "commands.h"

/* Includes -- modules -------------------------------------------------------*/
#include "logging.h"

/* References ----------------------------------------------------------------*/

#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
Logging mvcDebug;
//#define localPrintf consoleDebug.printf

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif

//Send output to two devices
static void localPrintf(const char* fmt, ...)
{
    //Setup
    va_list args;
    va_start(args, fmt);
    //Send to serial
    if(mvcDebug.pVPrintf)
    {
        mvcDebug.pVPrintf(fmt, args);
    }
    va_end(args);
}

/* Functions -----------------------------------------------------------------*/
extern "C" void taskMVCStart(void * argument)
{
#ifdef USE_LOGGING
    mvcDebug.setStamp("MVC", 3);
    mvcDebug.setMode(LOG_MODE_DEFAULT);
#endif
    localPrintf("MVC task started.\n");
    while(1)
    {
        EventBits_t uxBits = xEventGroupGetBits( xTestEventGroup );
        if(!(uxBits & 0x08))
        {
            controller.tick();
        }
        vTaskDelay( 20 );
    }
}

