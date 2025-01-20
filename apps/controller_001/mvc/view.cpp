#include <stdint.h>
#include <stdio.h>

#include "globals.h"

#include "model.h"
#include "view.h"
#include "controller.h"

#define localPrintf bspPrintf

View::View(void)
{
}

void View::tick(void)
{

}

void View::printModel(void)
{
    localPrintf("Info on Model @ %p\n", (void*)&model);
    localPrintf("  playing: %d\n", model.playing);
    localPrintf("  recording: %d\n", model.recording);
    localPrintf("  beatKnown: %d\n", model.beatKnown);
    localPrintf("  beatKnownChanged: %d\n", model.beatKnownChanged);
    localPrintf("  modeOption: %d\n", model.modeOption);
    localPrintf("  modeValue: %d\n", model.modeValue);
    localPrintf("  noteLen: %d\n", model.noteLen);
    localPrintf("  clkActiveTimeoutCtr_Main: %d\n", model.clkActiveTimeoutCtr_Main);
    localPrintf("  clkActiveTimeoutCtr_Aux: %d\n", model.clkActiveTimeoutCtr_Aux);
    localPrintf("  clkActive: %d\n", model.clkActive);

}
