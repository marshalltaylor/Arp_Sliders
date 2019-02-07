#ifndef MIDITIME_H
#define MIDITIME_H
#include <stdio.h>

class MidiClock;

//class MidiClockCallbacks
//{
//public:
//	MidiClockCallbacks(void){};
//	virtual void beatCallback(MidiClock *) = 0;
//};

class MidiClock
{
public:
	MidiClock(void);
	void incrementTick(void);
	void incrementTime_uS(int);
	void service(void);
	void setBPM(int);
	void play(void);
	void enableOutput(void);
	void disableOutput(void);
	void stop(void);
	bool isTickSubDiv( uint32_t inputTicks, int8_t inputSubDiv );
	uint32_t ticksToQuarterNotes( uint32_t inputTicks );
	uint32_t ticksToMeasures( uint32_t inputTicks );
	void processCallbacks( void );	
	uint32_t ticks;
	
	void (*BeatCallback)(MidiClock *);
	//MidiClockCallbacks * midiClockCallbacks;
	static void hwTimerCallback(void);

//shouldBePrivate:
	bool isPlaying;
	bool outputEnabled = false;

private:
	uint32_t timeElapsed;
	uint32_t timeTickLength;
};

class MidiClockSocket
{
public:
	MidiClockSocket(void);
	void SwitchMidiClock(MidiClock *);
	void SetBeatCallback(void (*name)(MidiClock *));
	MidiClock * socketed;
private:
	//MidiClockCallbacks * callbacks; //These are templented into the callbacks of the clock modules, not called from this class.
	void (*BeatCallback)(MidiClock *);
};

#endif
