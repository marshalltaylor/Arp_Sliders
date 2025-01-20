#ifndef SEQUENCEPLAYER_H
#define SEQUENCEPLAYER_H

#include <stdint.h>

#include "sequence.h"
#include "noteOnList.h"

typedef void (*sequenceCB_t)(bool isNoteOn, sequence_t * seq, seqNote_t * pNote);

class SequencePlayer
{
public:
    SequencePlayer(void);
    
    //Set before use
    void setNoteOnList(NoteOnList * noteOnList);
    void setUndoNoteList(NoteList * noteList);
    
    //Sequence Manipulation
    //Set before use
    void setSequence(sequence_t * sequence);
    void clearSequence(void); // reset time and sequence, not channel and stuf
    
    //Time based things
    void syncToClock(int32_t midiClocks);
    void start(void);
    void stop(void);
    void cont(void);
    
    //Work function
    bool process(uint16_t pulseDelta);
    void setCallback(sequenceCB_t inputFnPtr);

    uint8_t notesGated(void);
    int16_t getLength(void);
    
    void recordNote(seqNote_t * pNote);
    bool saveForUndo(void);
    bool undo(void);
    
    void printInfo(void);

    //Directly accessed hard-set
    void setNoteLen(int len);

private:
    bool sequenceBackupAvailable;
    int noteLen;
    //void setupNextNote(void);
    inline void goNoteOn(seqNote_t * pNote);
    inline void goNoteOff(seqNote_t * pNote);
    
    sequenceCB_t sequenceCB;
    
    NoteList * undoNoteList;
public://? TODO: get it local
    sequence_t * seq;

protected:
//public: //TODO: Make this private
    NoteOnList * pNoteOnList;
};
#endif //SEQUENCE_H
