#include "commands.h"

// Connect directly to bsp.
#include "bsp.h"
#define localPrintf bspPrintf

CommandTable::CommandTable(void)
{
    cmds[0] = &statCmd;
    cmds[1] = &bitCmd;
    cmds[2] = &delayCmd;
    cmds[3] = &helloCmd;
    cmds[4] = &xqCmd;
    cmds[5] = &logCmd;
    cmds[6] = &spiCmd;
    cmds[7] = &adcCmd;
    cmds[8] = &midiCmd;
    cmds[9] = &settingsCmd;
    
    cmdLen = 10;
};

int CommandTable::run(int argc, char *argv[])
{
    int retVal = 1;
    //Test for 'help' command first
    if(0 == strcmp((const char*)argv[0], "help"))
    {
        help();
        retVal = 0;
    }
    //Look for command
    else for(int i = 0; i < cmdLen; i++)
    {
        if(0 == strcmp((const char*)argv[0], cmds[i]->cmd))
        {
            //check if additional arg is for help
            if((argc > 1)&&(0 == strcmp((const char*)argv[1], "help")))
            {
                localPrintf("%s\n", cmds[i]->cmd, cmds[i]->comment);
                localPrintf(cmds[i]->helpDetail);
            }
            else
            {
                //Run the command
                cmds[i]->fnptr(argc - 1, &argv[1]);
            }
            retVal = 0;
            break;
        }
    }
    if(retVal == 1)
    {
        localPrintf("Command not found.  Try 'help'\n");
    }
    return retVal;
};

void CommandTable::help(void)
{
    localPrintf("Generic Console.\n");
    for(int i = 0; i < cmdLen; i++)
    {
        localPrintf(" '%s' -- %s\n", cmds[i]->cmd, cmds[i]->comment);
    }
}
