
static inline void printAppHelpMessage(void)
{
    localPrintf("Super Looper\n");
    localPrintf("------------\n");
    localPrintf("MIDI In:\n");
    localPrintf(" port 1: clock + keyboard\n");
    localPrintf(" port 2: footswitch in\n");

    localPrintf("\nMIDI out:\n");
    localPrintf(" port 1: to instrument (no clocks)\n");
    localPrintf("  Set `outputChannel` to match instrument\n");
    localPrintf(" port 2: ctrl out to next super looper\n");
    localPrintf("  Enable clock through with `clockThru`\n");
    localPrintf("  Enable notes through with `notesOnCtrlMidi`\n");

    localPrintf("\nConfig settings with 'set' command\n");

    localPrintf("\nPanel operation\n");
    localPrintf(" rest: acts as function key for alt functions\n");
    localPrintf(" clear: press to clear pattern\n");
    localPrintf("  rest + clear: toggle keyboard playthough to instrument\n");
    localPrintf(" rec: press to arm recording.  If new pattern, recording starts at first note on\n");
    localPrintf("  rest + rec: toggle 'A' or 'B' pattern\n");
    localPrintf(" play: start playing at current measure position, or stop current loop\n");
    localPrintf("  rest + play: start playing from beginning\n");

    localPrintf("\n");
}
