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
#ifndef SEQUENCEDEBUG_H_INCLUDED
#define SEQUENCEDEBUG_H_INCLUDED
#include "MidiUtils.h"
#include "SequenceCommon.h"

class SequenceDebug
{
public:
	SequenceDebug(void);
	void attachMainRegister( SequenceRegister * targetRegister );

	// These methods access a queue for output
	void printDebug( void );

private:
	//don't think i need this
	//bool detachMainRegister( void );

	SequenceRegister * mainRegister;

};

#endif
