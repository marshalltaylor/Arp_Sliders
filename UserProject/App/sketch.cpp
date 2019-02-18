#include "Arduino.h"
#include "SlidersPanel.h"
#include "sketch.h"
#include <MIDI.h>
#include "adc_ext.h"

#include "midiTime.h"
#include "timerModule32.h"


MidiClock extMidiClock;
MidiClock intMidiClock;
MidiClockSocket clockSocket;

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

SlidersPanel mainPanel;

uint16_t debugCounter = 0;

TimerClass32 debugTimer( 1000000 ); //1 second
TimerClass32 mainPanelTimer( 10000 );
//TimerClass32 statusPanelTimer( 400 );

#if defined(__arm__)
extern "C" char* sbrk(int incr);
static int FreeStack() {
  char top = 't';
  return &top - reinterpret_cast<char*>(sbrk(0));
}
#endif

void handleClock( void )
{
	extMidiClock.incrementTick();
	//Serial6.println("Clock");
}

void handleStart( void )
{
	extMidiClock.setTickCount(0);
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
	Serial6.print("pitch: ");
	Serial6.println(pitch);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
	//digitalWrite(D6, 1);
}

void hwTimerCallback(void)
{
	// Things!
	intMidiClock.incrementTime_uS(100);
}

static void sketchTickCallback(MidiClock * caller);

extern void setup()
{
	//pinMode(D6, OUTPUT);
	Serial2.begin(9600, 6);
	Serial6.begin(12345, 1);
	//delay(2000);
	//Serial2.println("ok");
	Serial6.println("OK");
	intMidiClock.setBPM(100);
	// Write our function address into the hw timer
	//timer3TickCallback = &MidiClock::hwTimerCallback;
	timer3TickCallback = hwTimerCallback;
	//Go to fresh state
	mainPanel.reset();
	
	//clockSocket.SetBeatCallback(statusPanel.BeatCallback);
	clockSocket.SetTickCallback(sketchTickCallback);
	clockSocket.SwitchMidiClock(&extMidiClock);

	MIDI.setHandleClock(handleClock);
	MIDI.setHandleStart(handleStart);
	MIDI.setHandleContinue(handleContinue);
	MIDI.setHandleStop(handleStop);
	MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.begin(MIDI_CHANNEL_OMNI);
	
}

extern void loop()
{
	MIDI.read();
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
		//Serial6.println(mainPanel.getState());
		//Serial6.print("Playing: ");
	}
	
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
			MIDI.sendRealTime(midi::Clock);
		break;
		break;
	}

}
