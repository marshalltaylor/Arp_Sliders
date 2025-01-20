/* Copyright (C) Marshall Taylor - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Marshall Taylor <marshall.taylor@alumni.uidaho.edu>, December 2023
 */


#include <string.h>
#include <stdint.h>
#include <cstdio>

#include "midi47fx.h"
#include "arp.h"
#include "bsp.h"

#include "globals.h" //for led (via controller)

//------- defines ---------------------------------------//

// Connect directly to bsp.
#define localPrintf bspPrintf

//------- class -----------------------------------------//

// ------ Arp List (pattern)
ArpPattern::ArpPattern(listIndex_t maxLengthIn, uint16_t dataSizeIn) : LinkedList(maxLengthIn, dataSizeIn)
{
    //((seqNote_t*)&nullObject)->pitch = 33;
    //((seqNote_t*)&nullObject)->velocity = 0;
}

//void ArpPattern::addNote(arpSeqNote_t * note)
//{
//    
//}

void ArpPattern::printList(void)
{
    listItemContainer_t * tempCont;
    //our custom type
    MidiMessage * pData;
    tempCont = startObjectPtr;
    pData = (MidiMessage *)tempCont->data;
    localPrintf("\n%s, %s, %s, %s, %s\n",
                "controlMask",
                "channel",
                "value",
                "data",
                "tick");
    localPrintf(  "-------------------------\n");
    //Iterate to the depth
    for ( uint8_t i = 0; i < currentPosition; i++ )
    {
        localPrintf("%d, 0x%02X, %d, %d, %d, %d",
                    i,
                    pData->controlMask,
                    pData->channel,
                    pData->value,
                    pData->data,
                    pData->tick);
        
        //move index
        tempCont = tempCont->next;
        pData = (MidiMessage *)tempCont->data;
        //Print bookmarks
        for(int j = 0; j < BOOKMARKS_MAX; j++)
        {
            if(i == bookmarks[j])
            {
                localPrintf("[%d]",j);
            }
        }
        localPrintf("\n");
  }
  localPrintf("\ncurrentPosition: %d\n", currentPosition);
  
}

//------- class -----------------------------------------//

// ----- Arp Module -- play and record
ArpModule::ArpModule(void)
{
    arpEnabled = true;
    patternEntry = true;
    arpPattern = NULL;
    gatedCount = 0;
    quantizeStepLength = true;

    playHead = 0;
    currentNote = 0;
    playing = false;
    length = 24 * 4;
    rootNote = 24;
    rootNoteDirtyFlag = false;
    //noteList->clear();
    syncState = seqType::LENGTH_SET;

    liveTimeQuantize.setEnable(true);
    setQuantizeNotePeriod(24);
}

//This function will get kinda twisted, so basically what does it do?
//Look at subpulse playhead and do something
//  is this the subpulse of the next note?
//  is a note currently playing that has been on too long?

    // I'm going go go ahead and say when notes are put in the
    // the noteOnList, their 'length' member becomes 'time to live'.
    // That way this function can pass delta into it and kill when < 0.

