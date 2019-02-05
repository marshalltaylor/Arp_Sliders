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
	PFastFlash
};

typedef enum displayStates
{
	init,
	idle,
	fadeout,
	waitForFadeout,
	fadeInSelection,
	showSelection,
	fadeOutSelection
} displayStates_t;

typedef enum selectedKnobStr
{
	SELECT_TEMPO,
	SELECT_KNOB_1,
	SELECT_KNOB_3
} selectedKnobStr_t;

class BlinkerPanel : public Panel
{
public:
	BlinkerPanel( void );
	void reset( void );
	void tickStateMachine( int msTicksDelta );
	uint8_t getState( void ){
		return (uint8_t)displayState;
	}
	void switchToInternalClock( void );
	void switchToExternalClock( void );
	
private:
	//Internal Panel Components
	Button button1;
	Button button2;
	Button button3;
	Button button4;
	Button buttonSelect;
	Button play;
	Button stop;
	Led led1;
	Led led2;
	Led led3;
	Led led4;
	Led ledPlay;
	
	Windowed10BitKnob knob1;
	Windowed10BitKnob knob3;
	Windowed10BitKnob knobTempo;

	//  ..and data
	uint8_t timeMaster;
	
	//State machine stuff  
	PStates state;
private:
	displayStates_t displayState;
	int8_t fadeLevel = 0;
	TimeKeeper32 displaySMTK;
	
	char knobTempoStr[3];
	char knob1Str[3];
	char knob3Str[3];
	
	selectedKnobStr_t knobStrShown;
	
	bool startShowingKnob = 0;
	
	//knob1/glide
	bool glideEnabled = false;
	int32_t targetBPM = 100;
	int32_t currentBPM = 100;
	int32_t glideRate = 100;

};

#endif