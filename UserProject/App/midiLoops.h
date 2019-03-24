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

//class PatternRegister
//{
//public:
//    PatternRegister( void ){};
//	void load( PatternData * );
//	void clear( void );
//	PatternData * subscribe( void (*newSub)(void) );
//	bool unsubscribe( void (*newSub)(void) );
//
//private:
//	PatternData data;
//	// Example fn ptr:  void (*foo)(int);
//	void (*subscribers[10])(void);
//	uint8_t numSubscribed = 0;
//	
//};



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
//Note data type
struct PatternElement
{
  uint8_t value;
  uint8_t velocity;
  bool gated;
  

};

struct PatternData
{
	// Note information
	PatternElement step[40];
	bool ctrlNotes[25];
	uint8_t rootNote;
	// Time information
	uint8_t ticksPerStep;
	int16_t depth; //In notes
	int16_t overclock;
	int16_t patternLength;
	int16_t noteLength;
	int16_t subDivision;
	int16_t direction;
	bool quantize;
	bool syncDownbeat;
};


class PatternContainer
{
public:
    PatternContainer( void ){};
	// Operational
	void resetStep( void );
	int16_t incStep( void );
	int16_t getNote( void );
	void setRoot( int16_t input );
	void clearCtrlNotes( void );
	void saveCtrlNote( uint8_t input );
	PatternData * curPattern( void );
	// Generational
	void constrain( int16_t * input, int16_t lower, int16_t upper );
	void setDepth( int16_t input );
	void setOverclock( int16_t input );
	void setPatternLength( int16_t input );
	void setNoteLength( int16_t input );
	void setSubDivision( int16_t input );
	void setDirection( int16_t input );
	void setQuantize( bool input );
	void setSyncDownbeat( bool input );
	void generatePattern( void );
	void printPattern( void );
	void printControls( void );
	void generateGraphic( void );

private:
	int16_t nextStepToPlay;
	PatternData patternData;
	bool freeRunning = false;
};


#endif // MIDILOOPS_H_INCLUDED
