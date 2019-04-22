//Header
#ifndef SYSTEMPANEL_H_INCLUDED
#define SYSTEMPANEL_H_INCLUDED

#include "Panel.h"
#include "PanelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>
#include "timeKeeper32.h"

enum PStates
{
	PInit,
	POn,
	PFlash,
	PFastFlash,
	PRunning
};

enum MCStates
{
	MCInit,
	MCWait,
	MCIdle,
	MCRecording,
	MCClear
};

class SlidersPanel : public Panel
{
public:
	SlidersPanel( void );
	void reset( void );
	void tickStateMachine( int msTicksDelta );
	void switchToInternalClock( void );
	void switchToExternalClock( void );
	void printDebug( void );
	bool isRecording( void );
private:
	//Internal Panel Components
	Button sw1Up;
	Button sw2Up;
	Button sw3Up;
	Button sw4Up;
	Button sw5Up;
	Button sw1Down;
	Button sw2Down;
	Button sw3Down;
	Button sw4Down;
	Button sw5Down;
	
	Windowed10BitKnob slider1;
	Windowed10BitKnob slider2;
	Windowed10BitKnob slider3;
	Windowed10BitKnob slider4;
	Windowed10BitKnob slider5;
	Windowed10BitKnob slider6;
	
	int16_t lastKnob1;
	int16_t lastKnob3;
	int16_t lastKnobTempo;

	//  ..and data
	bool timeMaster;
	
	//State machine stuff  
	PStates state;
	MCStates recState;
private:
	
	char knobTempoStr[3];
	char knob1Str[3];
	char knob3Str[3];
	
	
	//knob1/glide
	bool glideEnabled = false;
	int32_t targetBPM = 100;
	int32_t currentBPM = 100;
	int32_t glideRate = 100;

};

#endif