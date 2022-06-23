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
#include "os.h"
#include "taskConsole.h"

/* Includes -- modules -------------------------------------------------------*/
#include "logging.h"

/* References ----------------------------------------------------------------*/
#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
Logging logSystemLog;
#define localPrintf logSystemLog.printf

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif

static comPortInterface_t bspCom;
static int getsBufferPtr = 0;

TaskHandle_t xProgramHandle = NULL;
int debugCtr = 0;
//A common interface for serial object is presented by the bsp.
//Example:  bspGetSerialFunctions(COM0, &console);
//So, present this interface to application tasks that may need
//the serial interface... after we have delt with supervisory
//activities like ctrl-c
#define SYSBUFFERSIZE 128
uint8_t sysSerBuffer[SYSBUFFERSIZE];
int bufferStartIndex = 0;
int bufferEndIndex = 0;
uint16_t cmdBufferPtr = 0;

char * gets( char * str )
{
    xEventGroupSetBits(xTestEventGroup, 0x10);
    getsBufferPtr = cmdBufferPtr;
    //Now wait for it to clear
    EventBits_t uxBits = xEventGroupGetBits( xTestEventGroup );
    while(uxBits & 0x10)
    {
        //Waiting
        vTaskDelay( 20 );
        uxBits = xEventGroupGetBits( xTestEventGroup );
    }
    sprintf(str, "%s", &cmdBuffer[getsBufferPtr]);
    return NULL;
}

uint8_t tSys_Peek(void){
    return sysSerBuffer[bufferStartIndex];
}
void tSys_Write(uint8_t data){
    ;
}
uint16_t tSys_BytesAvailable(void){
    if(bufferStartIndex <= bufferEndIndex) //no roll
    {
        return bufferEndIndex - bufferStartIndex;
    }
    else //Rolled,
    {
        return (bufferEndIndex + SYSBUFFERSIZE) - bufferStartIndex;
    }
}
uint8_t tSys_Read(void){ //only call when there is data to be read!  Unprotected!

    uint8_t retVal = sysSerBuffer[bufferStartIndex];
    bufferStartIndex += 1;
    if(bufferStartIndex >= SYSBUFFERSIZE) bufferStartIndex = 0;
    return retVal;
}

void tSysGetSerialFunctions(comPortInterface_t * interface)
{
    interface->peek = tSys_Peek;
    interface->write = tSys_Write;
    interface->read = tSys_Read;
    interface->bytesAvailable = tSys_BytesAvailable;
}

