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
#ifndef OUTPUTNOTEMIXER_H_INCLUDED
#define OUTPUTNOTEMIXER_H_INCLUDED

#include "stdint.h"
#include "MidiUtils.h"

class OutputNoteMixer
{
public:
    OutputNoteMixer( void );
	void keyboardInput( MidiMessage * data );
	void playerInput( MidiMessage * data );
private:
	void flushOutputList( void );
public: // For debug printer
	MidiMessageLinkedList keyboardInputNoteOnList;
	MidiMessageLinkedList playerInputNoteOnList;
	MidiMessageLinkedList outputNoteBuffer;

};

#endif // OUTPUTNOTEMIXER_H_INCLUDED

