#include "globals.h"



void handleClock1(void)
{
}

void handleClock2(void)
{
}

void handleStart1(void)
{
}

void handleStart2(void)
{
}

void handleContinue1(void)
{
}

void handleContinue2(void)
{
}

void handleStop1(void)
{
}

void handleStop2(void)
{
}

void handleNoteOn1(byte channel, byte pitch, byte velocity)
{
}

void handleNoteOff1(byte channel, byte pitch, byte velocity)
{
}

void handleNoteOn2(byte channel, byte pitch, byte velocity)
{
}

void handleNoteOff2(byte channel, byte pitch, byte velocity)
{
}

void handleControlChange1(byte channel, byte number, byte value)
{
//    if(model.muteInput) return;
//
//    ////sendControlChange(DataByte inControlNumber, DataByte inControlValue, Channel inChannel)
//    //MIDI.sendControlChange(number, value, channel);
//
//    MidiMessage msg;
//    msg.tick = midiClock.ticks;
//
//    msg.controlMask = ControlChange;
//    msg.channel = channel;
//    msg.value = number;
//    msg.data = value;
//    splitChannels.input(&msg, 0);
}

void handlePitchBend1(byte channel, int bend)
{
//    if(model.muteInput) return;
//
//    MidiMessage msg;
//    msg.tick = midiClock.ticks;
//
//    msg.controlMask = PitchBend;
//    msg.channel = channel;
//    msg.value = 0;
//    msg.data = bend;
//    splitChannels.input(&msg, 0);
}

void handleControlChange2(byte channel, byte number, byte value)
{
}
