//Header
#ifndef STATUSPANEL_H_INCLUDED
#define STATUSPANEL_H_INCLUDED

#include "Panel.h"
#include "PanelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>
#include "flagMessaging.h"
#include "timeKeeper32.h"
#include "midiTime.h"

enum BeatLedStates
{
	BeatLedStateInit,
	BeatLedStateOff,
	BeatLedStateOn
};

enum PlayLedStates
{
	PlayLedStateInit,
	PlayLedStateOff,
	PlayLedStateBlink,
	PlayLedStateOn
};

class StatusPanel : public Panel
{
public:
	StatusPanel( void );
	void reset( void );
	void tickStateMachine( int msTicksDelta );
	uint8_t getBeatLedState( void ){
		return (uint8_t)beatLedState;
	}
	uint8_t getPlayLedState( void ){
		return (uint8_t)beatLedState;
	}
	MidiClockSocket * ClockSocket;
	static void BeatCallback(MidiClock *);
	static void TickCallback(MidiClock *);

private:
	//State machine stuff  
	BeatLedStates beatLedState;
	PlayLedStates playLedState;
	//Internal Panel Components
	Led ledBeat;
	Led ledPlay;
	
	//...And variables
	MessagingFlag beatFlag;
	MessagingFlag playFlag;
	
	TimeKeeper32 beatTimeKeeper;
	TimeKeeper32 playTimeKeeper;

};

#endif