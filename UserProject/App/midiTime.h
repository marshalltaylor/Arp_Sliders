#ifndef MIDITIME_H
#define MIDITIME_H
#include <stdio.h>

enum PlayState_t {
	OutputOff,
	Stopped,
	Playing,
	Paused
};
	
class MidiClock
{

public:
	MidiClock(void);
	void incrementTick(void);
	void incrementTime_uS(int);
	void service(void);
	void setBPM(int);
	void setTickCount( uint32_t );
	void setState( PlayState_t );
	PlayState_t getState(void);

	//bool isTickSubDiv( uint32_t inputTicks, int8_t inputSubDiv );
	uint32_t ticksToQuarterNotes( uint32_t inputTicks );
	uint32_t ticksToMeasures( uint32_t inputTicks );

	uint32_t ticks;
	
	void processCallbacks( void );	
	//Callbacks are outputs of MidiClock 
	//Set to NULL when not in use, otherwise MidiClock calls these periodically
	void (*BeatCallback)(MidiClock *) = NULL;
	void (*TickCallback)(MidiClock *) = NULL;

private:
	PlayState_t playState;

	uint32_t timeElapsed;
	uint32_t timeTickLength;
};

class MidiClockSocket
{
public:
	MidiClockSocket(void);
	void SwitchMidiClock(MidiClock *);
	MidiClock * getSocketedClock( void );
	void SetBeatCallback(void (*name)(MidiClock *));
	void SetTickCallback(void (*name)(MidiClock *));

private:
	MidiClock * socketed;
	//These are templented into the callbacks of the clock modules, not called from this class.
	void (*BeatCallback)(MidiClock *);
	void (*TickCallback)(MidiClock *);
	
};

#endif