// NOTE: Getting 24 ticks/beat
bool ArpModule::process(uint16_t pulseDelta)
{
    if (pulseDelta > 1) localPrintf("D=%d\n", pulseDelta);
    //Age the notes in the on list
    pNoteOnList->decrementTimes(pulseDelta);
    
    //Check for expired notes, and go accordingly
    seqNote_t note;
    while(pNoteOnList->dropExpiredNote(&note))
    {
        goNoteOff(&note);
    }
    
    bool retVal = false;
    //len should be listIndex_t
    uint32_t len = arpPattern->listLength();
//    //Check for new notes to be played and put them in the note on list
//    if(playing)
    if ((arpEnabled) && (gatedCount > 0) && (((playHead % notePeriod) == 0) || rootNoteDirtyFlag))
    {
        if (len > 0)
        {
            bool playNoteOn = true;
            if (rootNoteDirtyFlag)
            {
                rootNoteDirtyFlag = false;
                //Restart pattern
                localPrintf("Restart pattern\n");
                currentNote = 0;
                if (playHead >= 0)
                {
                    localPrintf("Catch up\n");
                    //play has already started, but the first note was missed
                    playNoteOn = false;
                    uint32_t notePeriodsWhole = playHead / notePeriod;
                    uint32_t notePeriodsPartial = playHead - (notePeriodsWhole * notePeriod);
                    uint32_t noteLen = (notePeriod / 2);
                    
                    if (notePeriodsPartial < noteLen)
                    {
                        localPrintf("enough time to sound\n");
                        //Only start the note if it actually has time to play
                        playNoteOn = true;
                        noteLen = noteLen - notePeriodsPartial;
                        //Build up and play the note
                        currentNote = notePeriodsWhole;
                        uint32_t len = arpPattern->listLength();
                        currentNote = currentNote % len;
                    }
                }
                else
                {
                    playNoteOn = false;
                }
            }
            if (playNoteOn)
            {
                dPrintf("currentNote = %d\n", currentNote);
                MidiMessage * pMsg = (MidiMessage *)arpPattern->readObject(currentNote);
                int offset = (int)pMsg->value - CONTROL_MIDI_KEY_ZERO_OFFSET;
                dPrintNoteStr(pMsg, 100);
                dPrintf("currentNote2 = %d\n", currentNote);
                //send(pMsg, 0);
                seqNote_t sNote;
                sNote.length = notePeriod / 2;
                sNote.pitch = rootNote + offset;
                sNote.velocity = pMsg->data;
                pNoteOnList->push(&sNote);
                goNoteOn(&sNote);
                currentNote++;
                if (currentNote >= len)
                {
                    currentNote = 0;
                }
            }
        }
        else
        {
            //drone beats
            seqNote_t sNote;
            sNote.length = notePeriod / 2;
            sNote.pitch = rootNote;
            sNote.velocity = 100;
            pNoteOnList->push(&sNote);
            goNoteOn(&sNote);
            //also cancel the new note flag
            rootNoteDirtyFlag = false;
        }
    }

    //move the play head -- but only if we know what zero time is!!!
    if(playing)
    {
        if(syncState != seqType::NO_ZERO_TIME)
        {
            playHead += pulseDelta;
            if((playHead > 0) && ((uint32_t)playHead > length))
            {
                //rewind to periodic spot
#ifdef PIN_DEBUG
                bool value = bspIOPinRead(DEBUG_0);
                bspIOPinWrite(DEBUG_0, !value);
#endif
                playHead %= length;
                //seq->noteList->setBookmark(seq->bookmark, 0);

#ifdef PRINT_DEBUG
                localPrintf("player rolled\n");
#endif
            }
        }
    }

    return retVal;
}

void ArpModule::setArpPattern(ArpPattern * pList)
{
    arpPattern = pList;
}

inline void ArpModule::goNoteOn(seqNote_t * pNote)
{
    controller.ledGate.setState(LEDON);
    //Convert to midi library's note format
    MidiMessage note;
    note.channel = 1;
    note.value = pNote->pitch;
    note.data = pNote->velocity;
    note.controlMask = NoteOn;
    //Output
    send(&note, 0);
}

inline void ArpModule::goNoteOff(seqNote_t * pNote)
{
    if(notesGated() == 0)
    {
        controller.ledGate.setState(LEDOFF);
    }
    //Convert to midi library's note format
    MidiMessage note;
    note.channel = 1;
    note.value = pNote->pitch;
    note.data = pNote->velocity;
    note.controlMask = NoteOff;
    //Output
    send(&note, 0);
}

void ArpModule::setNoteOnList(NoteOnList * noteOnList)
{
    pNoteOnList = noteOnList;
}

void ArpModule::setQuantizeNotePeriod(uint32_t midiTicks)
{
    localPrintf("ArpModule: q note period: %d\n", midiTicks);
    notePeriod = midiTicks;
    liveTimeQuantize.setQuantizePeriod(midiTicks);
}

void ArpModule::rest(void)
{
    dPrintf("ArpModule: Adding rest\n");
    MidiMessage msg;
    msg.channel = model.outputChannel;
    msg.value = rootNote;//this should be the last known value
    msg.data = 0;
    arpPattern->push((listItem_t *)&msg);
}

