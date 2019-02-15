//********************************************//
#include "BlinkerPanel.h"
#include "panelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>
#include "midiTime.h"
#include "StatusPanel.h"
#include "MidiClockDisplay.h"
#include <MIDI.h>
#include "midi_Defs.h"
extern midi::MidiInterface<HardwareSerial> MIDI;

extern MidiClock extMidiClock;
extern MidiClock intMidiClock;
extern MidiClockSocket clockSocket;
extern StatusPanel statusPanel;
extern MidiClockDisplay Segments;

const uint8_t FadePixelAllow[11] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t FadePixelDeny[11] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	
BlinkerPanel::BlinkerPanel( void )
{
	buttonSelect.setHardware(new ArduinoDigitalIn( D24 ), 1);
	add( &buttonSelect );
	button4.setHardware(new ArduinoDigitalIn( D25 ), 1);
	add( &button4 );
	button3.setHardware(new ArduinoDigitalIn( D26 ), 1);
	add( &button3 );
	button2.setHardware(new ArduinoDigitalIn( D27 ), 1);
	add( &button2 );
	button1.setHardware(new ArduinoDigitalIn( D28 ), 1);
	add( &button1 );
	stop.setHardware(new ArduinoDigitalIn( D29 ), 1);
	add( &stop );
	play.setHardware(new ArduinoDigitalIn( D30 ), 1);
	add( &play );

	led1.setHardware(new ArduinoDigitalOut( D32 ), 1);
	add( &led1 );
	led2.setHardware(new ArduinoDigitalOut( D31 ), 1);
	add( &led2 );
	led3.setHardware(new ArduinoDigitalOut( D33 ), 1);
	add( &led3 );
	led4.setHardware(new ArduinoDigitalOut( D10 ), 1);
	add( &led4 );

	knob1.setHardware(new ArduinoAnalogIn( A1 ));
	add( &knob1 );
	knob1.setLowerKnobVal(250);
	knob1.setUpperKnobVal(1014);
	knob1.setLowerIntVal(0);
	knob1.setUpperIntVal(100);
	knob1.setSamplesAveraged(10);

	knob3.setHardware(new ArduinoAnalogIn( A0 ));
	add( &knob3 );
	knob3.setLowerKnobVal(10);
	knob3.setUpperKnobVal(1014);
	knob3.setLowerIntVal(0);
	knob3.setUpperIntVal(255);
	knob3.setSamplesAveraged(10);

	knobTempo.setHardware(new ArduinoAnalogIn( A2 ));
	add( &knobTempo );
	knobTempo.setLowerKnobVal(10);
	knobTempo.setUpperKnobVal(1014);
	knobTempo.setLowerIntVal(40);
	knobTempo.setUpperIntVal(208);
	knobTempo.setSamplesAveraged(10);

	reset();

}

void BlinkerPanel::reset( void )
{
	//Set explicit states
	//Set all LED off
	led1.setState(LEDOFF);
	led2.setState(LEDOFF);
	led3.setState(LEDOFF);
	led4.setState(LEDOFF);
	state = PInit;
	
	// timeMaster = true; //overridden by switchTo...
	switchToInternalClock();
	
	currentBPM = knobTempo.getAsInt16() * 1000;
	targetBPM = currentBPM;
	glideRate = knob1.getAsInt16() * 1000;
	intMidiClock.setBPM(currentBPM / 1000);
	
}

