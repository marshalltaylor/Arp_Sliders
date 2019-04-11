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
//#include "MicroLL.h"
#include "SequenceCommon.h"
//#include "Arduino.h"
//#include "globals.h"

//**********************************************************************//
//
//  Register
//

SequenceRegister::SequenceRegister( void )
{
	for( int i = 0; i < 10; i++ )
	{
		subscribers[i] = NULL;
	}
	numSubscribed = 0;
}

// Copy the referenced data to the register and alert the users
void SequenceRegister::load( Sequence * input )
{
	memcpy(&data, input, sizeof(data));
	
	for( int i = 0; i < numberOfUsers; i++ )
	{
		users[i].changed = true;
	}
}

// Turn register data to zeros and alert the users
void SequenceRegister::clear( void )
{
	memset(&data, 0, sizeof(data));

	for( int i = 0; i < numberOfUsers; i++ )
	{
		users[i].changed = true;
	}

}

// Add a user.  User gets the pointer to the register data
// The arg is called parentObject because of how the register is used.
//
//object -- player or drum or whatever
//{
////private member
//	SequenceRegister * mainRegister;
//
////method
//	
//	if( mainRegister.serviceChanged(this) )
//	{
//		...
//	}
//}
//
Sequence * SequenceRegister::addUser( void * parentObject )
{
	if( numberOfUsers < MAX_NUMBER_OF_USERS )
	{
		// Check for existing user
		for( int i = 0; i < numberOfUsers; i++ )
		{
			if( users[i].reference == parentObject )
			{
				// Oops but it's Ok, return data.  Same as happy path really.
				return &data;
			}
		}
		//Not found in list
		users[numberOfUsers] = parentObject;
		numberOfUsers++;
		return &data;
	}
	// Full, return null
	return NULL;
}

// Remove a user.  Move higher users down in index.  Returns true if found in list
bool SequenceRegister::removeUser( void * parentObject )
{
	// Walk up the array sucking out matches as you go
	// Report at the end if you found any
	bool anyUsersFound = false;
	for( int i = 0; i < numberOfUsers; i++ )
	{
		if( users[i].reference == parentObject )
		{
			// user i will be removed
			for( int j = i; j < numberOfUsers; j++ )
			{
				users[j].reference = user[j + 1].reference;
				users[j].changed = user[j + 1].changed;
			}
			numberOfUsers--;
			anyUsersFound = true;
		}
	}
	return anyUsersFound;
}

bool SequenceRegister::serviceChanged( void * parentObject )
{
	bool retVar; 
	// As soon as a user is found, return with results
	for( int i = 0; i < numberOfUsers; i++ )
	{
		if( users[i].reference == parentObject )
		{
			retVar = users[i].changed;
			users[i].changed = false;
			return retVar;
		}
	}
	return false;
}

//**********************************************************************//
//
//
//
//
//
//

//Loop::Loop( void )
//{
//	
//}
//
//MicroLL * Loop::getLoopPtr( void )
//{
//	return &loopData;
//}
	
//void Loop::clear( void )
//{
//	loopData.clear();
//}

//LoopGenTest::LoopGenTest( void )
//{
//	
//}
//
//void LoopGenTest::generate( Loop * loop, float lengthInBeats, uint8_t subdivision )
//{
//	MicroLL * noteList = loop->getLoopPtr();
//	noteList->clear();
//	listObject_t tempLO;
//	tempLO.channel = 1;
//	
//	uint16_t writeTick = 0;
//	uint16_t totalPatternTicks = lengthInBeats * 24;
//	loop->length = totalPatternTicks;
//	uint16_t noteLengthTicks = (24 / (subdivision + 1));
//	uint16_t noteOnTicks = noteLengthTicks / 2;
//	uint16_t noteOffTicks = noteLengthTicks - noteOnTicks;
//	while(writeTick < totalPatternTicks)
//	{
//		//Write a simple pattern
//		tempLO.timeStamp = writeTick;
//		tempLO.eventType = 0x09;
//		tempLO.value = 48;
//		tempLO.data = 100;
//		noteList->pushObject(tempLO);
//		writeTick += noteOnTicks;
//
//		tempLO.timeStamp = writeTick;
//		tempLO.eventType = 0x08;
//		tempLO.data = 0;
//		noteList->pushObject(tempLO);
//		writeTick += noteOffTicks;
//
//
//		tempLO.timeStamp = writeTick;
//		tempLO.eventType = 0x09;
//		tempLO.value = 60;
//		tempLO.data = 100;
//		noteList->pushObject(tempLO);
//		writeTick += noteOnTicks;
//
//		tempLO.timeStamp = writeTick;
//		tempLO.eventType = 0x08;
//		tempLO.data = 0;
//		noteList->pushObject(tempLO);
//		writeTick += noteOffTicks;
//
//		
//		tempLO.timeStamp = writeTick;
//		tempLO.eventType = 0x09;
//		tempLO.value = 72;
//		tempLO.data = 100;
//		noteList->pushObject(tempLO);
//		writeTick += noteOnTicks;
//
//		tempLO.timeStamp = writeTick;
//		tempLO.eventType = 0x08;
//		tempLO.data = 0;
//		noteList->pushObject(tempLO);
//		writeTick += noteOffTicks;
//	}
//	
//	noteList->printfMicroLL();
//	
//}


	// Operational
