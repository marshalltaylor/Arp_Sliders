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
#include "MidiUtils.h"
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

void OutputNoteMixer::keyboardInput( MidiMessage * data )
{
	// if( channel == ...     // Filter out channels
	
	switch( data->controlMask & 0xF0 ) //Upper nibble only -- SYSEX will fail here
	{
		case NoteOn: // Note on
		{
			//Search for the note.  If found, do nothing, else write
			if( keyboardInputNoteOnList.seekObjectByNoteValue( data ) == -1 )
			{
				//note not found.
				//Mark key now on
				keyboardInputNoteOnList.pushObject( data );
				//Put to tx buffer for through
				//outputNoteBuffer.pushObject( data );
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
		case NoteOff: // Note off
		{
			//Search for the note.
			int16_t tempSeekDepth;
			tempSeekDepth = keyboardInputNoteOnList.seekObjectByNoteValue( data );
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
				//outputNoteBuffer.pushObject( data );
				//flushOutputList();
			}
		}
		break;
		default:
		break;
	}
	//Find lowest note in keyboard (if any)
	mmqItemNumber_t keyListLength = keyboardInputNoteOnList.listLength();
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

void OutputNoteMixer::playerInput( MidiMessage * data )
{
	switch( data->controlMask & 0xF0 )
	{
		case NoteOn: // Note on
		{
			//Search for the note.  If found, do nothing, else write
			if( playerInputNoteOnList.seekObjectByNoteValue( data ) == -1 )
			{
				//note not found.
				//Mark key now on
				playerInputNoteOnList.pushObject( data );
				//Put to tx buffer for through
				outputNoteBuffer.pushObject( data );
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
		case NoteOff: // Note off
		{
			//Search for the note.
			int16_t tempSeekDepth;
			tempSeekDepth = playerInputNoteOnList.seekObjectByNoteValue( data );
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
				outputNoteBuffer.pushObject( data );
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
	mmqItemNumber_t len = outputNoteBuffer.listLength();
	while( len > 0 )
	{
		mmqObject_t * nextOutput = outputNoteBuffer.readObject( len - 1 );
		
		char buffer[200] = {0};
		sprintf(buffer, "OM: Len=%d, Mask=0x%d, Chan=%d, note=%d, 0x%X\n", len, nextOutput->controlMask, nextOutput->channel, nextOutput->value, nextOutput->data);
		Serial6.print(buffer);
		
		switch( nextOutput->controlMask )
		{
			case NoteOn: //note on
			{
				MIDI.sendNoteOn(nextOutput->value, nextOutput->data, nextOutput->channel);
			}
			break;
			case NoteOff: //note off
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