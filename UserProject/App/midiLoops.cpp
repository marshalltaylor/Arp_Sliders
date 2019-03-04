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
	step = 0;
}

int16_t PatternContainer::incStep( void )
{
	step++;
	if( step >= length )
	{
		step = 0;
	}
	return loopData.pattern[step];
}

int16_t PatternContainer::getNote( void )
{
	return loopData.pattern[step];
}

PatternData * PatternContainer::curPattern( void )
{
	return NULL;
}

// Generational
void PatternContainer::genMethod1( int16_t depth, int16_t subDiv, int16_t dir )
{
	
}

//	
//private:
//	int16_t step;
//	int16_t clocksPerStep;
//	int16_t msPerStep = -1;
//	int16_t length;
//	PatternData loopData;
//	bool freeRunning = false;;
//};

