#include "bsp.h"
#include "commands.h"
#include "model.h"
#include "globals.h"

#include "taskSystem.h"

// Connect directly to bsp.
#define localPrintf bspPrintf

int setProgram(int argc, char *argv[]);

commandInfo_t settingsCmd = {
    "set",
    "Settings editor",
    "Settings editor\nDisplay, set, or reset deafults\n usage: set [|edit|reset]\n",
    &setProgram
};

int setProgram(int argc, char *argv[])
{
    bool askForChanges = false;
    bool writeChanges = false;
    char str[100];

    if ((argc == 1) && (0 == strcmp((const char*)argv[0], "reset")))
    {
        localPrintf("resetting...\n");
        controller.resetDefaultSettings();
    }
    else if (argc == 1)
    {
        if(0 == strcmp((const char*)argv[0], "edit"))
        {
            askForChanges = true;
        }
    }
    localPrintf("\n--- Settings ---\n");
    if (askForChanges)
    {
        localPrintf("Interactive edit mode\n");
    }

    localPrintf("outputChannel");
    if (!askForChanges)
    {
        localPrintf(" = %d\n", model.outputChannel);
    }
    else
    {
        localPrintf(" (%d):", model.outputChannel);
        gets(str);
        if(str[0] != '\0')
        {
            model.outputChannel = atoi(str);
            writeChanges = true;
        }
    }

    localPrintf("clockThru");
    if (!askForChanges)
    {
        localPrintf(" = %d\n", model.clockThru);
    }
    else
    {
        localPrintf(" (%d):", model.clockThru);
        gets(str);
        if(str[0] != '\0')
        {
            model.clockThru = atoi(str);
            writeChanges = true;
        }
    }

    localPrintf("notesOnCtrlMidi");
    if (!askForChanges)
    {
        localPrintf(" = %d\n", model.notesOnCtrlMidi);
    }
    else
    {
        localPrintf(" (%d):", model.notesOnCtrlMidi);
        gets(str);
        if(str[0] != '\0')
        {
            model.notesOnCtrlMidi = atoi(str);
            writeChanges = true;
        }
    }

    localPrintf("----------------\n");
    
    if (writeChanges)
    {
        localPrintf("saving...\n");
        controller.saveSettingsToFlash();
        localPrintf("reloading...\n");
        controller.reloadSettings();
        localPrintf("Done.\n");
    }
    
    return 0;
}
