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

enum PlayState_t {
	OutputOff,
	Stopped,
	Playing,
	Paused
};
	
class MidiClock
{

//enum {
//	Stop,
//	Play,
//	Continue
//} Command_t;	
public:
	MidiClock(void);
	void incrementTick(void);
	void incrementTime_uS(int);
	void service(void);
	void setBPM(int);
	void enableOutput(void);
	void disableOutput(void);
	void setTickCount( uint32_t );
	void setState( PlayState_t );
	PlayState_t getState(void);

	//bool isTickSubDiv( uint32_t inputTicks, int8_t inputSubDiv );
	uint32_t ticksToQuarterNotes( uint32_t inputTicks );
	uint32_t ticksToMeasures( uint32_t inputTicks );
	void processCallbacks( void );	
	uint32_t ticks;
	
	//Callbacks are outputs of MidiClock 
	//Set to NULL when not in use, otherwise MidiClock calls these periodically
	void (*BeatCallback)(MidiClock *) = NULL;
	void (*TickCallback)(MidiClock *) = NULL;

	//MidiClockCallbacks * midiClockCallbacks;
	static void hwTimerCallback(void);

//shouldBePrivate:
	//bool isPlaying;
	bool outputEnabled = true;

	PlayState_t playState;

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
	void SetTickCallback(void (*name)(MidiClock *));
	//void tickPlayStateMachine(void);
	MidiClock * getSocketedClock(void);

	MidiClock * socketed;
private:
	//MidiClockCallbacks * callbacks; //These are templented into the callbacks of the clock modules, not called from this class.
	void (*BeatCallback)(MidiClock *);
	void (*TickCallback)(MidiClock *);
	
};

#endif
