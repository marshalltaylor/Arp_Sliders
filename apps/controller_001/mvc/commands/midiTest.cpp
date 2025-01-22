#include "commands.h"
#include "globals.h"

// Connect directly to bsp.
#include "bsp.h"
#define localPrintf bspPrintf

int midiProgram(int argc, char *argv[]);

commandInfo_t midiCmd = {
    "midi",
    "Unknown midi tests",
    " usage: midi [g [0|1]|clock|outs|ins]\n  g - enb debug msgs\n  clock - midiClock\n  outs - outputMixer\n  ins - inputNoteList",
    &midiProgram
};

int midiProgram(int argc, char *argv[])
{
    bool all = false;
    bool argError = false;
    if(argc == 0) all = true;
    
    if(all || 0 == strcmp((const char*)argv[0], "ctrl"))
    {
        controllers.printDebug();
        //controllers.tick();
    }
    if(all || 0 == strcmp((const char*)argv[0], "merger"))
    {
        merger.printDebug();
    }
    if(all || 0 == strcmp((const char*)argv[0], "ins"))
    {
        //inputNoteList.printList();
    }
    if (!all && 0 == strcmp((const char*)argv[0], "g"))
    {
        if(argc < 2)
        {
            argError = true;
        }
        else
        {
            bool enb = strtol(argv[1], NULL, 10);
            localPrintf("Midi graph debug = %d\n", enb);
            controllers.setDebug(enb);
            outMain.setDebug(enb);
            outAux.setDebug(enb);
            merger.setDebug(enb);
        }
    }
    
    if (argError)
    {
        localPrintf("Bad arguement\n");
    }
    return 0;
}
