//midiDB.h
#include "MicroLL.h"
#ifndef MIDIDB_H_INCLUDED
#define MIDIDB_H_INCLUDED

class MidiSong
{
public:
	MidiSong( void );
	void recordNote( listObject_t & );//Pass ( noteToRecord )
	uint8_t getNextNote( listObject_t &, uint32_t );//Pass ( note to make next ) Returns: play note flag
	void setRecordingTrack( uint8_t );
	uint8_t getRecordingTrack( uint8_t );
	void reset( void );
	uint8_t txLedFlagService( void );
	void clear( void );
	void clearTrack( uint8_t );
	void setPlayEnabled( uint8_t * );
	
	MicroLL track[16];
private:
	MidiEvent * playBackNote[16];
	uint8_t recordingTrack;
	uint8_t playEnable[16];
	uint8_t txLedEnableFlag;
	
};

#endif