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
#include "MicroLL.h"

class SourceNoteMixer
{
public:
    SourceNoteMixer( void );
	void input( uint8_t eventType, uint8_t channel, uint8_t pitch, uint8_t velocity );
private:
	void flushOutputList( void );
public: // For debug printer
	MicroLL inputNoteOnList;
	MicroLL outputNoteBuffer;

};

#endif // SOURCENOTEMIXER_H_INCLUDED

