#include "globals.h"



void handleClock1(void)
{
    model.clkActiveTimeoutCtr_Main = 0;
    if(model.clkActive == 0)
    {
        midiClock.incrementTick();
        if(model.clockThru)
        {
            CtrlMIDI.sendRealTime(midi::Clock);
        }
    }
    if(midiClock.ticks % 24 == 0)
    {
        bspIOPinWrite(DEBUG_1, 1);
    }
    else
    {
        bspIOPinWrite(DEBUG_1, 0);
    }
}

void handleClock2(void)
{
    model.clkActiveTimeoutCtr_Aux = 0;
    
    if(model.clkActive == 1)
    {
        midiClock.incrementTick();
        if(model.clockThru)
        {
            CtrlMIDI.sendRealTime(midi::Clock);
        }
    }
}

void handleStart1(void)
{
    if(model.clkActive == 0)
    {
        CtrlMIDI.sendRealTime(midi::Start);
        midiClock.setTickCount(0);
        midiClock.setState(Playing);
        controller.clockGood();
        controller.startPlayingAbsolute();
    }
}

void handleStart2(void)
{
    if(model.clkActive == 1)
    {
        CtrlMIDI.sendRealTime(midi::Start);
        midiClock.setTickCount(-1);
        midiClock.setState(Playing);
        controller.clockGood();
        controller.startPlayingAbsolute();
    }
}

void handleContinue1(void)
{
    if(model.clkActive == 0)
    {
        CtrlMIDI.sendRealTime(midi::Continue);
        midiClock.setState(Playing);
        midiClock.setTickCount(player.seq->playHead);
        controller.startPlayingSynchronously(player.seq->playHead);
    }
}

void handleContinue2(void)
{
    if(model.clkActive == 1)
    {
        CtrlMIDI.sendRealTime(midi::Continue);
        midiClock.setState(Playing);
        midiClock.setTickCount(player.seq->playHead);
        controller.startPlayingSynchronously(player.seq->playHead);
    }
}

void handleStop1(void)
{
    model.clkActiveTimeoutCtr_Main = 0;
    
    if(model.clkActive == 0)
    {
        CtrlMIDI.sendRealTime(midi::Stop);
        midiClock.setState(Stopped);
        controller.stopPlaying();
    }
}

void handleStop2(void)
{
    if(model.clkActive == 1)
    {
        CtrlMIDI.sendRealTime(midi::Stop);
        midiClock.setState(Stopped);
        controller.stopPlaying();
    }
}

void handleNoteOn1(byte channel, byte pitch, byte velocity)
{
    if(model.muteInput) return;
    int32_t noteAtFnStart = midiClock.ticks;
    bspPrintf("Note On (main), @ %d\n", noteAtFnStart);
    //bspPrintf("Channel %d\n", channel);
    MidiMessage msg;
    msg.tick = noteAtFnStart;
    //while(msg.timeStamp >= loopLength )
    //{
    //    msg.timeStamp = msg.timeStamp - loopLength;
    //}
    msg.controlMask = NoteOn;
    msg.channel = channel;
    msg.value = pitch;
    msg.data = velocity;
    splitChannels.input(&msg, 0);
}

void handleNoteOff1(byte channel, byte pitch, byte velocity)
{
    if(model.muteInput) return;
    int32_t noteAtFnStart = midiClock.ticks;
    bspPrintf("Note Off (main), @ %d\n", noteAtFnStart);
    MidiMessage msg;
    msg.tick = noteAtFnStart;
    //while(msg.timeStamp >= loopLength )
    //{
    //    msg.timeStamp = msg.timeStamp - loopLength;
    //}
    msg.controlMask = NoteOff;
    msg.channel = channel;
    msg.value = pitch;
    msg.data = velocity;
    splitChannels.input(&msg, 0);
}

void handleNoteOn2(byte channel, byte pitch, byte velocity)
{
//TODO: Match input 1, this is legacy calling
//    controller.checkFirstInput();
//    int32_t noteAtFnStart = midiClock.ticks;
//    //bspPrintf("Note On (ctrl), @ %d\n", noteAtFnStart);
//    
//    seqNoteRecorded_t noteRec;
//    noteRec.note.pitch = pitch;
//    noteRec.note.velocity = velocity;
//    
//    
//    noteRec.note.time = noteAtFnStart;
//    noteRec.recordedFlag = model.recording;
//    inputNoteList.noteOn( &noteRec );
}

void handleNoteOff2(byte channel, byte pitch, byte velocity)
{
//TODO: Match input 1, this is legacy calling
//    //int32_t noteAtFnStart = midiClock.ticks;
//    //bspPrintf("Note Off (ctrl), @ %d\n", noteAtFnStart);
//    
//    seqNoteRecorded_t noteRec;
//    noteRec.note.pitch = pitch;
//    noteRec.note.velocity = velocity;
//    
//    noteRec.recordedFlag = model.recording; //Maybe used for notes on during rec start?
//
//    inputNoteList.noteOff( &noteRec );
}

void handleControlChange1(byte channel, byte number, byte value)
{
    if(model.muteInput) return;

    ////sendControlChange(DataByte inControlNumber, DataByte inControlValue, Channel inChannel)
    //MIDI.sendControlChange(number, value, channel);

    MidiMessage msg;
    msg.tick = midiClock.ticks;

    msg.controlMask = ControlChange;
    msg.channel = channel;
    msg.value = number;
    msg.data = value;
    splitChannels.input(&msg, 0);
}

void handlePitchBend1(byte channel, int bend)
{
    if(model.muteInput) return;

    MidiMessage msg;
    msg.tick = midiClock.ticks;

    msg.controlMask = PitchBend;
    msg.channel = channel;
    msg.value = 0;
    msg.data = bend;
    splitChannels.input(&msg, 0);
}

void handleControlChange2(byte channel, byte number, byte value)
{
    //sendControlChange(DataByte inControlNumber, DataByte inControlValue, Channel inChannel)
    CtrlMIDI.sendControlChange(number, value, channel);
    //Also parse for this looper
    if(channel != 1) return;
    if(model.muteInput) return;
    switch(number)
    {
        case 0:
        {
            if(value == 1)
            {
                //Same action as 'rec' rising edge
                controller.midiCtlRec.setFlag();
            }
            else if(value == 0)
            {
                controller.midiCtlRec.clearFlag();
            }
        }
        break;
        case 1:
        {
            if(value == 1)
            {
                //Same action as 'play' rising edge
                controller.midiCtlPlay.setFlag();
            }
            else if(value == 0)
            {
                controller.midiCtlPlay.clearFlag();
            }
        }
        break;
        default:
        break;
    }
}
