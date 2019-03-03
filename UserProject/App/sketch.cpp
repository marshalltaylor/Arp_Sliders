#include "Arduino.h"
#include "SlidersPanel.h"
#include "sketch.h"
#include "adc_ext.h"

#include "timerModule32.h"

#include "globals.h"

SlidersPanel mainPanel;

uint16_t debugCounter = 0;

TimerClass32 debugTimer( 3000000 );
TimerClass32 mainPanelTimer( 10000 );
//TimerClass32 statusPanelTimer( 400 );

#if defined(__arm__)
extern "C" char* sbrk(int incr);
static int FreeStack() {
  char top = 't';
  return &top - reinterpret_cast<char*>(sbrk(0));
}
#endif

/***** Main MIDI Callbacks ****************************************************/
void handleClock( void )
{
	extMidiClock.incrementTick();
	//Serial6.println("Clock");
}

void handleStart( void )
{
	extMidiClock.setTickCount(-1);
	extMidiClock.setState(Playing);
}

void handleContinue( void )
{
	extMidiClock.setState(Playing);
}

void handleStop( void )
{
	extMidiClock.setState(Stopped);
}

	
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
	//digitalWrite(D6, 0);
	//Serial6.print("pitch: ");
	//Serial6.println(pitch);
	//controlNoteMixer.input( 0x09, channel, pitch, velocity );
	outputNoteMixer.keyboardInput( 0x09, channel, pitch, velocity );
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
	//digitalWrite(D6, 1);
	//controlNoteMixer.input( 0x08, channel, pitch, velocity );
	outputNoteMixer.keyboardInput( 0x08, channel, pitch, velocity );
}

/***** Control MIDI Callbacks *************************************************/
void handleAltClock( void )
{
	//extMidiClock.incrementTick();
	Serial6.println("Alt Clock");
}

/***** Hardware Timer Callbacks ***********************************************/
void hwTimerCallback(void)
{
	// Things!
	intMidiClock.incrementTime_uS(100);
}

/***** Clock Socket Callbacks *************************************************/
void sketchBeatCallback(MidiClock * caller)
{
	Serial6.print("....beat\n");
}

void sketchTickCallback(MidiClock * caller)
{
	//char buffer[5];
	switch(caller->getState())
	{
		case Stopped:
		{
			MIDI.sendRealTime(midi::Clock);
			//sprintf( buffer, "----" );
			//Segments.displayDrawClockNums(buffer);
			outputPlayer.updateTicks(caller->ticks);
		}
		break;
		case OutputOff:
		{
			//sprintf( buffer, "    " );
			//Segments.displayDrawClockNums(buffer);
		}
		default:
		case Paused:
		case Playing:
			//Serial6.print("ticks = ");
			//Serial6.println(caller->ticks);
			MIDI.sendRealTime(midi::Clock);
			outputPlayer.updateTicks(caller->ticks);
		break;
		break;
	}

}

/***** Program ****************************************************************/
extern void setup()
{
	//Serial2.begin(9600, 6);
	//Serial6.begin(12345, 1);
	//delay(2000);

	Serial6.println("OK");
	
	// Write our function address into the hw timer
	timer3TickCallback = hwTimerCallback;
	
	// Configure Clocks and Sockets
	intMidiClock.setBPM(100);
	clockSocket.SetBeatCallback(sketchBeatCallback);
	clockSocket.SetTickCallback(sketchTickCallback);
	clockSocket.SwitchMidiClock(&extMidiClock);
	
	extMidiClock.setState(Stopped);
	
	// Test config loop
	outputPlayer.setLoop(&loops[0]);

	// Go to fresh state
	mainPanel.reset();

	// Configure MIDI objects
	MIDI.setHandleClock(handleClock);
	MIDI.setHandleStart(handleStart);
	MIDI.setHandleContinue(handleContinue);
	MIDI.setHandleStop(handleStop);
	MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.begin(MIDI_CHANNEL_OMNI);
	MIDI.turnThruOff();
	
	CtrlMIDI.setHandleClock(handleAltClock);
    CtrlMIDI.begin(MIDI_CHANNEL_OMNI);
	CtrlMIDI.turnThruOff();
	
}

extern void loop()
{
	MIDI.read();
	CtrlMIDI.read();
	if(Serial6.available())
	{
		Serial6.println((char)Serial6.read());
	}
	if( usTicksLocked == 0 )
	{
		mainPanelTimer.update(usTicks);
		debugTimer.update(usTicks);
		//statusPanelTimer.update(usTicks);
		//Done?  Lock it back up
		usTicksLocked = 1;
	}  //The ISR will unlock.

	if(mainPanelTimer.flagStatus() == PENDING)
	{
		convertADC();
		mainPanel.tickStateMachine(10);
	}

//	if(statusPanelTimer.flagStatus() == PENDING)
//	{
//		statusPanel.tickStateMachine(400);
//		//intMidiClock.incrementTime_uS(400);
//		intMidiClock.service();
//	}
	
	if(debugTimer.flagStatus() == PENDING)
	{
		//User code
		char buffer[200] = {0};
		//sprintf(buffer, "__DEBUG______\nintPlayState = %d, extPlayState = %d\nbeatLedState = %d, playLedState = %d\nFreeStack() = %d\n\n", intMidiClock.getState(), extMidiClock.getState(), statusPanel.getBeatLedState(), statusPanel.getPlayLedState(), FreeStack());
		sprintf(buffer, "__DEBUG__\nFreeStack() = %d\n", FreeStack());
		Serial6.print(buffer);
		mainPanel.printDebug();
		extMidiClock.printDebug();
		CtrlMIDI.sendRealTime(midi::Clock);
	}
	
}