extern "C" void taskSystemStart(void * argument)
{
#ifdef USE_LOGGING
	logSystemLog.setStamp("taskSystem", 10);
	logSystemLog.setMode(LOG_MODE_DEFAULT);
#endif
    bspGetSerialFunctions(COM0, &bspCom);
    
    cmdBufferPtr = 0;
    
    //Run a default task at boot:
    sprintf(cmdActive, "help");
    xEventGroupSetBits(xTestEventGroup, 0x20 );
    uint8_t retVal = xTaskCreate( taskConsoleStart, "console", 512, (void*) 1, 1, &xProgramHandle);
    if (retVal != pdPASS)
    {
        while(1);
    }
    
    while(1)// Smash it?
    {
        debugCtr++;
        if(debugCtr > 5000)
        {
            debugCtr = 0;
            //EventBits_t uxBits = xEventGroupGetBits( xTestEventGroup );
            //localPrintf("B:0x%02X\n", uxBits);
        }
        if(xSemaphoreTake( xSerCtrl, 1000 )) //try for 10 ticks
        {
            if(bspCom.bytesAvailable())
            {
                char c = (char)bspCom.read();
                //Always write to passthrough serial
                {
                    int index = bufferEndIndex + 1;
                    if(index >= SYSBUFFERSIZE) index = 0;
                    //index is now the prospective new end index
                    if(index == bufferStartIndex)
                    {
                        //Too far, overwrite last char
                        index = bufferEndIndex;
                    }        
                    sysSerBuffer[bufferEndIndex] = c;
                    bufferEndIndex = index;
                }
                //If bit 5 is set, program is running.
                EventBits_t uxBits = xEventGroupGetBits( xTestEventGroup );
                if(uxBits & 0x20)
                {
                    //Program is running
                    
                    //Ctrl chars
                    if(c == 0x03) // Ctrl-c
                    {
                        //if program running, switch to console
                        localPrintf("Ctrl-c caught by system\n");
                        xEventGroupClearBits(xTestEventGroup, 0x20);
                        vTaskSuspendAll();
                        //TaskHandle_t xTask = xProgramHandle;
                        //if( xProgramHandle != NULL )
                        //{
                        //    xProgramHandle = NULL;
                        //    /* Delete using the copy of the handle. */
                        vTaskDelete(xProgramHandle);
                        //    vTaskDelete(&xTask);
                        //}
                        xTaskResumeAll();
                        localPrintf(">");
                    }
                }
                if((c >= 0x20)&&(c < 0x80))
                {
                    if(!(uxBits & 0x20) || (uxBits & 0x10))
                    {
                        localPrintf("%c", c);
                        cmdBuffer[cmdBufferPtr] = c;
                        if(cmdBufferPtr < CMDBUFFERSIZE - 1)
                        {
                            cmdBufferPtr++;
                        }
                    }
                }
                else if(c == 0x08) // Backspace
                {
                    int limit = 0;
                    if(!(uxBits & 0x20) || (uxBits & 0x10))
                    {
                        limit = getsBufferPtr;
                    }
                    cmdBuffer[cmdBufferPtr] = 0x00;
                    if(cmdBufferPtr > limit)
                    {
                        localPrintf("%c", 0x08);
                        localPrintf("%c", ' ');
                        localPrintf("%c", 0x08);
                        cmdBufferPtr--;
                    }
                }
                else if(c == '\n')
                {
                    localPrintf("\n");
                    // Parse buffer
                    cmdBuffer[cmdBufferPtr] = 0x00;
                    //  here, cmdBufferPtr is length of string
                    //localPrintf("%s\n", cmdBuffer);  // <-- USE TO DEBUG INPUT STRING
                    
                    // The captured line can be used as args to be executed, or getline() input
                    if(!(uxBits & 0x20)) // execute args
                    {
                        //If cmdBufferPtr is 0, no chars have been entered.  Give a new shell and bail
                        if(cmdBufferPtr == 0)
                        {
                            localPrintf(">");
                        }
                        else
                        {
                            //Copy the buffer for the command task's usage
                            sprintf(cmdActive, "%s", cmdBuffer);
                            
                            //  Identify first real char
                            int firstCharIndex = 0;
                            for(firstCharIndex = 0; (cmdBuffer[firstCharIndex] == ' ') && (firstCharIndex < cmdBufferPtr); firstCharIndex++)
                            {
                            }
                            int argc = 0;
                            if(firstCharIndex < cmdBufferPtr)
                            {
                                argc = 1;
                            }
                            
                            if(argc == 1)
                            {
                                // Set flag for program running
                                xEventGroupSetBits(xTestEventGroup, 0x20 );
                                //cmdBuffer won't be used until the flag is clear
                                
                                //Now start the task
                                uint8_t retVal = xTaskCreate( taskConsoleStart, "console", 512, (void*) 1, 1, &xProgramHandle);
                                if (retVal != pdPASS)
                                {
                                    while(1);
                                }
                                
                                //localPrintf("\n");
                            }
                        }
                    }
                    else if(uxBits & 0x10)
                    {
                        //We got a newline and gets() is waiting.
                        xEventGroupClearBits(xTestEventGroup, 0x10 );
                    }
                    
                    // Reset
                    cmdBufferPtr = 0;
                }
                
            }
            xSemaphoreGive( xSerCtrl );
        }
        
        vTaskDelay( 1 );
        
    }
}
