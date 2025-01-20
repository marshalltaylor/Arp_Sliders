//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  Feb 22, 2015
//
//**********************************************************************//
#include "globals.h"

/* References ----------------------------------------------------------------*/
#include "sequence.h"
#include "inputNoteList.h"
#include "bsp.h"

#define localPrintf bspPrintf

InputNoteList::InputNoteList(listIndex_t maxLengthIn, uint16_t dataSizeIn) : LinkedList(maxLengthIn, dataSizeIn)
{
    ((seqNoteRecorded_t*)&nullObject)->note.pitch = 33;
    ((seqNoteRecorded_t*)&nullObject)->note.velocity = 0;
    ((seqNoteRecorded_t*)&nullObject)->recordedFlag = false;
}

void InputNoteList::noteOn(seqNoteRecorded_t * pNoteRec)
{
    localPrintf("in list: note: %d\n", pNoteRec->note.pitch);
    if(pitchInList(pNoteRec->note.pitch) == -1)
    {
        push(pNoteRec);
    }
}

void InputNoteList::noteOff(seqNoteRecorded_t * pNoteRec)
{
    int16_t idxNote = pitchInList(pNoteRec->note.pitch);
    if(idxNote != -1)
    {
        seqNoteRecorded_t * pOnRec = (seqNoteRecorded_t*)readObject(idxNote);
        
        //now - saved
        pNoteRec->note.time = pOnRec->note.time;
        //get the delta time
        uint16_t dTime = midiClock.ticks - pOnRec->note.time;//TODO: Bad access of member
        pNoteRec->note.length = dTime;
        pNoteRec->note.velocity = pOnRec->note.velocity;
        pNoteRec->note.accent = pOnRec->note.accent;
        //move the note time to within the loop range
        //TODO: This drops residual but we aren't using float compare yet:
        //TODO: This whole thing needs to be in a sequenc recorder
        pNoteRec->note.time = (uint32_t)pNoteRec->note.time % player.seq->length;
        //Quantize
        pNoteRec->note.time = qPattern.quantize(pNoteRec->note.time);
        pNoteRec->recordedFlag = pOnRec->recordedFlag;
        if((pNoteRec->recordedFlag) &&
            (pNoteCompleteCB != NULL))
        {
            localPrintf("note length: %d\n", (uint32_t)pNoteRec->note.length);
            pNoteCompleteCB(&pNoteRec->note);
        }
        drop(idxNote);
    }
}

void InputNoteList::setNoteCompleteCB(pNoteCompleteCB_t cb)
{
    pNoteCompleteCB = cb;
}

int16_t InputNoteList::pitchInList(uint8_t pitch)
{
    int16_t retVal = -1;

    listItemContainer_t * pObj;

    pObj = startObjectPtr;
    uint8_t i;
    //Iterate to the depth
    for (i = 0; i < currentPosition; i++)
    {
        seqNoteRecorded_t * pNoteRec = (seqNoteRecorded_t *)(pObj->data);
        if(pNoteRec->note.pitch == pitch)
        {
            //this pitch is in the list
            retVal = i;
            break;
        }
        else
        {
            pObj = pObj->next;
        }
    }
    return retVal;
}

void InputNoteList::printList(void)
{
    listItemContainer_t * tempCont;
    //our custom type
    seqNoteRecorded_t * pNoteRec;
    tempCont = startObjectPtr;
    pNoteRec = (seqNoteRecorded_t *)tempCont->data;
    localPrintf("\n%s, %s, %s, %s, %s, %s, %s, %s\n",
                "pos",
                "time",
                "len",
                "pitch",
                "velo",
                "rec",
                "adr",
                "adrNext");
    localPrintf(  "-------------------------\n");
    //Iterate to the depth
    for ( uint8_t i = 0; i < currentPosition; i++ )
    {
        localPrintf("%d, %f, %f, %d, %d, %d, %p, %p",
                    i,
                    pNoteRec->note.time,
                    pNoteRec->note.length,
                    pNoteRec->note.pitch,
                    pNoteRec->note.velocity,
                    (int)pNoteRec->recordedFlag,
                    (void*)tempCont,
                    (void*)tempCont->next);
        
        //move index
        tempCont = tempCont->next;
        pNoteRec = (seqNoteRecorded_t *)tempCont->data;
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
  localPrintf("length: %d\n", currentPosition);
  
}