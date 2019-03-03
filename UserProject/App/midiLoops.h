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
#ifndef MIDILOOPS_H_INCLUDED
#define MIDILOOPS_H_INCLUDED

#include "stdint.h"
#include "MicroLL.h"

class Loop
{
public:
    Loop( void );
	MicroLL * getLoopPtr( void );
	//void clear( void );
	int16_t length;
private:
	int8_t ticksPerEvent;
	MicroLL loopData;
};

class LoopGenTest
{
public:
    LoopGenTest( void );
	void generate( Loop * output, float lengthInBeats, uint8_t subdivision );
private:
};

// Pattern based attempt

struct PatternData
{
	// Note information
	uint8_t pattern[40];
	uint8_t noteInList[128];
	uint8_t rootNote;
	// Time information
	uint8_t patternLen;
};


class PatternContainer
{
public:
    PatternContainer( void ){};
	// Operational
	void resetStep( void );
	int16_t incStep( void );
	int16_t getNote( void );
	PatternData * curPattern( void );
	// Generational
	void genMethod1( int16_t depth, int16_t subDiv, int16_t dir );
	
private:
	int16_t step;
	int16_t length;
	PatternData loopData;
	bool freeRunning = false;
};


#endif // MIDILOOPS_H_INCLUDED

