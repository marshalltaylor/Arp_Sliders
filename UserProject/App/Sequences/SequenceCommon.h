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

#include <stdint.h>

#define MAX_SEQUENCE_STEPS 40

#define MAX_NUMBER_OF_USERS 10

//Note data type
struct PatternElement
{
  unsigned value: 8;
  uint8_t velocity: 8;
  bool gated: 1;
};

enum LoopingControl_t
{
	LOOPING_DISABLED = 0,
	LOOPING_AUTO,
	LOOPING_MANUAL
};

//Sequence data
class Sequence
{
	static PatternElement nullStep;
public:
	Sequence( void );
	PatternElement * virtualStep( uint16_t );
	
	// Note information
	PatternElement step[MAX_SEQUENCE_STEPS];
	uint8_t stepsUsed;
	uint8_t rootNote;
	// Time information
	uint8_t ticksPerStep;
	uint16_t tapeLengthInTicks;
	// Looping behavior
	uint8_t loopSequenceAtStep;
	LoopingControl_t loopingControl;
	
	//Unused or unknown
	int16_t depth; //In notes
	int16_t overclock;
	//int8_t noteLength;
	int16_t subDivision;
	int16_t direction;
	bool quantize;
	bool syncDownbeat;
private:
};

// Used to make a list of users for a register
struct SequenceRegisterUser
{
	void * reference;
	bool changed;
};


// Register object with memory storage
class SequenceRegister
{
public:
    SequenceRegister( void );
	void load( Sequence * );
	void clear( void );
	SequenceRegister * addUser( void * parentObject );
	bool removeUser( void * parentObject );

	// This is the key to keeping in sync.  The users ask the register
	//  if the data has changed since they last asked.  This function
	//  clear's the new data flag for calling user.
	bool serviceChanged( void * parentObject );
	// get the current pointer.
	Sequence * getSequence( void );
	
	
private:
	Sequence data;
	// Example fn ptr:  void (*foo)(int);
	SequenceRegisterUser users[MAX_NUMBER_OF_USERS];
	uint8_t numberOfUsers = 0;
	
};

#endif // MIDILOOPS_H_INCLUDED
