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

void SourceNoteMixer::input( MidiMessage * data )
{
	// if( data->channel == ...     // Filter out channels
	
	switch( data->controlMask & 0xF0 ) //Upper nibble only -- SYSEX will fail here
	{
		case NoteOn: // Note on
		{
			//Search for the note.  If found, do nothing, else write
			if( inputNoteOnList.seekObjectByNoteValue( data ) == -1 )
			{
				//note not found.
				//Mark key now on
				inputNoteOnList.pushObject( data );
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
			tempSeekDepth = inputNoteOnList.seekObjectByNoteValue( data );
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
				outputNoteBuffer.pushObject( data );
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
	mmqItemNumber_t len = outputNoteBuffer.listLength();
	while( len > 0 )
	{
		mmqObject_t * nextOutput = outputNoteBuffer.readObject( len - 1 );
		
		char buffer[200] = {0};
		sprintf(buffer, "SM: Len=%d, Mask=0x%d, Chan=%d, note=%d, 0x%X\n", len, nextOutput->controlMask, nextOutput->channel, nextOutput->value, nextOutput->data);
		Serial6.print(buffer);
		
		outputNoteBuffer.dropObject( len - 1 );
		
		len = outputNoteBuffer.listLength();
	}

}