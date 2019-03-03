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
#include "MicroLL.h"

class OutputNoteMixer
{
public:
    OutputNoteMixer( void );
	void keyboardInput( uint8_t eventType, uint8_t channel, uint8_t pitch, uint8_t velocity );
	void playerInput( listObject_t * obj );
private:
	void flushOutputList( void );
public: // For debug printer
	MicroLL keyboardInputNoteOnList;
	MicroLL playerInputNoteOnList;
	MicroLL outputNoteBuffer;

};

#endif // OUTPUTNOTEMIXER_H_INCLUDED

