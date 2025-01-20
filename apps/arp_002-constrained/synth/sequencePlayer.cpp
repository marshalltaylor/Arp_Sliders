#include <string.h>
#include <cstdio>
#include "midi47fx.h"
#include "sequence.h"
#include "sequencePlayer.h"

#define localPrintf bspPrintf

// Enable for file debugging
//#define PRINT_DEBUG

// Enable for pin debugging
#define PIN_DEBUG
#ifdef PIN_DEBUG
#include "globals.h"
#endif

SequencePlayer::SequencePlayer(void)
{
    sequenceCB = NULL;
    seq = NULL;
    noteLen = 0;
    sequenceBackupAvailable = false;
}

void SequencePlayer::setSequence(sequence_t * sequence)
{
    bool isPlaying = seq->playing;
    //The undo data will no longer be good
    sequenceBackupAvailable = false;
    
    sequence->playing = false;

    if(isPlaying)
    {
        //stop playing
        controller.stopPlaying();
        controller.stopRecording();
    }
    seq = sequence;
    if(isPlaying)
    {
        //The previous one was playing.  Make sure the new one can
        //be playing before use
        if(seq->syncState == seqType::LENGTH_SET)
        {
            controller.startPlayingSynchronously(midiClock.ticks);
        }
    }
}

void SequencePlayer::setUndoNoteList(NoteList * noteList)
{
    sequenceBackupAvailable = false;
    undoNoteList = noteList;
}

void SequencePlayer::setCallback(sequenceCB_t inputFnPtr)
{
    sequenceCB = inputFnPtr;
}

inline void SequencePlayer::goNoteOn(seqNote_t * pNote)
{
    if(sequenceCB != NULL) sequenceCB(true, seq, pNote);
}

inline void SequencePlayer::goNoteOff(seqNote_t * pNote)
{
    if(sequenceCB != NULL) sequenceCB(false, seq, pNote);
}

void SequencePlayer::setNoteOnList(NoteOnList * noteOnList)
{
    pNoteOnList = noteOnList;
}

void SequencePlayer::clearSequence(void)
{
    seq->playHead = 0;
    seq->playing = false;
    seq->length = 10000;
    seq->noteList->clear();
    seq->syncState = seqType::NO_ZERO_TIME;
    localPrintf("SP    : clear\n");
}

void SequencePlayer::syncToClock(int32_t midiClocks)
{
    if(seq->syncState == seqType::LENGTH_SET)
    {
        midiClocks %= seq->length;
    }
    localPrintf("SP STC: %d -> %d\n", seq->playHead, midiClocks);
    seq->playHead = midiClocks;
    seq->noteList->markToTime(seq->bookmark, midiClocks);
}

void SequencePlayer::start(void)
{
    localPrintf("SP    : start\n");
    seq->playHead = 0;
    seq->playing = true;
    seq->noteList->setBookmark(seq->bookmark, 0);
}

void SequencePlayer::cont(void)
{
    seq->playing = true;
}

void SequencePlayer::stop(void)
{
    seq->playing = false;
}

uint8_t SequencePlayer::notesGated(void)
{
    return pNoteOnList->currentPosition;
}

//This function will get kinda twisted, so basically what does it do?
//Look at subpulse playhead and do something
//  is this the subpulse of the next note?
//  is a note currently playing that has been on too long?

    // I'm going go go ahead and say when notes are put in the
    // the noteOnList, their 'length' member becomes 'time to live'.
    // That way this function can pass delta into it and kill when < 0.
