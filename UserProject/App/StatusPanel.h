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

enum SPStates
{
	SPInit,
	SPOff,
	SPOn
};

class StatusPanel : public Panel
{
public:
	StatusPanel( void );
	void reset( void );
	void tickStateMachine( int msTicksDelta );
	uint8_t getState( void ){
		return (uint8_t)state;
	}

	MidiClockSocket * ClockSocket;
	static void BeatCallback(MidiClock *);

private:
	//Internal Panel Components
	Led ledBeat;
	Led ledPlay;
	
	//...And variables
	MessagingFlag beatFlag;
	TimeKeeper32 beatTimeKeeper;
	
	//State machine stuff  
	SPStates state;
};

#endif