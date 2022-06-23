//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  April 11, 2019
//
//**********************************************************************//
#ifndef SEQUENCEGENERATORSLIDERS_H_INCLUDED
#define SEQUENCEGENERATORSLIDERS_H_INCLUDED
#include "SequenceGeneratorSliders.h"
#include "midiTime.h"
#include "MidiUtils.h"



   ///// NOT INCLUDED IN BUILD

of course this needs to be done

then attach it to a midi input to start testing.

After that, write the microview to be able to display any sequence.

class SequenceGeneratorSliders
{
public:
	SequenceGeneratorSliders(void);
	void attachMainRegister( SequenceRegister * targetRegister );

	// These methods access a queue for output
	uint16_t available( void );
	void read( MidiMessage * );

	// Cause the player to queue new output events, if needed
	// ticks are midi ticks... is this how I want it?
	void updateTicks( uint32_t ticks );
	
	void printDebug( void );

private:
	//don't think i need this
	//bool detachMainRegister( void );
	uint16_t playHeadTicks;

	MidiMessageQueue outputQueue;
	MidiMessageQueue outputNoteOn;
	
	SequenceRegister * mainRegister;

};

//class PatternContainer
//{
//public:
//    PatternContainer( void ){};
//	// Operational
//	void resetStep( void );
//	int16_t incStep( void );
//	int16_t getNote( void );
//	void setRoot( int16_t input );
//	void clearCtrlNotes( void );
//	void saveCtrlNote( uint8_t input );
//	Sequence * curPattern( void );
//	// Generational
//	void constrain( int16_t * input, int16_t lower, int16_t upper );
//	void setDepth( int16_t input );
//	void setOverclock( int16_t input );
//	void setPatternLength( int16_t input );
//	void setNoteLength( int16_t input );
//	void setSubDivision( int16_t input );
//	void setDirection( int16_t input );
//	void setQuantize( bool input );
//	void setSyncDownbeat( bool input );
//	void generatePattern( void );
//	void printPattern( void );
//	void printControls( void );
//	void generateGraphic( void );
//
//private:
//	int16_t nextStepToPlay;
//	Sequence data;
//	bool freeRunning = false;
//};

#endif // SEQUENCEGENERATORSLIDERS_H_INCLUDED

