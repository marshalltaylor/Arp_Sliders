//********************************************//
#include "StatusPanel.h"
#include "panelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>
#include "midiTime.h"
#include "flagMessaging.h"
#include "timeKeeper32.h"
#include "BlinkerPanel.h"
#include "SegmentVideo.h"

extern MidiClock extMidiClock;
extern MidiClock intMidiClock;

extern SegmentVideo Segments;

//extern BlinkerPanel mainPanel;
extern StatusPanel statusPanel;

void StatusPanel::BeatCallback(MidiClock * caller)
{
	char buffer[4];
	sprintf( buffer, "%2d%2d", caller->ticksToMeasures(caller->ticks), caller->ticksToQuarterNotes(caller->ticks) );
	Segments.displayDrawClockNums(buffer);
	//toggleClockColon();
	Serial6.println("  *");	

	statusPanel.beatFlag.setFlag();
	statusPanel.beatFlag.clearFlag();

}

StatusPanel::StatusPanel( void )
{
	ledBeat.setHardware(new ArduinoDigitalOut( A5 ), 1);
	add( &ledBeat );
	ledPlay.setHardware(new ArduinoDigitalOut( D34 ), 1);
	add( &ledPlay );

	reset();
	//freshenComponents( 1 );
}

void StatusPanel::reset( void )
{
	//Set explicit states
	//Set all LED off
	ledBeat.setState(LEDOFF);
	ledPlay.setState(LEDOFF);
	state = SPInit;
	ClockSocket->socketed = NULL;
	
}

void StatusPanel::tickStateMachine( int usTicksDelta )
{
	beatTimeKeeper.uIncrement( usTicksDelta );
	
	// I don't think LEDs use the value here, normally you would pass msTicksDelta
	freshenComponents( 1 );
	
	//***** PROCESS THE LOGIC *****//
	//Now do the states.
	SPStates nextState = state;
	switch( state )
	{
	case SPInit:
		nextState = SPOff;
		break;
	case SPOff:
		if( beatFlag.serviceRisingEdge() )
		{
			nextState = SPOn;
			ledBeat.setState(LEDON);
			beatTimeKeeper.uClear();
		}
		break;
	case SPOn:
		if( beatTimeKeeper.uGet() > 10000 )
		{
			nextState = SPOff;
			ledBeat.setState(LEDOFF);
		}
		break;
	default:
		nextState = SPInit;
		break;
	}
	state = nextState;

	if( ClockSocket->socketed->isPlaying )
	{
		ledPlay.setState(LEDON);
	}
	else
	{
		ledPlay.setState(LEDOFF);
	}
}
