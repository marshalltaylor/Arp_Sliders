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
#include "sourceNoteMixer.h"
#include "Arduino.h"

//**********************************************************************//
//
//
//
//
//
//

SourceNoteMixer::SourceNoteMixer( void )
{
	
}

void SourceNoteMixer::input( uint8_t eventType, uint8_t channel, uint8_t pitch, uint8_t velocity )
{
	MidiEvent tempEvent;
	// if( channel == ...     // Filter out channels
	tempEvent.eventType = eventType;
	tempEvent.channel = channel;
	tempEvent.value = pitch;
	tempEvent.data = velocity;
	
	switch( eventType )
	{
		case 0x09: // Note on
		{
			//Search for the note.  If found, do nothing, else write
			if( inputNoteOnList.seekObjectbyNoteValue( tempEvent ) == -1 )
			{
				//note not found.
				//Mark key now on
				inputNoteOnList.pushObject( tempEvent );
				//Put to tx buffer for through
				outputNoteBuffer.pushObject( tempEvent );
				flushOutputList();
			}
			else
			{
				//Was found -- Retrigger?
				
				//do nothing
			}			
			break;
		}
		break;
		case 0x08: // Note off
		{
			//Search for the note.
			int16_t tempSeekDepth;
			tempSeekDepth = inputNoteOnList.seekObjectbyNoteValue( tempEvent );
			if( tempSeekDepth == -1 )
			{
				//not found.
				//Do nothing
				
			}
			else
			{
				//Was found.  Time for note off actions
				//Remove from key list
				inputNoteOnList.dropObject( tempSeekDepth );
				//Put to tx buffer
				outputNoteBuffer.pushObject( tempEvent );
				flushOutputList();
			}
		}
		break;
		default:
		break;
	}
	
}

void SourceNoteMixer::flushOutputList( void )
{
	listIdemNumber_t len = outputNoteBuffer.listLength();
	while( len > 0 )
	{
		listObject_t * nextOutput = outputNoteBuffer.readObject( len - 1 );
		
		char buffer[200] = {0};
		sprintf(buffer, "SM: Len = %d, Chan = %d, note = %d, %s\n", len, nextOutput->channel, nextOutput->value, (nextOutput->eventType == 0x09)?"NoteOn":"NoteOff");
		Serial6.print(buffer);
		
		outputNoteBuffer.dropObject( len - 1 );
		
		len = outputNoteBuffer.listLength();
	}

}