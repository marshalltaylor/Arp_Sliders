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
#ifndef SOURCENOTEMIXER_H_INCLUDED
#define SOURCENOTEMIXER_H_INCLUDED

#include "stdint.h"
#include "MidiUtils.h"

class SourceNoteMixer
{
public:
    SourceNoteMixer( void );
	void input( MidiMessage * data );
private:
	void flushOutputList( void );
public: // For debug printer
	MidiMessageLinkedList inputNoteOnList;
	MidiMessageLinkedList outputNoteBuffer;

};

#endif // SOURCENOTEMIXER_H_INCLUDED

