#include "globals.h"

void handleClock1(void)
{
    MidiMessage msg;
    msg.tick = 0;

    msg.controlMask = Clock;
    msg.channel     = 0;
    msg.value       = 0;
    msg.data        = 0;
    merger.input(&msg, 0);
}

void handleClock2(void)
{
    MidiMessage msg;
    msg.tick = 0;

    msg.controlMask = Clock;
    msg.channel     = 0;
    msg.value       = 0;
    msg.data        = 0;
    merger.input(&msg, 1);
}

void handleStart1(void)
{
    MidiMessage msg;
    msg.tick = 0;

    msg.controlMask = Start;
    msg.channel     = 0;
    msg.value       = 0;
    msg.data        = 0;
    merger.input(&msg, 0);
}

void handleStart2(void)
{
    MidiMessage msg;
    msg.tick = 0;

    msg.controlMask = Start;
    msg.channel     = 0;
    msg.value       = 0;
    msg.data        = 0;
    merger.input(&msg, 1);
}

void handleContinue1(void)
{
    MidiMessage msg;
    msg.tick = 0;

    msg.controlMask = Continue;
    msg.channel     = 0;
    msg.value       = 0;
    msg.data        = 0;
    merger.input(&msg, 0);
}

void handleContinue2(void)
{
    MidiMessage msg;
    msg.tick = 0;

    msg.controlMask = Continue;
    msg.channel     = 0;
    msg.value       = 0;
    msg.data        = 0;
    merger.input(&msg, 1);
}

void handleStop1(void)
{
    MidiMessage msg;
    msg.tick = 0;

    msg.controlMask = Stop;
    msg.channel     = 0;
    msg.value       = 0;
    msg.data        = 0;
    merger.input(&msg, 0);
}

void handleStop2(void)
{
    MidiMessage msg;
    msg.tick = 0;

    msg.controlMask = Stop;
    msg.channel     = 0;
    msg.value       = 0;
    msg.data        = 0;
    merger.input(&msg, 1);
}

void handleNoteOn1(byte channel, byte pitch, byte velocity)
{
    MidiMessage msg;
    msg.tick = 0;
    // bspPrintf("Note On (main), @ %d\n", msg.tick);

    msg.controlMask = NoteOn;
    msg.channel     = channel;
    msg.value       = pitch;
    msg.data        = velocity;
    merger.input(&msg, 0);
}

void handleNoteOff1(byte channel, byte pitch, byte velocity)
{
    MidiMessage msg;
    msg.tick = 0;
    // bspPrintf("Note Off (main), @ %d\n", msg.tick);

    msg.controlMask = NoteOff;
    msg.channel     = channel;
    msg.value       = pitch;
    msg.data        = velocity;
    merger.input(&msg, 0);
}

void handleNoteOn2(byte channel, byte pitch, byte velocity)
{
    MidiMessage msg;
    msg.tick = 0;
    // bspPrintf("Note On (aux), @ %d\n", msg.tick);

    msg.controlMask = NoteOn;
    msg.channel     = channel;
    msg.value       = pitch;
    msg.data        = velocity;
    merger.input(&msg, 1);
}

void handleNoteOff2(byte channel, byte pitch, byte velocity)
{
    MidiMessage msg;
    msg.tick = 0;
    // bspPrintf("Note Off (aux), @ %d\n", msg.tick);

    msg.controlMask = NoteOff;
    msg.channel     = channel;
    msg.value       = pitch;
    msg.data        = velocity;
    merger.input(&msg, 1);
}

void handleControlChange1(byte channel, byte number, byte value)
{
    MidiMessage msg;
    msg.tick = 0;

    msg.controlMask = ControlChange;
    msg.channel     = channel;
    msg.value       = number;
    msg.data        = value;
    merger.input(&msg, 0);
}

void handlePitchBend1(byte channel, int bend)
{
    MidiMessage msg;
    msg.tick = 0;

    msg.controlMask = PitchBend;
    msg.channel     = channel;
    msg.value       = 0;
    msg.data        = bend;
    merger.input(&msg, 0);
}

void handleControlChange2(byte channel, byte number, byte value)
{
    MidiMessage msg;
    msg.tick = 0;

    msg.controlMask = ControlChange;
    msg.channel     = channel;
    msg.value       = number;
    msg.data        = value;
    merger.input(&msg, 1);
}

void
handlePitchBend2 (byte channel, int bend)
{
    MidiMessage msg;
    msg.tick = 0;

    msg.controlMask = PitchBend;
    msg.channel     = channel;
    msg.value       = 0;
    msg.data        = bend;
    merger.input(&msg, 1);
}
