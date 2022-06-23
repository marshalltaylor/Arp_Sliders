#include "Arduino.h"
#include "MidiUtils.h"
#include "SequencePlayer.h"

#define Serial Serial6

//This calls member constructors
SequencePlayer::SequencePlayer(void) : outputQueue(200), outputNotesOn(127)
{
	playHeadTicks = 0;
	mainRegister = NULL;
}

void SequencePlayer::attachMainRegister( SequenceRegister * targetRegister )
{
	// Remove old association
	if( mainRegister )
	{
		mainRegister->removeUser(this);
	}
	// Subscribe to new register
	mainRegister = targetRegister->addUser(this);
}

uint16_t SequencePlayer::available( void )
{
	return outputQueue.listLength();
}

void SequencePlayer::read( MidiMessage * outputObject )
{
	if(!available())
	{
		return;
	}
	mmqObject_t * nextObject = outputQueue.readObject( 0 );
	
	outputObject->controlMask = nextObject->controlMask;
	outputObject->channel = nextObject->channel;
	outputObject->value = nextObject->value;
	outputObject->data = nextObject->data;
	
	outputQueue.dropObject(0);
}

// not sure if input is ticks from loop start or delta ticks
void SequencePlayer::updateTicks( uint32_t ticks )
{
	// Leave immediately if no register loaded
	if( mainRegister == NULL )
	{
		return;
	}

	//Get reference to sequence for next section
	Sequence * sequenceData = mainRegister->getSequence();
	uint16_t stepLength = sequenceData->ticksPerStep;
	uint16_t noteLength = stepLength / 2;
	bool newSeqStepPlaying = false;
	
	// Roll ticks
	ticks = ticks % sequenceData->tapeLengthInTicks;

	/***** This object will only play without transpose *****/
	// also note length fixed to 1/2 step for now
	
	// calculate ticks per half-setp
	uint8_t ticksPerHalfStep = sequenceData->ticksPerStep / 2;
	
	// calculate current step
	// Index to step, round to earlier
	uint32_t currentStep = ticks / sequenceData->ticksPerStep;

	// Do things if the pointed to register has changed.
	// This is pretty important to preventing glitches.  Allow notes now in progress to continue if they previously were playing
	if(mainRegister->serviceChanged(this))
	{
		// get the new currently gated note if available
		if( sequenceData->virtualStep(currentStep)->gated )
		{
			if( ticks <= (currentStep * sequenceData->ticksPerStep) + noteLength )
			{
				newSeqStepPlaying = true;
			}
			else
			{
				newSeqStepPlaying = false;
			}
		}
		//send all the note offs
		uint8_t inspectedObject = 0;
		while( inspectedObject < outputNotesOn.listLength() )
		{
			mmqObject_t * object = outputNotesOn.readObject(inspectedObject);
			if( (object->value == sequenceData->virtualStep(currentStep)->value) && newSeqStepPlaying )
			{
				// The new sequence is playing the current object's note value
				
				// step list
				inspectedObject++;
			}
			else
			{
				//Drop the note
				
				// Convert to note off
				object->controlMask = NoteOff;
				// Give to output queue
				outputQueue.pushObject(object);
				// Drop from list
				outputNotesOn.dropObject(inspectedObject);
			}
		}
	}

	// Figure out what to do
	if( ticks == currentStep * (uint32_t)sequenceData->ticksPerStep )
	{
		//note on
		mmqObject_t newNoteOn;
		newNoteOn.channel = 0;
		newNoteOn.controlMask = NoteOn;
		// get the pointed to register's sequence data
		newNoteOn.value = sequenceData->virtualStep(currentStep)->value;
		newNoteOn.data = 127;
		// gated?  other parameters?
		if( newNoteOn.value != 0 )
		{
			outputNotesOn.pushObject(&newNoteOn);
			outputQueue.pushObject(&newNoteOn);
		}
	}
	if( ticks == (currentStep * (uint32_t)sequenceData->ticksPerStep) + ticksPerHalfStep )
	{
		//note off
		
		mmqObject_t newNoteOff;
		newNoteOff.channel = 0;
		newNoteOff.controlMask = NoteOff;
		// get the pointed to register's sequence data
		newNoteOff.value = sequenceData->virtualStep(currentStep)->value;
		newNoteOff.data = 0;
		if( newNoteOff.value != 0 )
		{
			mmqItemNumber_t seekResult = outputNotesOn.seekObjectByNoteValue(&newNoteOff);
			if( seekResult != -1 )
			{
				outputNotesOn.dropObject(seekResult);
			}
			// Always send note-off, who knows what harm it will do?
			outputQueue.pushObject(&newNoteOff);
		}
	}
}

void SequencePlayer::printDebug( void )
{
	
	char buffer[200] = {0};
	sprintf(buffer, "SequencePlayer TODO: Write debug routine\n");
	Serial6.print(buffer);

}
