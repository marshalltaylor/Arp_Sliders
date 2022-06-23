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
#ifndef LOOPPLAYER_H_INCLUDED
#define LOOPPLAYER_H_INCLUDED
#include "midiLoops.h"


class LoopPlayer
{
public:
	LoopPlayer(void);
//	void setLoop( Loop * input );
	void updateTicks( uint32_t ticks );
	void setRoot( int16_t );
	void setDrone( bool value );
	
	void printDebug( void );
private:
//	Loop * loaded;
	uint16_t playHeadTicks;
	uint16_t tickOfNextEvent;
	
	int16_t oldRoot;
	int16_t root;
	bool rootHasChanged;
	bool drone;

};

#endif // LOOPPLAYER_H_INCLUDED

