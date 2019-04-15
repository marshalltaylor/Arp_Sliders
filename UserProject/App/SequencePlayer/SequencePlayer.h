//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  March 23, 2019
//
//**********************************************************************//
#ifndef SEQUENCEPLAYER_H_INCLUDED
#define SEQUENCEPLAYER_H_INCLUDED
#include "MidiUtils.h"
#include "SequenceCommon.h"

// Write this player to match common sequence interfaces

class SequencePlayer
{
public:
	SequencePlayer(void);
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
	MidiMessageLinkedList outputNotesOn;
	
	SequenceRegister * mainRegister;

};

#endif
