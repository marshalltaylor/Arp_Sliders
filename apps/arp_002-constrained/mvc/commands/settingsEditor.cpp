#include "bsp.h"
#include "commands.h"
#include "model.h"

#include "taskSystem.h"

// Connect directly to bsp.
#define localPrintf bspPrintf

int setProgram(int argc, char *argv[]);

commandInfo_t settingsCmd = {
    "set",
    "Settings editor",
    "A text string!\n usage: set [auto|default]\n",
    &setProgram
};

void listSettings(void)
{
    localPrintf("--- Settings ---\n");
    localPrintf("Param = %d\n", model.param);
}

int setProgram(int argc, char *argv[])
{
    if(argc < 1)
    {
        //no argument given
        listSettings();
        return 0;
    }
    else if(argc < 2)
    {
        localPrintf("SETTINGS EDITOR\n");
        listSettings();
        //Look for 'set edit' or 'set save'
        if(0 == strcmp((const char*)argv[0], "edit"))
        {
            char str[100];
            localPrintf("\n");
            localPrintf("Enter Param (%d):", model.param);
            gets(str);
            model.param = atoi(str);
            localPrintf("Done.\n");
            localPrintf("\n");
            listSettings();
        }
    }
    return 0;
}