void ArpModule::clearPattern(void)
{
    if (arpPattern == NULL)
    {
        localPrintf("ArpModule: arpPattern not assigned!\n");
        return;
    }
    arpPattern->clear();
}

void ArpModule::syncToClock(int32_t midiClocks)
{
    if(syncState == seqType::LENGTH_SET)
    {
        midiClocks %= length;
    }
    localPrintf("ARP: STC: %d -> %d\n", playHead, midiClocks);
    playHead = midiClocks;
}

void ArpModule::start(int32_t t_minus_ticks)
{
    //If t_minus_ticks is negative, pattern will start in the future
    localPrintf("ARP: start in t minus %d ticks\n", t_minus_ticks);
    playHead = -t_minus_ticks;
    playing = true;
    //Set bookmarks?
}

void ArpModule::start(void)
{
    start(0);
}

uint8_t ArpModule::notesGated(void)
{
    return pNoteOnList->currentPosition;
}

void ArpModule::input(MidiMessage * pMsg, uint8_t inPort)
{
    dPrintf("ArpModule: input()\n");
    dPrintNoteStr(pMsg, inPort);
    dPrintf("\n");
    uint8_t key = pMsg->value;// short-hand
    pMsg->channel = model.outputChannel; //Force channel to msg
    //Module accppts keyboard on 0, arp control on 2
    if (inPort == 0)
    {
        //Melodic input port
        if(pMsg->controlMask == NoteOn)
        {
            rootNote = key;
            if(notesOn[key] == 1)
            {
                //already had a note on, oh well
            }
            notesOn[key] = 1;
            gatedCount++;
            if (gatedCount == 1)
            {
                //First note.
                rootNoteDirtyFlag = true;
                if (quantizeStepLength)
                {
                    // quantize the startint position
                    localPrintf("ARP: now = %d\n", playHead);
                    localPrintf("ARP: actual = %d\n", pMsg->tick);
                    //Re-stamp note
                    pMsg->tick = playHead;
                    int32_t qTime = liveTimeQuantize.quantize(pMsg->tick);
                    localPrintf("ARP: quantized = %d\n", qTime);
                    rootNoteTime = qTime - pMsg->tick;
                    localPrintf("ARP: qTime = %d\n", rootNoteTime);
                    //Configure the arpegiator
                    arp.start(rootNoteTime);
                }
            }
        }
        else if(pMsg->controlMask == NoteOff)
        {
            if(notesOn[key] == 0)
            {
                //Oh well, the key wasn't on.  Probably ok to send
            }
            notesOn[key] = 0;
            //if any notes are still on, don't un-gate
            gatedCount--;
            //TODO: Work in progress -- how to handle multi (update notepile)
            //if (key == rootNote)
            //{
                //rootNote = key;
        }

        if (!arpEnabled)
        {
            //passthrough
            send(pMsg, 0);
        }
        else
        {
            //TODO: Apply beautiful arppegiations
        }
    }
    else if (inPort == 1)
    {
        //Control port
        if (patternEntry)
        {
            if (arpPattern != NULL)
            {
                if(pMsg->controlMask == NoteOn)
                {
                    dPrintf("ArpModule: Adding note\n");
                    arpPattern->push((listItem_t *)pMsg);
                }
            }
        }
        else
        {
            //TODO: apply modifiers
        }
    }
}

void ArpModule::enableArp(bool value)
{
    arpEnabled = value;
}

void ArpModule::enablePatternEntry(bool value)
{
    patternEntry = value;
}

void ArpModule::printInfo(void)
{
    localPrintf("---ArpModule---\n");
    localPrintf("[misc]\n");
    localPrintf(" arpEnabled: %d\n", arpEnabled);
    localPrintf(" patternEntry: %d\n", patternEntry);
    localPrintf(" playHead: %d\n", playHead);
    localPrintf(" playing: %d\n", playing);
    localPrintf(" length: %d\n", length);
    localPrintf(" notePeriod: %d\n", notePeriod);
    localPrintf(" currentNote: %d\n", currentNote);
    localPrintf(" rootNote: %d\n", rootNote);
    localPrintf(" syncState: %d\n", (int)syncState);
    localPrintf("[seq list]\n");
    arpPattern->printList();
    localPrintf("[on list]\n");
    pNoteOnList->printList();
}
