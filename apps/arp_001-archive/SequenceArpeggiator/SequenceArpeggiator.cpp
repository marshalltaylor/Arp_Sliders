#include "Arduino.h"
#include "MidiUtils.h"
#include "SequenceArpeggiator.h"

#define Serial Serial6

//#define ROOT_OFFSET 48
#define ROOT_OFFSET 72

//This calls member constructors
SequenceArpeggiator::SequenceArpeggiator(void) : outputQueue(200), outputNotesOn(127)
{
	playHeadTicks = 0;
	mainRegister = NULL;
}

void SequenceArpeggiator::attachMainRegister( SequenceRegister * targetRegister )
{
	// Remove old association
	if( mainRegister )
	{
		mainRegister->removeUser(this);
	}
	// Subscribe to new register
	mainRegister = targetRegister->addUser(this);
}

void SequenceArpeggiator::writeRoot( MidiMessage * input )
{
	bool foundInList = false;
	switch( input->controlMask )
	{
		case NoteOn:
		{
			// If not in list, add it
			for( mmqItemNumber_t i = inputNotesOn.listLength() - 1; i >= 0; i-- )
			{
				if( inputNotesOn.readObject(i)->value == input->value )
				{
					foundInList = true;
				}
			}
			if(!foundInList)
			{
				inputNotesOn.pushObject( input );
				root = input->value;
				rootChanged = true;
				gated = true;
			}
			break;
		}
		case NoteOff:
		{
			// Destroy all copies
			for( mmqItemNumber_t i = inputNotesOn.listLength() - 1; i >= 0; i-- )
			{
				if( inputNotesOn.readObject(i)->value == input->value )
				{
					inputNotesOn.dropObject(i);
				}
			}
			if( inputNotesOn.listLength() )
			{
				// Something is still playing, fall back
				root = input->value;
				gated = false;
				break;
			}
			gated = false;
			break;
		}
	}
}

uint16_t SequenceArpeggiator::available( void )
{
	return outputQueue.listLength();
}

void SequenceArpeggiator::read( MidiMessage * outputObject )
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
void SequenceArpeggiator::updateTicks( uint32_t ticks )
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
	
	// If root changed
	if( rootChanged )
	{
		//Drop everything in the note-on list
		//send all the note offs
		uint8_t inspectedObject = 0;
		while( inspectedObject < outputNotesOn.listLength() )
		{
			mmqObject_t * object = outputNotesOn.readObject(inspectedObject);
			//Drop the note
			
			// Convert to note off
			object->controlMask = NoteOff;
			// Give to output queue
			outputQueue.pushObject(object);
			// Drop from list
			outputNotesOn.dropObject(inspectedObject);
		}
		rootChanged = false;
	}

	// get the pointed to register's sequence data
	PatternElement * stepReference;
	stepReference = sequenceData->virtualStep(currentStep);
	// Figure out what to do for a note on tick
	if( ticks == currentStep * (uint32_t)sequenceData->ticksPerStep )
	{
		// If not gated bail.  Note offs are OK to send
		if( !gated && !drone )
		{
			return;
		}
		//note on
		mmqObject_t newNoteOn;
		newNoteOn.channel = 0;
		newNoteOn.controlMask = NoteOn;
		if((stepReference->value != 0) && (stepReference->gated))// gated?  other parameters?
		{
			int16_t calcValue = (int16_t)stepReference->value + root - ROOT_OFFSET;
			// Screen output
			if( (calcValue < 0)||(calcValue >= 127 ) )
			{
				return;
			}
			newNoteOn.value = calcValue;
			newNoteOn.data = 127;
			outputNotesOn.pushObject(&newNoteOn);
			outputQueue.pushObject(&newNoteOn);
		}
	}
	// and a note off tick
	if( ticks == (currentStep * (uint32_t)sequenceData->ticksPerStep) + ticksPerHalfStep )
	{
		//note off
		
		mmqObject_t newNoteOff;
		newNoteOff.channel = 0;
		newNoteOff.controlMask = NoteOff;
		if((stepReference->value != 0) && (stepReference->gated))// gated?  other parameters?
		{
			newNoteOff.value = stepReference->value + root - ROOT_OFFSET;
			newNoteOff.data = 0;
			mmqItemNumber_t seekResult = outputNotesOn.seekObjectByNoteValue(&newNoteOff);
			if( seekResult != -1 )
			{
				outputNotesOn.dropObject(seekResult);
				outputQueue.pushObject(&newNoteOff);
			}
			// Could always send note-off, who knows what harm it will do?
		}
	}
}

void SequenceArpeggiator::printDebug( void )
{
	
	char buffer[200] = {0};
	sprintf(buffer, "SequenceArpeggiator Brief\n");
	Serial6.print(buffer);
	sprintf(buffer, "  gated: %d\n  drone: %d\n  rootChanged: %d\n  root: %d\n", gated, drone, rootChanged, root);
	Serial6.print(buffer);
	inputNotesOn.printDebug();
	outputNotesOn.printDebug();

}

void SequenceArpeggiator::configDrone( bool input )
{
	drone = input;
}
