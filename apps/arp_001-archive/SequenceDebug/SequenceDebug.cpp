#include "Arduino.h"
#include "MidiUtils.h"
#include "SequenceDebug.h"

#define Serial Serial6

SequenceDebug::SequenceDebug(void)
{
	mainRegister = NULL;
}

void SequenceDebug::attachMainRegister( SequenceRegister * targetRegister )
{
	// Remove old association
	if( mainRegister )
	{
		mainRegister->removeUser(this);
	}
	// Subscribe to new register
	mainRegister = targetRegister->addUser(this);
}

void SequenceDebug::printDebug( void )
{
	if( mainRegister == NULL )
	{
		return;
	}
	Sequence * seq = mainRegister->getSequence();
	
	char buffer[200] = {0};
	
	sprintf(buffer, "SequenceDebug:\n");
	Serial6.print(buffer);
	sprintf(buffer, "step note gate velo \n-----------------\n");
	Serial6.print(buffer);
	for( int i = 0; i < seq->stepsUsed; i++ )
	{
		sprintf(buffer, "  %3d, %3d, %3d, %3d\n", i, seq->step[i].value, seq->step[i].gated, seq->step[i].velocity);
		Serial6.print(buffer);
	}
	sprintf(buffer, "-----------------\n");
	Serial6.print(buffer);
	sprintf(buffer, "Pattern length: %d\n", seq->tapeLengthInTicks);
	Serial6.print(buffer);
	sprintf(buffer, "ticksPerStep: %d\n", seq->ticksPerStep);
	Serial6.print(buffer);
}
