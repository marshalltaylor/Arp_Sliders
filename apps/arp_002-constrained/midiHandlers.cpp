#include "globals.h"

void handleClock1( void )
{
}

void handleStart1( void )
{
}

void handleContinue1( void )
{
}

void handleStop1( void )
{
}

void handleClock2( void )
{
}

void handleStart2( void )
{
}

void handleContinue2( void )
{
}

void handleStop2( void )
{
}

void handleNoteOn1(byte channel, byte pitch, byte velocity)
{
    bspPrintf("Note On (main)\n");
}

void handleNoteOff1(byte channel, byte pitch, byte velocity)
{
}

void handleNoteOn2(byte channel, byte pitch, byte velocity)
{
    bspPrintf("Note On (ctrl)\n");
}

void handleNoteOff2(byte channel, byte pitch, byte velocity)
{
}
