#include "commands.h"
#include "globals.h"

// Connect directly to bsp.
#include "bsp.h"
#define localPrintf bspPrintf

int appProgram(int argc, char *argv[]);

commandInfo_t appCmd = {
    "app",
    "do app related things",
    "  load -- fill seq with test data (probably broken)\n  mem - pool info\n  model - data\n  seq - sequences",
    &appProgram
};

static void printPoolInfo(Pool * pPool)
{
    localPrintf("Info on poolData @ %p\n", (void*)pPool);
    localPrintf("  Bytes in type: %d\n", pPool->dataWidth);
    localPrintf("  number of slots: %d\n", pPool->poolSize);
    localPrintf("  slots used: %d (%.2f%%)\n", pPool->slotsUsed(), 100*(float)pPool->slotsUsed()/pPool->poolSize);
    localPrintf("  Total bytes of data: %d\n", pPool->poolSize * pPool->dataWidth);
    localPrintf("  Slot bits: \n   ");
    for(int i = 0; i < pPool->poolSize; i++)
    {
        localPrintf("%d", pPool->inUseRead(i));
        if((i + 1) % 40 == 0)
        {
            localPrintf("\n   ");
        }
        else if((i + 1) % 10 == 0)
        {
            localPrintf(" ");
        }
    }
    localPrintf("\n");
    
    //Errors
    poolErrorStats_t * pErr = pPool->getErrorStats();
    localPrintf("  Errors, palloc: %d, pfree: %d\n", pErr->failedPalloc, pErr->failedPfree);

}

int appProgram(int argc, char *argv[])
{
    bool all = false;
    if(argc == 0) all = true;
    
    if(0 == strcmp((const char*)argv[0], "load"))
    {
        //if(0 == strcmp((const char*)argv[1], "1"))
        //{
        //    //consoleDebug.setMode(LOG_MODE_AUTO);
        //}
        
        //Load test data
        //fillListWithTestData(&mainNoteList, real4bar);
        //player.seq->length = 8 * 24;
    }
    
    if(all || 0 == strcmp((const char*)argv[0], "mem"))
    {
        //Print mem info
        printPoolInfo(&pool_20);
        printPoolInfo(&pool_32);
        //player.printInfo();
    }
    
    if(all || 0 == strcmp((const char*)argv[0], "model"))
    {
        //Print mem info
        view.printModel();
    }
    
    if(all || 0 == strcmp((const char*)argv[0], "seq"))
    {
        localPrintf("-- mainNoteList --\n");
        //mainNoteList.printList();
        localPrintf("-- backupNoteList --\n");
        //backupNoteList.printList();
        localPrintf("-- sequence info --\n");
        //localPrintf(" channel: %d\n", player.seq->channel);
        //localPrintf(" playing: %d\n", player.seq->playing);
        //localPrintf(" playHead: %d\n", player.seq->playHead);
        //localPrintf(" length: %d\n", player.seq->length);
        //localPrintf(" syncState: %d\n", player.seq->syncState);
    }
    
    if(all || 0 == strcmp((const char*)argv[0], "arp"))
    {
        //Print mem info
        if (argc < 2)
        {
            //arp.printInfo();
            
            localPrintf(" Extra arguments: <cmd> <value>\n");
            localPrintf("  cmd: e, mode, step, pat\n");
        }
        //Process 1 field commands
        else if(argc == 2)
        {
            if (0 == strcmp((const char*)argv[1], "c"))
            {
                localPrintf("Clearing arp\n");
                //arp.clearPattern();
            }
        }
        //Process 2 field commands
        else if(argc == 3)
        {
            int value = strtol(argv[2], NULL, 10);
            if (0 == strcmp((const char*)argv[1], "e"))
            {
                localPrintf("Enable: %d\n", value);
                //arp.enableArp(value);
            }
            if (0 == strcmp((const char*)argv[1], "mode"))
            {
                localPrintf("Mode: %d\n", value);
                //arp.enablePatternEntry(value);
            }
            if (0 == strcmp((const char*)argv[1], "step"))
            {
                localPrintf("step period: %d\n", value);
                //arp.setQuantizeNotePeriod(value);
            }
            if (0 == strcmp((const char*)argv[1], "pat"))
            {
                localPrintf("pattern len: %d\n", value);
                //arp.length = value;
            }
        }
    }

    return 0;
}
