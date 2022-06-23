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
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "event_groups.h"
#include "os.h"
#include "pool.h"

/* Includes -- FreeRTOS app --------------------------------------------------*/
//#include "taskLog.h"
//#include "taskCRT.h"
//#include "MidiClockDisplay.h"
//#include "globals.h"

/* Includes -- modules -------------------------------------------------------*/
//#include "logging.h"
//#include "CRTVideo.h"

/* References ----------------------------------------------------------------*/

//#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
//#define localPrintf consoleDebug.printf
Logging consoleDebug;

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif

void test_pool(void)
{
    Pool<uint16_t> unit(20);
    for(int i = 0; i < unit.sz; i++)
    {
        localPrintf("%d", unit.inUseRead(i));
    }
    localPrintf("\n");

    unit.inUseWrite(3, 1);
    unit.inUseWrite(4, 1);
    unit.inUseWrite(14, 1);
    localPrintf("Bits: ");
    for(int i = 0; i < unit.sz; i++)
    {
        localPrintf("%d", unit.inUseRead(i));
    }
    localPrintf("\n");
    
    //change something
    uint16_t * pThingA = unit.palloc();
    uint16_t * pThingB = unit.palloc();
    uint16_t * pThingC = unit.palloc();
    UNUSED(pThingA);
    UNUSED(pThingB);
    UNUSED(pThingC);

    //Floats not working
    localPrintf("Usage %d.%d\n", (uint16_t)unit.used(), (uint16_t)((unit.used() - ((uint16_t)unit.used()))* 10));//unit.used());

    //print again
    localPrintf("Bits: ");
    for(int i = 0; i < unit.sz; i++)
    {
        localPrintf("%d", unit.inUseRead(i));
    }
    localPrintf("\n");

    //palloc until full
    for(int i = 0; unit.palloc(); i++)
    {
        localPrintf("palloc, i=%d\n", i);
    }

    unit.free(pThingB);
    unit.free(pThingC);

    //print again
    localPrintf("Bits: ");
    for(int i = 0; i < unit.sz; i++)
    {
        localPrintf("%d", unit.inUseRead(i));
    }
    localPrintf("\n");

    //Floats not working
    localPrintf("Usage %d.%d\n", (uint16_t)unit.used(), (uint16_t)((unit.used() - ((uint16_t)unit.used()))* 10));//unit.used());

    //palloc until full
    for(int i = 0; unit.palloc(); i++)
    {
        localPrintf("palloc, i=%d\n", i);
    }
    
    //empty
    for(int i = 0; i < unit.sz; i++)
    {
        //This could be a private function to clear
        if(unit.free(&unit.poolData[i]))
        {
            localPrintf("free, i=%d\n", i);
        }
    }
    
    //Floats not working
    localPrintf("Usage %d.%d\n", (uint16_t)unit.used(), (uint16_t)((unit.used() - ((uint16_t)unit.used()))* 10));//unit.used());

}