void BlinkerPanel::tickStateMachine( int msTicksDelta )
{
	freshenComponents( msTicksDelta );
	Segments.displaySMTK.uIncrement( msTicksDelta * 1000 );
	//***** PROCESS THE LOGIC *****//
	MidiClock * clock = statusPanel.ClockSocket->getSocketedClock();
	//Now do the states.
	PStates nextState = state;
	switch( state )
	{
	case PInit:
		//Can't be running, if button pressed move on
		reset();
		nextState = PRunning;
		break;
	case PRunning:
		break;
	default:
		nextState = PInit;
		break;
	}
	state = nextState;

	if( button1.serviceRisingEdge() )
	{
		Serial6.println("Button1");
		glideEnabled = !glideEnabled;
		if(glideEnabled)
		{
			led1.setState(LEDON);
		}
		else
		{
			led1.setState(LEDOFF);
		}
	}
	if( button2.serviceRisingEdge() )
	{
		Serial6.println("Button2");
		led2.toggle();
		//emergencyRestart();
		Segments.debugNoise = !led2.getState();
	}
	if( button3.serviceRisingEdge() )
	{
		Serial6.println("Button3");
		led3.toggle();
	}
	if( button4.serviceRisingEdge() )
	{
		Serial6.println("Button4");
		if(!timeMaster)
		{
//			clock->stop();
			switchToInternalClock();
		}
		else
		{
			switchToExternalClock();
		}
	}
	if( buttonSelect.serviceRisingEdge() )
	{
		Serial6.println("Select");
	}
	if( play.serviceRisingEdge() )
	{
		Serial6.println("play");
		
		if( timeMaster )
		{
			switch( clock->getState() )
			{
				case OutputOff:
				case Stopped:
				{
					clock->setState(Playing);
					clock->setTickCount(0);
					MIDI.sendRealTime(midi::Start);
				}
				break;
				case Playing:
				{
					clock->setState(Paused);
					MIDI.sendRealTime(midi::Stop);
				}
				break;
				case Paused:
				{
					clock->setState(Playing);
					MIDI.sendRealTime(midi::Continue);
				}
				break;
				default:
				break;

			}
		}
		else
		{
			//Let midi do it
		}
	}
	if( stop.serviceRisingEdge() )
	{
		if( timeMaster )
		{
			switch( clock->getState() )
			{
			case Paused:
			case Playing:
			{
				clock->setState(Stopped);
				MIDI.sendRealTime(midi::Stop);
			}
			break;
			case Stopped:
			{
				clock->setState(OutputOff);
				//just in case
				MIDI.sendRealTime(midi::Stop);
			}
			break;
			default:
			break;
			}
		}
		else
		{
			//Let midi do it
		}
	}
	

	if( knob1.serviceChanged() )
	{
		Serial6.print("knob1: ");
		Serial6.print(knob1.getState());
		Serial6.print(" Val: ");
		Serial6.println(analogRead(A1));
		int16_t newValue = knob1.getAsInt16();
		if( newValue != lastKnob1 )
		{
			lastKnob1 = newValue;
			sprintf( knob1Str, "%3d", newValue );
			Segments.showNewValue(knob1Str);
			glideRate = newValue * 1000;
		}
	}
	if( knob3.serviceChanged() )
	{
		Serial6.print("knob3: ");
		Serial6.print(knob3.getState());
		Serial6.print(" Val: ");
		Serial6.println(analogRead(A2));
		int16_t newValue = knob3.getAsInt16();
		if( newValue != lastKnob3 )
		{
			lastKnob3 = newValue;
			sprintf( knob3Str, "%3d", newValue );
			Segments.showNewValue(knob3Str);
		}
	}
	if( knobTempo.serviceChanged() )
	{
		Serial6.print("knobTempo: ");
		Serial6.print(knobTempo.getState());
		Serial6.print(" Val: ");
		Serial6.println(analogRead(A2));
		int16_t newValue = knobTempo.getAsInt16();
		if( newValue != lastKnobTempo )
		{
			lastKnobTempo = newValue;		
			targetBPM = newValue * 1000;
			sprintf( knobTempoStr, "%3d", newValue );
			Segments.showNewValue(knobTempoStr);
		}
	}
	
	int32_t scaledGlide = glideRate / 100;
	if( glideEnabled )
	{
		if( currentBPM > targetBPM )
		{
			currentBPM -= scaledGlide;
			if( currentBPM < targetBPM )
			{
				currentBPM = targetBPM;
			}
			intMidiClock.setBPM(currentBPM / 1000);
			Serial6.print("(-)currentBPM: ");
			Serial6.print(currentBPM);
			Serial6.print("   targetBPM: ");
			Serial6.println(targetBPM);

		}
		if( currentBPM < targetBPM )
		{
			currentBPM += scaledGlide;
			if( currentBPM > targetBPM )
			{
				currentBPM = targetBPM;
			}
			intMidiClock.setBPM(currentBPM / 1000);
			Serial6.print("(+)currentBPM: ");
			Serial6.print(currentBPM);
			Serial6.print("   targetBPM: ");
			Serial6.println(targetBPM);
		}
	}
	else
	{
		if( currentBPM != targetBPM )
		{
			currentBPM = targetBPM;
			intMidiClock.setBPM(currentBPM / 1000);
			Serial6.print("(S)currentBPM: ");
			Serial6.print(currentBPM);
			Serial6.print("   targetBPM: ");
			Serial6.println(targetBPM);
		}
	}
}

void BlinkerPanel::switchToInternalClock( void )
{
	led4.setState(LEDON);
	timeMaster = 1;
	clockSocket.SwitchMidiClock(&intMidiClock);
}

void BlinkerPanel::switchToExternalClock( void )
{
	led4.setState(LEDOFF);
	timeMaster = 0;
	clockSocket.SwitchMidiClock(&extMidiClock);
}
