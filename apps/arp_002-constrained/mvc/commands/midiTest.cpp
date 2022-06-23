#include "commands.h"

// Connect directly to bsp.
#include "bsp.h"
#define localPrintf bspPrintf

int midiProgram(int argc, char *argv[]);

commandInfo_t midiCmd = {
    "midi",
    "Unknown midi tests",
    " TODO",
    &midiProgram
};

int midiProgram(int argc, char *argv[])
{
    //MIDI.setHandleNoteOn(handleNoteOn);
    //MIDI.setHandleNoteOff(handleNoteOff);
    //MIDI.begin(MIDI_CHANNEL_OMNI);
    return 0;
}
