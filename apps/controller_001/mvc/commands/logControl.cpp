#include "commands.h"

// Connect directly to bsp.
#include "bsp.h"
#define localPrintf bspPrintf

int logProgram(int argc, char *argv[]);

commandInfo_t logCmd = {
    "log",
    "set mode",
    " usage: log [auto|default]\n  auto - TODO\n  default - TODO",
    &logProgram
};

int logProgram(int argc, char *argv[])
{
    if(0 == strcmp((const char*)argv[0], "auto"))
    {
        //consoleDebug.setMode(LOG_MODE_AUTO);
    }
    else if(0 == strcmp((const char*)argv[0], "default"))
    {
        //consoleDebug.setMode(LOG_MODE_DEFAULT);
    }
    else
    {
        localPrintf("Needs 'auto' or 'default'\n");
    }
    return 0;
}
