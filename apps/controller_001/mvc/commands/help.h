
static inline void printAppHelpMessage(void)
{
    localPrintf("Six Sliders Controller\n");
    localPrintf("----------------------\n");
    localPrintf("MIDI In:\n");
    localPrintf(
        " input ports are mixed.  Note on/off input is parsed to prevent stuck "
        "notes from duplicate channels\n");

    localPrintf("\nMIDI out:\n");
    localPrintf(" data duplicated to all ports\n");

    localPrintf("\nConfig settings with 'set' command\n");

    localPrintf("\nPanel operation\n");
    localPrintf(" Sliders (left = 1):\n");
    localPrintf("  1: 0 (breath) channel 5\n");
    localPrintf("  2: 0 (pedal) channel 5\n");
    localPrintf("  3: 0 (breath) channel 6\n");
    localPrintf("  4: 0 (pedal) channel 6\n");
    localPrintf("  5: 10 channel 6\n");
    localPrintf("  6: 11 channel 6\n");
    localPrintf(" Switches:\n");
    localPrintf("  Sent as note on/off, channel 14\n");

    localPrintf("\n");
}
