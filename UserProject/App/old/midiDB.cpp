#include "midiDB.h"
#include "Arduino.h"

MidiSong::MidiSong( void )
{
	for( int i = 0; i < 16; i++ )
	{
		playBackNote[i] = track[i].startObjectPtr;
		recordingTrack = 0;
		playEnable[i] = 1;
	}
	txLedEnableFlag = 0;
};

void MidiSong::recordNote( listObject_t & noteToRecord )//Pass ( noteToRecord )
{
	track[recordingTrack].insertObjectByTime( noteToRecord );
	
}

uint8_t MidiSong::getNextNote( listObject_t & outputNote, uint32_t currentTimeTotalPulses )//Pass ( note to play next, time ) Returns: play note flag
{
	uint8_t returnVar = 0;
	uint8_t foundNoteFlag = 0;
	//Scan the tracks for a note to play
	for( int i = 0; i < 16; i++ )
	{
		//if we're looking at the null note, move on
		if((playBackNote[i] != &track[i].nullObject ) && ( foundNoteFlag == 0 ))
		{
			//Serial.println("Real object");
			//If the pulse count is greater than the next note, play it
			if( currentTimeTotalPulses >= playBackNote[i]->timeStamp )
			{
				//Only roll out the flags if not muted
				if( playEnable[i] == 1 )
				{
					//Roll out the flags
					txLedEnableFlag = 1;
					returnVar = 1;
					foundNoteFlag = 1; //We found one enabled!
					//Point the output at this note
					outputNote = *playBackNote[i];
				}

				// move to the next note always
				//note is not null, move to the next (may be null)
				//if( playBackNote[i]->nextObject != &track[i].nullObject )
				{
					playBackNote[i] = playBackNote[i]->nextObject;
				}
		
			}
			
		}
		else
		{
			//Serial.println("Null object");
		}
	}
	
	return returnVar;
}

void MidiSong::setRecordingTrack( uint8_t trackToRecord )
{
	recordingTrack = trackToRecord;
}

uint8_t MidiSong::getRecordingTrack( uint8_t )
{
	return recordingTrack;
}

void MidiSong::reset( void )
{
			for( int i = 0; i < 16; i++ )
			{
				playBackNote[i] = track[i].startObjectPtr;
				//track[i].printfMicroLL();
			}	
}

uint8_t MidiSong::txLedFlagService( void )
{
	uint8_t returnVar;
	
	returnVar = txLedEnableFlag;
	txLedEnableFlag = 0;
	
	return returnVar;
}

void MidiSong::clear( void )
{
	for( int i = 0; i < 16; i++ )
	{	
		track[i].clear();
	}
}


void MidiSong::clearTrack( uint8_t inputTrack )
{
	track[inputTrack].clear();
}

void MidiSong::setPlayEnabled( uint8_t * workingArray )
{
	for( int i = 0; i < 16; i++ )
	{
		playEnable[i] = workingArray[i] ^ 1;
		//Serial.print(playEnable[i]);
	}
	//Serial.println("");
}