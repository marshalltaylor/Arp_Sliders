//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  April 16, 2019
//
//**********************************************************************//
#ifndef SEQUENCERECORDER_H_INCLUDED
#define SEQUENCERECORDER_H_INCLUDED
#include "MidiUtils.h"
#include "SequenceCommon.h"

class SequenceRecorder
{
public:
	SequenceRecorder(void);
	void attachMainRegister( SequenceRegister * targetRegister );

	// These methods access a queue for output
	void clearAndInit( void );
	void recordNote( MidiMessage * );
	void recordRest( void );
	void setTapeLengthInBeats( uint8_t );
	void setTapeLengthInTicks( uint16_t );
	void setStepLength( uint8_t ticks );
	void setLoopAtStep( uint8_t step );
	void setLoopingControl( LoopingControl_t data );
	
	void printDebug( void );

private:
	void saveLocalStorage( void );
	//don't think i need this
	//bool detachMainRegister( void );

	Sequence localStorage;
	SequenceRegister * mainRegister;

};

#endif
