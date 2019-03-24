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
//#define MICROLL_DEBUG

//Includes
#include "stdint.h"
#include "MicroLL.h"
#include "midiLoops.h"
#include "Arduino.h"
#include "globals.h"

//**********************************************************************//
//
//
//
//
//
//

Loop::Loop( void )
{
	
}

MicroLL * Loop::getLoopPtr( void )
{
	return &loopData;
}
	
//void Loop::clear( void )
//{
//	loopData.clear();
//}

LoopGenTest::LoopGenTest( void )
{
	
}

void LoopGenTest::generate( Loop * loop, float lengthInBeats, uint8_t subdivision )
{
	MicroLL * noteList = loop->getLoopPtr();
	noteList->clear();
	listObject_t tempLO;
	tempLO.channel = 1;
	
	uint16_t writeTick = 0;
	uint16_t totalPatternTicks = lengthInBeats * 24;
	loop->length = totalPatternTicks;
	uint16_t noteLengthTicks = (24 / (subdivision + 1));
	uint16_t noteOnTicks = noteLengthTicks / 2;
	uint16_t noteOffTicks = noteLengthTicks - noteOnTicks;
	while(writeTick < totalPatternTicks)
	{
		//Write a simple pattern
		tempLO.timeStamp = writeTick;
		tempLO.eventType = 0x09;
		tempLO.value = 48;
		tempLO.data = 100;
		noteList->pushObject(tempLO);
		writeTick += noteOnTicks;

		tempLO.timeStamp = writeTick;
		tempLO.eventType = 0x08;
		tempLO.data = 0;
		noteList->pushObject(tempLO);
		writeTick += noteOffTicks;


		tempLO.timeStamp = writeTick;
		tempLO.eventType = 0x09;
		tempLO.value = 60;
		tempLO.data = 100;
		noteList->pushObject(tempLO);
		writeTick += noteOnTicks;

		tempLO.timeStamp = writeTick;
		tempLO.eventType = 0x08;
		tempLO.data = 0;
		noteList->pushObject(tempLO);
		writeTick += noteOffTicks;

		
		tempLO.timeStamp = writeTick;
		tempLO.eventType = 0x09;
		tempLO.value = 72;
		tempLO.data = 100;
		noteList->pushObject(tempLO);
		writeTick += noteOnTicks;

		tempLO.timeStamp = writeTick;
		tempLO.eventType = 0x08;
		tempLO.data = 0;
		noteList->pushObject(tempLO);
		writeTick += noteOffTicks;
	}
	
	noteList->printfMicroLL();
	
}


	// Operational
void PatternContainer::resetStep( void )
{
}

int16_t PatternContainer::incStep( void )
{
	return 0;
}

int16_t PatternContainer::getNote( void )
{
	return 0;//loopData.noteValue[step];
}

PatternData * PatternContainer::curPattern( void )
{
	return &patternData;
}

void PatternContainer::clearCtrlNotes( void )
{
	for( int i = 0; i < 25; i++ )
	{
		patternData.ctrlNotes[i] = false;
	}
}

void PatternContainer::saveCtrlNote( uint8_t input )
{
	if(input > 24)
	{
		return;
	}
	patternData.ctrlNotes[input] = true;
}

void PatternContainer::setRoot( int16_t input )
{
	constrain(&input, 0, 127);
	patternData.rootNote = input;
}

// Generational

void PatternContainer::constrain( int16_t * input, int16_t lower, int16_t upper )
{
	if( *input > upper )
	{
		*input = upper;
	}
	if( *input < lower )
	{
		*input = lower;
	}
}

// Range a little over 3 octaves
void PatternContainer::setDepth( int16_t input )
{
	constrain(&input, 0, 40);
	patternData.depth = input;
}

void PatternContainer::setOverclock( int16_t input )
{
}

// Input in ticks (24 ticks/beat)
void PatternContainer::setPatternLength( int16_t input )
{
	constrain(&input, 24, 768);
	patternData.patternLength = input;
}

// Input ticks
void PatternContainer::setNoteLength( int16_t input )
{
	constrain(&input, 0, 23);
	patternData.noteLength = input;
}

void PatternContainer::setSubDivision( int16_t input )
{
	constrain(&input, 0, 2);
	patternData.subDivision = input;
	patternData.ticksPerStep = 24 / ( patternData.subDivision + 1 );
}

void PatternContainer::setDirection( int16_t input )
{
	constrain(&input, -1, 1);
	patternData.direction = input;
}

void PatternContainer::setQuantize( bool input )
{
	patternData.quantize = input;
}

void PatternContainer::setSyncDownbeat( bool input )
{
	patternData.syncDownbeat = input;
}

void PatternContainer::generatePattern( void )
{
	int16_t dir = patternData.direction;
	if( dir == 0 )
	{
		dir = 1; //Start upwards
	}
	int16_t nextNote = patternData.rootNote;
	for( int i = 0; i < ( patternData.patternLength / patternData.ticksPerStep ); i++ )
	{
		patternData.step[i].value = nextNote;
		// Calculate next note
		switch( dir )
		{
			case -1:
			nextNote = nextNote + 12;
			if( nextNote > patternData.rootNote + patternData.depth )
			{
				nextNote = patternData.rootNote;
				if( patternData.direction == 0 )
				{
					dir = 1;
				}
			}
			break;
			case 1:
			nextNote = nextNote - 12;
			if( nextNote < patternData.rootNote )
			{
				nextNote = (( patternData.depth / 12 ) * 12) + patternData.rootNote;
				if( patternData.direction == 0 )
				{
					dir = -1;
				}
			}
			break;
			default:
			break;
		}
		
	}	
}

void PatternContainer::printControls( void )
{
	char buffer[200] = {0};
	sprintf(buffer, "---Controls---\n");
	Serial6.print(buffer);
	sprintf(buffer, "  rNote: %2d dir: %3d depth: %3d, len: %3d, tkPerStep: %3d\n", patternData.rootNote, patternData.direction, patternData.depth, patternData.patternLength, patternData.ticksPerStep);
	Serial6.print(buffer);
	sprintf(buffer, "\n");
	Serial6.print(buffer);
}

void PatternContainer::printPattern( void )
{
	char buffer[200] = {0};
	int16_t lengthInSteps = patternData.patternLength / patternData.ticksPerStep;
	sprintf(buffer, "---Pattern---\n");
	Serial6.print(buffer);
	for( int i = 0; i < lengthInSteps; i++ )
	{
		sprintf(buffer, "  step[ %2d ] note: %3d\n", i, patternData.step[i].value);
		Serial6.print(buffer);
		
	}
	sprintf(buffer, "\n");
	Serial6.print(buffer);
	sprintf(buffer, "---ControlNotes---\n");
	Serial6.print(buffer);
	for( int i = 0; i < 25; i++ )
	{
		sprintf(buffer, "  key[ %2d ] state: %3d\n", i, patternData.ctrlNotes[i]);
		Serial6.print(buffer);
		
	}
	sprintf(buffer, "\n");
	Serial6.print(buffer);
}
