#ifndef MIDICLOCKDISPLAY_H
#define MIDICLOCKDISPLAY_H
#include <stdint.h>
#include "SegmentVideo.h"
#include "timeKeeper32.h"

typedef enum displayStates
{
	init,
	idle,
	fadeIn,
	waitForFadeIn,
	waitForTimeout,
	fadeOut,
	waitForFadeOut
} displayStates_t;

class MidiClockDisplay : public SegmentVideo
{
public:
	MidiClockDisplay(void){};
	
	//Graphics
	void displayDrawClockNums( const char * input );
	void displayDrawValue( const char * input );
	void setPlayIndicator( void );
	void clearPlayIndicator( void );
	void toggleClockColon(void);
	
	void showNewValue( const char * input );
	void processEffects(void);
	bool debugNoise = false;
	
	//State machine
	void tickValueStateMachine( void );
	uint8_t getValueState( void );
	
	TimeKeeper32 displaySMTK;
private:
	bool newValueRequested = false;
	const char * currentValue;
	const char * nextValue;
	const char blankValue[4] = "   ";

	displayStates_t displayState;
};

#endif