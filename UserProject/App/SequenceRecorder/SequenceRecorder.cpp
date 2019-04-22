#include "Arduino.h"
#include "MidiUtils.h"
#include "SequenceRecorder.h"

#define Serial Serial6

SequenceRecorder::SequenceRecorder(void)
{
	memset(&localStorage, 0, sizeof(localStorage));
	mainRegister = NULL;
}

void SequenceRecorder::attachMainRegister( SequenceRegister * targetRegister )
{
	// Remove old association
	if( mainRegister )
	{
		mainRegister->removeUser(this);
	}
	// Subscribe to new register
	mainRegister = targetRegister->addUser(this);
}

void SequenceRecorder::recordNote( MidiMessage * msg )
{
	char buffer[200] = {0};
	
	sprintf(buffer, "rec: msg->controlMask: 0x%d\n",msg->controlMask);
	Serial6.print(buffer);
	if(( msg->controlMask & 0xF0 ) == NoteOn )
	{
		if( localStorage.stepsUsed < MAX_SEQUENCE_STEPS )
		{
			localStorage.step[localStorage.stepsUsed].value = msg->value;
			localStorage.step[localStorage.stepsUsed].gated = true;
			localStorage.step[localStorage.stepsUsed].velocity = msg->data;
			localStorage.stepsUsed++;
			saveLocalStorage();
		}
	}
}

void SequenceRecorder::recordRest( void )
{
	if( localStorage.stepsUsed < MAX_SEQUENCE_STEPS )
	{
		localStorage.step[localStorage.stepsUsed].value = localStorage.step[localStorage.stepsUsed - 1].value;
		localStorage.step[localStorage.stepsUsed].gated = true;
		localStorage.step[localStorage.stepsUsed].velocity = localStorage.step[localStorage.stepsUsed - 1].velocity;
		localStorage.stepsUsed++;
	}
}

void SequenceRecorder::saveLocalStorage( void )
{
	if( mainRegister != NULL )
	{
		mainRegister->load(&localStorage);
	}
}

void SequenceRecorder::clearAndInit( void )
{
	memset(&localStorage, 0, sizeof(localStorage));
	localStorage.ticksPerStep = 24; // pattern 16 steps per 4 bars
	// Think about this.  If there are 24 ticks per beat
	// How do we set beats per step?  
	//   is ticksPerStep good enough?
	
	localStorage.tapeLengthInTicks = 16 * 24;
	localStorage.loopSequenceAtStep = 16;
	localStorage.loopingControl = LOOPING_AUTO;
	// Should explain all of these parameters somewhere -- sequence common probably
	
	saveLocalStorage();
}

void SequenceRecorder::setTapeLengthInBeats( uint8_t newLen )
{
	localStorage.tapeLengthInTicks = newLen * 24;
	saveLocalStorage();
}

void SequenceRecorder::setStepLength( uint8_t ticks )
{
	localStorage.ticksPerStep = ticks;
	saveLocalStorage();
}

void SequenceRecorder::setLoopAtStep( uint8_t step )
{
	localStorage.loopSequenceAtStep = step;
	saveLocalStorage();
}

void SequenceRecorder::setLoopingControl( LoopingControl_t data )
{
	localStorage.loopingControl = data;
	saveLocalStorage();
}

void SequenceRecorder::printDebug( void )
{
	
	char buffer[200] = {0};
	
	sprintf(buffer, "SequenceRecorder:\n");
	Serial6.print(buffer);
	sprintf(buffer, "step note gate velo \n-----------------\n");
	Serial6.print(buffer);
	for( int i = 0; i < localStorage.stepsUsed; i++ )
	{
		sprintf(buffer, "  %3d, %3d, %3d, %3d\n", i, localStorage.step[i].value, localStorage.step[i].gated, localStorage.step[i].velocity);
		Serial6.print(buffer);
	}
	sprintf(buffer, "\n");
	Serial6.print(buffer);
	sprintf(buffer, "Pattern length: %d\n", localStorage.tapeLengthInTicks);
	Serial6.print(buffer);
}
