#include <stdio.h>
#include "display_clock.h"
#include "midiTime.h"
#include "interface.h"

extern MidiClock intMidiClock;


MidiClock::MidiClock(void)
{
	ticks = 0;
	timeElapsed = 0;
	timeTickLength = 0;

}

void MidiClock::incrementTime_uS(int deltaTime)
{
	timeElapsed += deltaTime;
}

void MidiClock::service(void)
{
	if(timeTickLength > 1000) // hardcode max BPM = 2500
	{
		while(timeElapsed > timeTickLength)
		{
			timeElapsed -= timeTickLength;
			incrementTick();
		}
	}		

}

void MidiClock::setBPM(int newBPM)
{
	timeTickLength = (1000000 * 60)/(newBPM * 24);
}

void MidiClock::incrementTick(void)
{
	if( playState != Paused )
	{
		ticks++;
	}
	processCallbacks();
}

void MidiClock::setTickCount( uint32_t input )
{
	ticks = input;
}

// The clock has no logic.  This configures it playing/paused etc.
void MidiClock::setState( PlayState_t input )
{
	playState = input;
}

// Things that use the clock read this to decide what to do.
PlayState_t MidiClock::getState(void)
{
	return playState;
}

uint32_t MidiClock::ticksToQuarterNotes( uint32_t inputTicks )
{
	uint32_t retVar = 0;
	retVar = ((inputTicks / 24) % 4) + 1;
	return retVar;
}

uint32_t MidiClock::ticksToMeasures( uint32_t inputTicks )
{
	uint32_t retVar = 0;
	retVar = ((inputTicks / 96) % 4) + 1;
	return retVar;
}

void MidiClock::processCallbacks( void )
{
	// Per-tick
	if(TickCallback != NULL)
	{
		TickCallback(this);
	}
	if( playState == Paused )
	{
		return;
	}
	// Down-beat
	int16_t ticksOfQuarterNote = ticks % 24;
	if( ticksOfQuarterNote == 1 )
	{
		if(BeatCallback != NULL)
		{
			BeatCallback(this);
		}
	}
}

MidiClockSocket::MidiClockSocket(void)
{
	
}

void MidiClockSocket::SwitchMidiClock(MidiClock * inputClock)
{
	if( socketed != NULL )
	{
		// Teardown current clock
		socketed->BeatCallback = NULL;
		socketed->TickCallback = NULL;
	}
	// Insert new one
	socketed = inputClock;
	if( socketed != NULL )
	{
		// set callbacks
		socketed->BeatCallback = BeatCallback;
		socketed->TickCallback = TickCallback;
	}
}

MidiClock * MidiClockSocket::getSocketedClock(void)
{
	return socketed;
}

// This sets the socket's callback
void MidiClockSocket::SetBeatCallback(void (*name)(MidiClock *))
{
	BeatCallback = name;
	//callbacks.beatCallback = name;
	//refresh socketed?
	socketed->BeatCallback = BeatCallback;
}

void MidiClockSocket::SetTickCallback(void (*name)(MidiClock *))
{
	TickCallback = name;
	socketed->TickCallback = name;
}