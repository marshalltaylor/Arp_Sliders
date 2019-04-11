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
#ifndef SEQUENCECOMMON_H_INCLUDED
#define SEQUENCECOMMON_H_INCLUDED

#include "stdint.h"

//Note data type
struct PatternElement
{
  unsigned value: 8;
  uint8_t velocity: 8;
  bool gated: 1;
};

//Sequence data
struct Sequence
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

// Used to make a list of users for a register
struct SequenceRegisterUser
{
	void * subscriber;
	bool changed;
};

#define MAX_NUMBER_OF_USERS 10

// Register object with memory storage
class SequenceRegister
{
public:
    SequenceRegister( void );
	void load( Sequence * );
	void clear( void );
	Sequence * addUser( void * parentObject );
	bool removeUser( void * parentObject );

	// This is the key to keeping in sync.  The users ask the register
	//  if the data has changed since they last asked.  This function
	//  clear's the new data flag for calling user.
	bool serviceChanged( void * parentObject );
	
private:
	Sequence data;
	// Example fn ptr:  void (*foo)(int);
	SequenceRegisterUser users[MAX_NUMBER_OF_USERS];
	uint8_t numberOfUsers = 0;
	
};

//class PatternContainer
//{
//public:
//    PatternContainer( void ){};
//	// Operational
//	void resetStep( void );
//	int16_t incStep( void );
//	int16_t getNote( void );
//	void setRoot( int16_t input );
//	void clearCtrlNotes( void );
//	void saveCtrlNote( uint8_t input );
//	Sequence * curPattern( void );
//	// Generational
//	void constrain( int16_t * input, int16_t lower, int16_t upper );
//	void setDepth( int16_t input );
//	void setOverclock( int16_t input );
//	void setPatternLength( int16_t input );
//	void setNoteLength( int16_t input );
//	void setSubDivision( int16_t input );
//	void setDirection( int16_t input );
//	void setQuantize( bool input );
//	void setSyncDownbeat( bool input );
//	void generatePattern( void );
//	void printPattern( void );
//	void printControls( void );
//	void generateGraphic( void );
//
//private:
//	int16_t nextStepToPlay;
//	Sequence data;
//	bool freeRunning = false;
//};


#endif // MIDILOOPS_H_INCLUDED
