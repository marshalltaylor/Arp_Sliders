/* Includes -- App -----------------------------------------------------------*/
#include "sequence.h"

/* Includes -- Test ----------------------------------------------------------*/
#include "sequenceTest.h"
#include "midi47fx.h"

/* References ----------------------------------------------------------------*/
#define UNUSED(expr) do { (void)(expr); } while (0)
#define SAFETY_LIMIT 10000 //Max index in case of unterminated loops

//static uint32_t subpulses = 0;
//
///* Functions -----------------------------------------------------------------*/
//static void printStamp(void)
//{
//    printf("% 4d(% 3d,% 3d,% 3d) ", subpulses, subpulses/SUBS_PER_RECORD, subpulses/SUBS_PER_PULSE, subpulses%SUBS_PER_PULSE);
//}
//
//void MidiTestFixture::sendNoteOn(DataByte inNoteNumber, DataByte inVelocity, Channel inChannel)
//{
//    printStamp();
//    printf("MIDI, Note on, note %d, velo %d, chan %d\n", inNoteNumber, inVelocity, inChannel);
//}
//
//void MidiTestFixture::sendNoteOff(DataByte inNoteNumber, DataByte inVelocity, Channel inChannel)
//{
//    printStamp();
//    printf("MIDI, Note off, note %d, velo %d, chan %d\n", inNoteNumber, inVelocity, inChannel);
//}
//
//void TestSequencePlayer::testBasic(void)
//{
//    printf("Running 4000 sub-pulses\n");
//    subpulses = 0;
//    for(int i = 0; i < 4000; i++)
//    {
//        subpulses++;
//        process(1);
//    }
//}
//
//void TestSequencePlayer::listSequence(bool listHeader, uint32_t start, uint32_t end)
//{
//    if(listHeader)
//    {
//        printf("Sequence data:\n");
//        printf(" channel = %d\n", seq->channel);
//        printf(" note = %d\n", seq->note);
//        printf(" info.gatedIndex = %d\n", seq->info.gatedIndex);
//        printf(" info.sendNoteOffs = %d\n", seq->info.sendNoteOffs);
//        printf(" info.subsToLive = %d\n", seq->info.subsToLive);
//        printf(" info.playHead = %d\n", seq->info.playHead);
//    }
//    if(end > (SEQ_LEN - 1)) end = SEQ_LEN - 1;
//    printf(" data:\n");
//    for(uint32_t i = start; i <= end; i++)
//    {
//        printf("  [%2d] 0x%02X, %3d, %f, %f\n", i, seq->data[i].flags, seq->data[i].velocity, seq->data[i].length, seq->data[i].offset);
//    }
//}
//
//void TestSequencePlayer::listSequence(void)
//{
//    listSequence(true, 0, SEQ_LEN - 1);
//}
