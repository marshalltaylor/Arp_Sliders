#include "Arduino.h"
#include "SlidersPanel.h"
#include "sketch.h"
#include "adc_ext.h"
#include "timerModule32.h"
#include "globals.h"

SlidersPanel mainPanel;

uint16_t debugCounter = 0;

TimerClass32 debugTimer( 3000000 );
TimerClass32 mainPanelTimer( 5000 );
TimerClass32 lcdFrameTimer( 100000 );
bool lcdFrameTimerRetick = false;

int32_t executionTimes[4] = {0};

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
	oled.setPlayHead(0);
}
	
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
	//MidiMessage newMessage;
	//newMessage.controlMask = NoteOn;
	//newMessage.channel = channel;
	//newMessage.value = pitch;
	//newMessage.data = velocity;
	////digitalWrite(D6, 0);
	//if(1)
	//{
	//	char buffer[200] = {0};
	//	sprintf(buffer, "MIDILIB: Mask=0x%d, Chan=%d, note=%d, 0x%X\n", newMessage.controlMask, newMessage.channel, newMessage.value, newMessage.data);
	//	Serial6.print(buffer);
	//}
	////controlNoteMixer.input( 0x09, channel, pitch, velocity );
	//outputNoteMixer.keyboardInput( &newMessage );
	
	MidiMessage newMsg;
	newMsg.controlMask = NoteOn;
	newMsg.channel = channel;
	newMsg.value = pitch;
	newMsg.data = velocity;
	
	//Recorder
	if( mainPanel.isRecording() )
	{
		myRecorder.recordNote(&newMsg);
		myRecorder.printDebug();
	}
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
	MidiMessage newMessage;
	newMessage.controlMask = NoteOff;
	newMessage.channel = channel;
	newMessage.value = pitch;
	newMessage.data = velocity;
	//digitalWrite(D6, 1);
	//controlNoteMixer.input( 0x08, channel, pitch, velocity );
	outputNoteMixer.keyboardInput( &newMessage );
}

void handleCtrlNoteOn(byte channel, byte pitch, byte velocity)
{
	//if((pitch < 48)||(pitch > 72))
	//{
	//	return;
	//}
	//mainPanel.inputCtrlNote(pitch - 48);
	
	/******** PORTABLE DEBUG ********/
	//char buffer[200] = {0};
	//sprintf(buffer, "sktch NoteOn: %d\n", pitch);
	//Serial6.print(buffer);	
	
	//MidiMessage newMsg;
	//newMsg.controlMask = NoteOn;
	//newMsg.channel = channel;
	//newMsg.value = pitch;
	//newMsg.data = velocity;
	//
	////Recorder
	//if( mainPanel.isRecording() )
	//{
	//	myRecorder.recordNote(&newMsg);
	//	myRecorder.printDebug();
	//}
}

void handleCtrlNoteOff(byte channel, byte pitch, byte velocity)
{
	//Should be copyish of other

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
		}
		break;
		case OutputOff:
		{
		}
		default:
		case Playing:
			oled.setPlayHead(caller->ticks);
			myPlayer.updateTicks(caller->ticks);
			if(myPlayer.available())
			{
				mmqObject_t readObject;
				myPlayer.read(&readObject);
				MIDI.send((midi::MidiType)readObject.controlMask, readObject.value, readObject.data, 1);
				Serial6.print("MIDI OUT!");
			}
		case Paused:
			Serial6.print("ticks = ");
			Serial6.println(caller->ticks);
			MIDI.sendRealTime(midi::Clock);
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
	//outputPlayer.setLoop(&loops[0]);

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
	CtrlMIDI.setHandleNoteOn(handleCtrlNoteOn);
    CtrlMIDI.setHandleNoteOff(handleCtrlNoteOff);
    CtrlMIDI.begin(MIDI_CHANNEL_OMNI);
	CtrlMIDI.turnThruOff();
	
	// Set up sequences
	sReg[REG_ARP].clear();
	myRecorder.attachMainRegister(&sReg[REG_ARP]);
	myRecorder.clearAndInit();
	myPlayer.attachMainRegister(&sReg[REG_ARP]);
	sDebug.attachMainRegister(&sReg[REG_ARP]);
	
	// Set up LCD to segment
	oled.attachMainRegister(&sReg[REG_ARP]);
	
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
		lcdFrameTimer.update(usTicks);
		//statusPanelTimer.update(usTicks);
		//Done?  Lock it back up
		usTicksLocked = 1;
	}  //The ISR will unlock.

	if(mainPanelTimer.flagStatus() == PENDING)
	{
		int32_t time = shitty_micros();
		
		convertADC();
		mainPanel.tickStateMachine(10);
		
		int32_t delta = shitty_micros() - time;
		if( delta > executionTimes[0] ) executionTimes[0] = delta;
	}

	if(lcdFrameTimer.flagStatus() == PENDING || oled.isProcessingFrame())
	{
		int32_t time = shitty_micros();
		
		if( !oled.isProcessingFrame() )
		{
			oled.drawFullScreen();
			//LCD was busy, frame was not processed at all.  hold this timer and keep checking.
			//lcdFrameTimerRetick = true;
		}
		oled.processDirectAccess();
		int32_t delta = shitty_micros() - time;
		if( delta > executionTimes[1] ) executionTimes[1] = delta;
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
		sprintf(buffer, "\n\n__DEBUG__\nFreeStack() = %d\n", FreeStack());
		Serial6.print(buffer);
		sprintf(buffer, "panel loop peak: %ld\n", executionTimes[0]);
		executionTimes[0] = 0;
		Serial6.print(buffer);
		sprintf(buffer, " oled loop peak: %ld\n", executionTimes[1]);
		executionTimes[1] = 0;
		Serial6.print(buffer);
		
		//Object debugs -- enable as needed
		
		//mainPanel.printDebug();
		//extMidiClock.printDebug();
		//sDebug.printDebug();
	
		CtrlMIDI.sendRealTime(midi::Clock);
	}
	
}
