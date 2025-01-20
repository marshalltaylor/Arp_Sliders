
#include "bsp.h"
#include "os.h"
#include "globals.h"

#include "commands.h"

#include "taskLog.h"

// Connect directly to bsp.
#define localPrintf bspPrintf

TaskStatus_t pxTaskStatusArray[10];

int statProgram(int argc, char *argv[]);
int bitProgram(int argc, char *argv[]);
int delayProgram(int argc, char *argv[]);
int helloProgram(int argc, char *argv[]);
int xqProgram(int argc, char *argv[]);

commandInfo_t statCmd = {
    "stat",
    "CPU usage",
    "No arguments.  Provides some info about the rtos.",
    &statProgram
};

commandInfo_t bitCmd = {
    "bit",
    "Set or clear general control bits",
    " usage: bit [set|clr] <bit num>",
    &bitProgram
};

commandInfo_t delayCmd = {
    "delay",
    "Load or sleep task",
    " usage: delay [task|load|forever]\n Caution! forever enters a while(1) loop",
    &delayProgram
};

commandInfo_t helloCmd = {
    "hello",
    "Send message over q",
    "TODO",
    &helloProgram
};

commandInfo_t xqCmd = {
    "xq",
    "Send message over q",
    "TODO",
    &xqProgram
};

int statProgram(int argc, char *argv[])
{
    volatile UBaseType_t uxArraySize, x;
    unsigned long ulTotalRunTime, ulStatsAsPercentage;

/* Memory information */
    localPrintf("Info on pool @ %p\n", (void*)&pool_20);
    //localPrintf("Info on pool_20\n");
    localPrintf("  byte width: %d\n", pool_20.dataWidth);
    localPrintf("  number of slots: %d\n", pool_20.poolSize);
    localPrintf("  slots used: %d (% 3d%%)\n", pool_20.slotsUsed(), 100*((float)pool_20.slotsUsed()/pool_20.poolSize));
    localPrintf("  Total bytes of data: %d\n", pool_20.poolSize * pool_20.dataWidth);
    localPrintf("  Slot bits: ");
    for(int i = 0; i < pool_20.poolSize; i++)
    {
        localPrintf("%d", pool_20.inUseRead(i));
    }
    localPrintf("\n");
    
    //Errors
    poolErrorStats_t * pErr = pool_20.getErrorStats();
    localPrintf("  Errors, palloc: %d, pfree: %d\n", pErr->failedPalloc, pErr->failedPfree);


/* Task information */
    uxArraySize = uxTaskGetNumberOfTasks();// get num of tasks
    // Generate raw status information about each task.
    uxTaskGetSystemState( pxTaskStatusArray,
                                uxArraySize,
                                &ulTotalRunTime );

    localPrintf("%d\n", ulTotalRunTime);
    ulTotalRunTime /= 100UL;// For percentage calculations.
    //ulTotalRunTime = 1; //fake it
    localPrintf("%d\n", ulTotalRunTime);
    
    if( ulTotalRunTime > 0 )// Avoid divide by zero errors.
    {
        localPrintf(" %-7s%7s%5s%7s\n", "name", "epoch", "Load", "StkFr");
        localPrintf("---------------------------\n");
        /* For each populated position in the pxTaskStatusArray array,
        format the raw data as human readable ASCII data. */
        for( x = 0; x < uxArraySize; x++ )
        {
            /* What percentage of the total run time has the task used?
            This will always be rounded down to the nearest integer.
            ulTotalRunTimeDiv100 has already been divided by 100. */
            ulStatsAsPercentage =
                pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;

            if( ulStatsAsPercentage > 0UL )
            {
                localPrintf(" %-7s%7lu%4lu%%%7lu\n",
                            pxTaskStatusArray[ x ].pcTaskName,
                            pxTaskStatusArray[ x ].ulRunTimeCounter,
                            ulStatsAsPercentage,
                            uxTaskGetStackHighWaterMark(pxTaskStatusArray[ x ].xHandle)
                            );
            }
            else
            {
             /* If the percentage is zero here then the task has
             consumed less than 1% of the total run time. */
             localPrintf(" %-7s%7lu%5s%7lu\n",
                        pxTaskStatusArray[ x ].pcTaskName,
                        pxTaskStatusArray[ x ].ulRunTimeCounter,
                        "<1%",
                        uxTaskGetStackHighWaterMark(pxTaskStatusArray[ x ].xHandle)
                        );
            }
        }
    }
    return 0;
}

int bitProgram(int argc, char *argv[])
{
    EventBits_t uxBits = xEventGroupGetBits(xTestEventGroup);
    if(argc < 2)
    {
        localPrintf("needs arg and value\n");
        return 1;
    }
    int intArg = atoi(argv[1]);
    if((intArg < 0)||(intArg > 7))
    {
        localPrintf("value 0 - 7\n");
    }
    else
    {
        uint16_t testKeyMask = 0;
        testKeyMask = 0x0001 << intArg;
        if(0 == strcmp((const char*)argv[0], "set"))
        {
            uxBits |= testKeyMask;
            xEventGroupSetBits(xTestEventGroup, testKeyMask );
        }
        else if(0 == strcmp((const char*)argv[0], "clr"))
        {
            uxBits &= ~testKeyMask;
            xEventGroupClearBits(xTestEventGroup, testKeyMask );
        }
        else
        {
            localPrintf("Needs 'set' or 'clr'\n");
        }
    }
    for(int i = 7; i >= 0; i--)
    {
        localPrintf("%d", (uxBits >> i)&0x0001);
    }
    return 0;
}

int delayProgram(int argc, char *argv[])
{
    if(0 == strcmp((const char*)argv[0], "task"))
    {
        //Test delay times
        localPrintf("\n");
        localPrintf("Start time  %dms\n", millis());
        vTaskDelay( 1000 );
        localPrintf("Stop time   %dms\n", millis());
        //localPrintf("tick count     %dms\n", xTaskGetTickCount());
    }
    else if(0 == strcmp((const char*)argv[0], "load"))
    {
        //Test delay times
        localPrintf("L");
        bspDelay( 333 );
        localPrintf("O");
        bspDelay( 333 );
        localPrintf("A");
        bspDelay( 333 );
        localPrintf("D");
    }
    else if(0 == strcmp((const char*)argv[0], "forever"))
    {
        while(1);
    }
    else
    {
        localPrintf("Error: args\n");
    }
    return 0;
}

int helloProgram(int argc, char *argv[])
{
    strMsg_t * msg = new strMsg_t();
    
    msg->id = 0;
    sprintf( msg->data, "Hello world!\n");
    if(pdPASS != xQueueSend( logQueue, &msg, 0 ))
    {
        //TODO: error on send
        delete msg;
    }
    return 0;
}

int xqProgram(int argc, char *argv[])
{
    localPrintf("3");
    bspDelay(300);
    localPrintf("2");
    bspDelay(300);
    localPrintf("1");
    bspDelay(300);
    
    strMsg_t * msg = new strMsg_t();
    
    msg->id = -1;
    msg->data[0] = '\0';
    if(pdPASS != xQueueSend( logQueue, &msg, 0 ))
    {
        localPrintf(".dud");
        delete msg;
    }
    return 0;
}
