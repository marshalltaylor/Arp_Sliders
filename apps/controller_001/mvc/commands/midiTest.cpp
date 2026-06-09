#include "commands.h"
#include "globals.h"

// Connect directly to bsp.
#include "bsp.h"
#define localPrintf bspPrintf

int midiProgram(int argc, char *argv[]);

commandInfo_t midiCmd
    = { "midi",
        "Unknown midi tests",
        " usage: midi [g [0(disable)|1|2|3]|ctrl|merger|outs]\n  g - enb debug "
        "msgs\n  ctrl - controllers\n  merger - note merge, pass all\n  outs - "
        "output ports",
        &midiProgram };

int midiProgram(int argc, char *argv[])
{
    bool all = false;
    bool argError = false;
    if(argc == 0) all = true;
    
    if(all || 0 == strcmp((const char*)argv[0], "ctrl"))
    {
        controllers.printDebug();
    }
    if(all || 0 == strcmp((const char*)argv[0], "merger"))
    {
        merger.printDebug();
    }
    if (all || 0 == strcmp((const char *)argv[0], "outs"))
    {
        // outMain.printList();
        // outAux.printList();
    }
    if (!all && 0 == strcmp((const char*)argv[0], "g"))
    {
        if(argc < 2)
        {
            argError = true;
        }
        else
        {
            int selection = strtol(argv[1], NULL, 10);
            localPrintf("Midi graph debug option: %d\n", selection);
            switch (selection)
            {
                case 0:
                    localPrintf("Disabling all midiMod loggers\n");
                    controllers.setDebug(false);
                    outMain.setDebug(false);
                    outAux.setDebug(false);
                    merger.setDebug(false);
                    break;
                case 1:
                    controllers.setDebug(true);
                    break;
                case 2:
                    merger.setDebug(true);
                    break;
                case 3:
                    outMain.setDebug(true);
                    outAux.setDebug(true);
                    break;
                default:
                    localPrintf("option not implemented\n");
            }
        }
    }
    
    if (argError)
    {
        localPrintf("Bad arguement\n");
    }
    return 0;
}
