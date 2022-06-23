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
#include "debugKeyParser.h"
#include "commands.h"

/* Includes -- modules -------------------------------------------------------*/
#include "logging.h"

/* References ----------------------------------------------------------------*/

#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
Logging consoleDebug;
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
    if(consoleDebug.pVPrintf)
    {
        consoleDebug.pVPrintf(fmt, args);
    }
    va_end(args);
}

static comPortInterface_t console;

CommandTable cmdTable;

/* Functions -----------------------------------------------------------------*/
extern "C" void taskConsoleStart(void * argument)
{
#ifdef USE_LOGGING
    consoleDebug.setStamp("Console", 7);
    consoleDebug.setMode(LOG_MODE_DEFAULT);
#endif

    tSysGetSerialFunctions(&console);
    
    uint16_t cmdActivePtr = 0;
    
    for(cmdActivePtr = 0; (cmdActive[cmdActivePtr] != 0) && (cmdActivePtr < CMDBUFFERSIZE); cmdActivePtr++);
    
    //  Identify first real char
    int firstCharIndex = 0;
    for(firstCharIndex = 0; (cmdActive[firstCharIndex] == ' ') && (firstCharIndex < cmdActivePtr); firstCharIndex++)
    {
    }
    int argc = 0;
    if(firstCharIndex < cmdActivePtr)
    {
        argc = 1;
    }
    
    if(argc == 1)
    {
        //  Count instances of " <char>" in string and build args
        int i;
        for(i = firstCharIndex; i < (cmdActivePtr - 1); i++)
        {
            if((cmdActive[i] == ' ')&&(cmdActive[i + 1] != ' '))
            {
                argc++;
            }
            if(cmdActive[i] == ' ')
            {
                cmdActive[i] = 0x00;
            }
        }
        //localPrintf("arg count:%d\n", argc);
        
        char *argv[argc];
        argv[0] = &cmdActive[firstCharIndex];
        int index = 1;
        
        for(i = firstCharIndex; i < (cmdActivePtr - 1); i++)
        {
            if((cmdActive[i] == 0x00)&&(cmdActive[i + 1] != 0x00))
            {
                argv[index] = &cmdActive[i + 1];
                index++;
            }
        }
        //  Call command handler
        cmdTable.run(argc, argv);
        localPrintf("\n");
    }
    xEventGroupClearBits(xTestEventGroup, 0x20);
    localPrintf(">");
    vTaskDelete(NULL);
}