bool SequencePlayer::process(uint16_t pulseDelta)
{
    //Age the notes in the on list
    pNoteOnList->decrementTimes(pulseDelta);
    
    //Check for expired notes, and go accordingly
    seqNote_t note;
    while(pNoteOnList->dropExpiredNote(&note))
    {
        goNoteOff(&note);
    }
    
    //move the play head -- but only if we know what zero time is!!!
    if(seq->playing)
    {
        if(seq->syncState != seqType::NO_ZERO_TIME)
        {
            seq->playHead += pulseDelta;
            if(seq->playHead > (int16_t)seq->length)
            {
                //rewind to periodic spot
#ifdef PIN_DEBUG
                bool value = bspIOPinRead(DEBUG_0);
                bspIOPinWrite(DEBUG_0, !value);
#endif
                seq->playHead %= seq->length;
                seq->noteList->setBookmark(seq->bookmark, 0);

#ifdef PRINT_DEBUG
                localPrintf("player rolled\n");
#endif
            }
        }
    }
    bool retVal = false;
    //Check for new notes to be played and put them in the note on list
    if(seq->playing)
    {
        //The bookmark will be the next note to play.
        seqNote_t * pNote;
        
        if(seq->noteList->listLength() == 0) return false;
        if(!seq->noteList->markIsValid(seq->bookmark)) return false;
        
        bool allNotesFound = false;
        
        while(!allNotesFound)
        {
            if(!seq->noteList->markIsValid(seq->bookmark))
            {
                allNotesFound = true;
            }
            else
            {
                pNote = (seqNote_t*)seq->noteList->readBookmarked(seq->bookmark);
                if(pNote->time < seq->playHead)
                {
                    //Add to note on list
                    if(noteLen > 0)
                    {
                        pNoteOnList->pushModLen(pNote, noteLen);
                    }
                    else
                    {
                        //Original length
                        pNoteOnList->push(pNote);
                    }
                    
                    goNoteOn(pNote);
                    seq->noteList->markForward(seq->bookmark);
                    retVal = true;
                }
                else
                {
                    allNotesFound = true;
                }
            }
        } //Looking for last played note
    }
    return retVal;
    //EXPERIMENTAL CODE:  DON'T PLAY NOTES WHILE CATCHING UP
    ////Check for new notes to be played and put them in the note on list
    //if(seq->playing)
    //{
    //    //The bookmark will be the next note to play.
    //    seqNote_t * pNote;
    //    
    //    if(seq->noteList->listLength() == 0) return;
    //    if(!seq->noteList->markIsValid(seq->bookmark)) return;
    //    
    //    bool allNotesFound = false;
    //    bool validNoteFound = false;
    //    uint8_t outNoteIdx = 0; //BOOKMARK_TEMP
    //    while(!allNotesFound)
    //    {
    //        if(!seq->noteList->markIsValid(seq->bookmark))
    //        {
    //            allNotesFound = true;
    //        }
    //        else
    //        {
    //            pNote = (seqNote_t*)seq->noteList->readBookmarked(seq->bookmark);
    //            if(pNote->time < seq->playHead)
    //            {
    //                validNoteFound = true;
    //                seq->noteList->setBookmark(BOOKMARK_TEMP, seq->noteList->getBookmark(seq->bookmark));
    //
    //                seq->noteList->markForward(seq->bookmark);
    //            }
    //            else
    //            {
    //                allNotesFound = true;
    //            }
    //        } //Else valid check
    //    }
    //    if(allNotesFound)
    //    {
    //        pNote = (seqNote_t*)seq->noteList->readBookmarked(BOOKMARK_TEMP);
    //        //Add to note on list
    //        if(model.noteLen > 0)
    //        {
    //            pNoteOnList->pushModLen(pNote, model.noteLen);
    //        }
    //        else
    //        {
    //            //Original length
    //            pNoteOnList->push(pNote);
    //        }
    //        
    //        goNoteOn(pNote);
    //    }
    //}
    //
}
//#error TODO: add output length effects
int16_t SequencePlayer::getLength(void)
{
    int16_t retVal = -1;
    if(seq->syncState == seqType::LENGTH_SET)
    {
        retVal = seq->length;
    }
    return retVal;
}

void SequencePlayer::recordNote(seqNote_t * pNote)
{
    //Quantize if qCalc is enabled.
    pNote->length = qPatternHalf.quantize(pNote->length);
    if(pNote->length < 1) pNote->length = 1;
    int16_t patternLen = getLength();
    if(patternLen > 0)
    {
        if(pNote->length > patternLen)
        {
            //This makes sure long notes don't pile up
            pNote->length = patternLen - 1;
        }
    }
    else
    {
        //Length unknown!
    }

    seq->noteList->insertByTime(pNote);
    seq->noteList->markForward(seq->bookmark);
    //TODO: Allow notes to go beyond end of loop
    //(record stragglers anyway, depending on some rules?)

}

bool SequencePlayer::saveForUndo(void)
{
    if(seq->noteList->listLength() > 0)
    {
        seqcpy(undoNoteList, seq->noteList);
        sequenceBackupAvailable = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool SequencePlayer::undo(void)
{
    if(!sequenceBackupAvailable) return false;
    seqcpy(seq->noteList, undoNoteList);
    undoNoteList->clear();
    sequenceBackupAvailable = false;
    return true;
}

void SequencePlayer::setNoteLen(int len)
{
    noteLen = len;
}

void SequencePlayer::printInfo(void)
{
    localPrintf("---sequencePlayer---\n");
    localPrintf("[info]\n");
    localPrintf(" seq->channel: %d\n", seq->channel);
    localPrintf(" seq->playing: %d\n", seq->playing);
    localPrintf(" seq->playHead: %d\n", seq->playHead);
    localPrintf(" seq->length: %d\n", seq->length);
    localPrintf(" seq->syncState: %d\n", seq->syncState);
    localPrintf(" seq->noteList->markIsValid(): %d\n", seq->noteList->markIsValid(seq->bookmark));
    //localPrintf("[seq list]\n");
    //seq->noteList->printList();
    localPrintf("[on list]\n");
    pNoteOnList->printList();
}