//void PatternContainer::resetStep( void )
//{
//}
//
//int16_t PatternContainer::incStep( void )
//{
//	return 0;
//}
//
//int16_t PatternContainer::getNote( void )
//{
//	return 0;//loopData.noteValue[step];
//}
//
//Sequence * PatternContainer::curPattern( void )
//{
//	return &data;
//}
//
//void PatternContainer::clearCtrlNotes( void )
//{
//	for( int i = 0; i < 25; i++ )
//	{
//		data.ctrlNotes[i] = false;
//	}
//}
//
//void PatternContainer::saveCtrlNote( uint8_t input )
//{
//	if(input > 24)
//	{
//		return;
//	}
//	data.ctrlNotes[input] = true;
//}
//
//void PatternContainer::setRoot( int16_t input )
//{
//	constrain(&input, 0, 127);
//	data.rootNote = input;
//}
//
//// Generational
//
//void PatternContainer::constrain( int16_t * input, int16_t lower, int16_t upper )
//{
//	if( *input > upper )
//	{
//		*input = upper;
//	}
//	if( *input < lower )
//	{
//		*input = lower;
//	}
//}
//
//// Range a little over 3 octaves
//void PatternContainer::setDepth( int16_t input )
//{
//	constrain(&input, 0, 40);
//	data.depth = input;
//}
//
//void PatternContainer::setOverclock( int16_t input )
//{
//}
//
//// Input in ticks (24 ticks/beat)
//void PatternContainer::setPatternLength( int16_t input )
//{
//	constrain(&input, 24, 768);
//	data.patternLength = input;
//}
//
//// Input ticks
//void PatternContainer::setNoteLength( int16_t input )
//{
//	constrain(&input, 0, 23);
//	data.noteLength = input;
//}
//
//void PatternContainer::setSubDivision( int16_t input )
//{
//	constrain(&input, 0, 2);
//	data.subDivision = input;
//	data.ticksPerStep = 24 / ( data.subDivision + 1 );
//}
//
//void PatternContainer::setDirection( int16_t input )
//{
//	constrain(&input, -1, 1);
//	data.direction = input;
//}
//
//void PatternContainer::setQuantize( bool input )
//{
//	data.quantize = input;
//}
//
//void PatternContainer::setSyncDownbeat( bool input )
//{
//	data.syncDownbeat = input;
//}
//
//void PatternContainer::generatePattern( void )
//{
//	int16_t dir = data.direction;
//	if( dir == 0 )
//	{
//		dir = 1; //Start upwards
//	}
//	int16_t nextNote = data.rootNote;
//	for( int i = 0; i < ( data.patternLength / data.ticksPerStep ); i++ )
//	{
//		data.step[i].value = nextNote;
//		// Calculate next note
//		switch( dir )
//		{
//			case -1:
//			nextNote = nextNote + 12;
//			if( nextNote > data.rootNote + data.depth )
//			{
//				nextNote = data.rootNote;
//				if( data.direction == 0 )
//				{
//					dir = 1;
//				}
//			}
//			break;
//			case 1:
//			nextNote = nextNote - 12;
//			if( nextNote < data.rootNote )
//			{
//				nextNote = (( data.depth / 12 ) * 12) + data.rootNote;
//				if( data.direction == 0 )
//				{
//					dir = -1;
//				}
//			}
//			break;
//			default:
//			break;
//		}
//		
//	}	
//}
//
//void PatternContainer::printControls( void )
//{
//	char buffer[200] = {0};
//	sprintf(buffer, "---Controls---\n");
//	Serial6.print(buffer);
//	sprintf(buffer, "  rNote: %2d dir: %3d depth: %3d, len: %3d, tkPerStep: %3d\n", data.rootNote, data.direction, data.depth, data.patternLength, data.ticksPerStep);
//	Serial6.print(buffer);
//	sprintf(buffer, "\n");
//	Serial6.print(buffer);
//}
//
//void PatternContainer::printPattern( void )
//{
//	char buffer[200] = {0};
//	int16_t lengthInSteps = data.patternLength / data.ticksPerStep;
//	sprintf(buffer, "---Pattern---\n");
//	Serial6.print(buffer);
//	for( int i = 0; i < lengthInSteps; i++ )
//	{
//		sprintf(buffer, "  step[ %2d ] note: %3d\n", i, data.step[i].value);
//		Serial6.print(buffer);
//		
//	}
//	sprintf(buffer, "\n");
//	Serial6.print(buffer);
//	sprintf(buffer, "---ControlNotes---\n");
//	Serial6.print(buffer);
//	for( int i = 0; i < 25; i++ )
//	{
//		sprintf(buffer, "  key[ %2d ] state: %3d\n", i, data.ctrlNotes[i]);
//		Serial6.print(buffer);
//		
//	}
//	sprintf(buffer, "\n");
//	Serial6.print(buffer);
//}
