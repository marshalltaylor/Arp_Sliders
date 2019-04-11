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
#include "outputNoteMixer.h"
#include "Arduino.h"

#include "globals.h"


//**********************************************************************//
//
//
//
//
//
//

OutputNoteMixer::OutputNoteMixer( void )
{
	
}

void OutputNoteMixer::keyboardInput( uint8_t eventType, uint8_t channel, uint8_t pitch, uint8_t velocity )
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
			if( keyboardInputNoteOnList.seekObjectbyNoteValue( tempEvent ) == -1 )
			{
				//note not found.
				//Mark key now on
				keyboardInputNoteOnList.pushObject( tempEvent );
				//Put to tx buffer for through
				//outputNoteBuffer.pushObject( tempEvent );
				//flushOutputList();
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
			tempSeekDepth = keyboardInputNoteOnList.seekObjectbyNoteValue( tempEvent );
			if( tempSeekDepth == -1 )
			{
				//not found.
				//Do nothing
				
			}
			else
			{
				//Was found.  Time for note off actions
				//Remove from key list
				keyboardInputNoteOnList.dropObject( tempSeekDepth );
				//Put to tx buffer
				//outputNoteBuffer.pushObject( tempEvent );
				//flushOutputList();
			}
		}
		break;
		default:
		break;
	}
	//Find lowest note in keyboard (if any)
	listIdemNumber_t keyListLength = keyboardInputNoteOnList.listLength();
	if( keyListLength != 0 )
	{
		int16_t lowNote = 127;
		for( int i = 0; i < keyListLength; i++ )
		{
			if( keyboardInputNoteOnList.readObject(i)->value < lowNote )
			{
				lowNote = keyboardInputNoteOnList.readObject(i)->value;
			}
		}
		// set the root
		//outputPlayer.setRoot(lowNote);
	}

}

void OutputNoteMixer::playerInput( listObject_t * obj )
{
	switch( obj->eventType )
	{
		case 0x09: // Note on
		{
			//Search for the note.  If found, do nothing, else write
			if( playerInputNoteOnList.seekObjectbyNoteValue( *obj ) == -1 )
			{
				//note not found.
				//Mark key now on
				playerInputNoteOnList.pushObject( *obj );
				//Put to tx buffer for through
				outputNoteBuffer.pushObject( *obj );
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
			tempSeekDepth = playerInputNoteOnList.seekObjectbyNoteValue( *obj );
			if( tempSeekDepth == -1 )
			{
				//not found.
				//Do nothing
				
			}
			else
			{
				//Was found.  Time for note off actions
				//Remove from key list
				playerInputNoteOnList.dropObject( tempSeekDepth );
				//Put to tx buffer
				outputNoteBuffer.pushObject( *obj );
				flushOutputList();
			}
		}
		break;
		default:
		break;
	}
	
}

void OutputNoteMixer::flushOutputList( void )
{
	listIdemNumber_t len = outputNoteBuffer.listLength();
	while( len > 0 )
	{
		listObject_t * nextOutput = outputNoteBuffer.readObject( len - 1 );
		
		char buffer[200] = {0};
		sprintf(buffer, "OM: Len = %d, Chan = %d, note = %d, %s\n", len, nextOutput->channel, nextOutput->value, (nextOutput->eventType == 0x09)?"NoteOn":"NoteOff");
		Serial6.print(buffer);
		
		switch( nextOutput->eventType )
		{
			case 0x09: //note on
			{
				MIDI.sendNoteOn(nextOutput->value, nextOutput->data, nextOutput->channel);
			}
			break;
			case 0x08: //note off
			{
				MIDI.sendNoteOff(nextOutput->value, nextOutput->data, nextOutput->channel);
			}
			break;
			default:
			break;
		}
			

		outputNoteBuffer.dropObject( len - 1 );
		
		len = outputNoteBuffer.listLength();
	}

}