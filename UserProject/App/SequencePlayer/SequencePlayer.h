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
#include "SequencePlayer.h"
#include "midiTime.h"
#include "MidiUtils.h"

// Write this player to match common sequence interfaces
// Clean up SequenceCommon!  Lots of dead code.
// 

class SequencePlayer
{
public:
	SequencePlayer(void);
	void attachMainRegister( SequenceRegister * targetRegister );
	void mainRegisterChanged( void );

	// These methods access a queue for output
	uint16_t available( void );
	void read( listIdemNumber_t * );

	// Cause the player to queue new output events, if needed
	void updateTicks( uint32_t ticks );
	
	void printDebug( void );

private:
	bool detachMainRegister( void );
	uint16_t playHeadTicks;

	MidiMessageQueue outputQueue;
	MidiMessageQueue outputNoteOn;
	
	SequenceRegister * mainRegister;

};

#endif // LOOPPLAYER_H_INCLUDED

