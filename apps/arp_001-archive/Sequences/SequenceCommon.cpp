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
#include <stdint.h>
#include <stddef.h>
#include "SequenceCommon.h"
#include <string.h>

//**********************************************************************//
//
//  Sequence
//
PatternElement Sequence::nullStep; // only one copy needed of reference object

Sequence::Sequence( void )
{
	nullStep.value = 0;
	nullStep.velocity = 0;
	nullStep.gated = false;
}

PatternElement * Sequence::virtualStep( uint16_t inputStep )
{
	uint8_t outputStep;
	// sanitize!
	if( (stepsUsed == 0 )||(loopSequenceAtStep == 0) )
	{
		return &nullStep;
	}
	
	// Operate by loop config
	if( loopingControl == LOOPING_DISABLED )
	{
		if( inputStep >= stepsUsed )
		{
			return &nullStep;
		}
		outputStep = inputStep;
	}
	else if( loopingControl == LOOPING_AUTO )
	{
		outputStep = inputStep % stepsUsed;
	}
	else if( loopingControl == LOOPING_MANUAL )
	{
		outputStep = inputStep % loopSequenceAtStep;
		if( outputStep >= stepsUsed )
		{
			return &nullStep;
		}
	}
	// Step should be valid here
	return &step[outputStep];
}
	
//**********************************************************************//
//
//  Register
//

SequenceRegister::SequenceRegister( void )
{
	for( int i = 0; i < 10; i++ )
	{
		users[i].reference = NULL;
	}
	numberOfUsers = 0;
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
SequenceRegister * SequenceRegister::addUser( void * parentObject )
{
	if( numberOfUsers < MAX_NUMBER_OF_USERS )
	{
		// Check for existing user
		for( int i = 0; i < numberOfUsers; i++ )
		{
			if( users[i].reference == parentObject )
			{
				// Oops but it's Ok, return data.  Same as happy-path really.
				// Set fresh flag anyway
				users[i].changed = true;
				return this;
			}
		}
		//Not found in list
		users[numberOfUsers].reference = parentObject;
		numberOfUsers++;
		users[numberOfUsers].changed = true;
		return this;
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
				users[j].reference = users[j + 1].reference;
				users[j].changed = users[j + 1].changed;
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

Sequence * SequenceRegister::getSequence( void )
{
	return &data;
}
