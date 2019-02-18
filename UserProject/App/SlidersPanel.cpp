//********************************************//
#include "SlidersPanel.h"
#include "panelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>
#include "midiTime.h"
#include <MIDI.h>
#include "midi_Defs.h"
extern midi::MidiInterface<HardwareSerial> MIDI;

extern MidiClock extMidiClock;
extern MidiClock intMidiClock;
extern MidiClockSocket clockSocket;
	
SlidersPanel::SlidersPanel( void )
{
	sw1Up.setHardware(new ArduinoDigitalIn( D24 ), 1);
	sw2Up.setHardware(new ArduinoDigitalIn( D25 ), 1);
	sw3Up.setHardware(new ArduinoDigitalIn( D26 ), 1);
	sw4Up.setHardware(new ArduinoDigitalIn( D27 ), 1);
	sw5Up.setHardware(new ArduinoDigitalIn( D28 ), 1);
	sw1Down.setHardware(new ArduinoDigitalIn( D29 ), 1);
	sw2Down.setHardware(new ArduinoDigitalIn( D30 ), 1);
	sw3Down.setHardware(new ArduinoDigitalIn( D32 ), 1);
	sw4Down.setHardware(new ArduinoDigitalIn( D33 ), 1);
	sw5Down.setHardware(new ArduinoDigitalIn( D31 ), 1);
	add( &sw1Down );
	add( &sw2Down );
	add( &sw3Down );
	add( &sw4Down );
	add( &sw5Down );
	add( &sw1Up );
	add( &sw2Up );
	add( &sw3Up );
	add( &sw4Up );
	add( &sw5Up );
	
	//button4.setHardware(new ArduinoDigitalIn( D25 ), 1);
	//add( &button4 );
	//button3.setHardware(new ArduinoDigitalIn( D26 ), 1);
	//add( &button3 );
	//button2.setHardware(new ArduinoDigitalIn( D27 ), 1);
	//add( &button2 );
	//button1.setHardware(new ArduinoDigitalIn( D28 ), 1);
	//add( &button1 );
	//stop.setHardware(new ArduinoDigitalIn( D29 ), 1);
	//add( &stop );
	//play.setHardware(new ArduinoDigitalIn( D30 ), 1);
	//add( &play );
    //
	//led1.setHardware(new ArduinoDigitalOut( D32 ), 1);
	//add( &led1 );
	//led2.setHardware(new ArduinoDigitalOut( D31 ), 1);
	//add( &led2 );
	//led3.setHardware(new ArduinoDigitalOut( D33 ), 1);
	//add( &led3 );
	//led4.setHardware(new ArduinoDigitalOut( D10 ), 1);
	//add( &led4 );

	slider1.setHardware(new ArduinoAnalogIn( A0 ));
	add( &slider1 );
	slider1.setLowerKnobVal(250);
	slider1.setUpperKnobVal(1014);
	slider1.setLowerIntVal(0);
	slider1.setUpperIntVal(100);
	slider1.setSamplesAveraged(10);

	slider2.setHardware(new ArduinoAnalogIn( A1 ));
	add( &slider2 );
	slider2.setLowerKnobVal(10);
	slider2.setUpperKnobVal(1014);
	slider2.setLowerIntVal(0);
	slider2.setUpperIntVal(255);
	slider2.setSamplesAveraged(10);

	slider3.setHardware(new ArduinoAnalogIn( A2 ));
	add( &slider3 );
	slider3.setLowerKnobVal(10);
	slider3.setUpperKnobVal(1014);
	slider3.setLowerIntVal(40);
	slider3.setUpperIntVal(208);
	slider3.setSamplesAveraged(10);

	slider4.setHardware(new ArduinoAnalogIn( A3 ));
	add( &slider4 );
	slider4.setLowerKnobVal(250);
	slider4.setUpperKnobVal(1014);
	slider4.setLowerIntVal(0);
	slider4.setUpperIntVal(100);
	slider4.setSamplesAveraged(10);

	slider5.setHardware(new ArduinoAnalogIn( A4 ));
	add( &slider5 );
	slider5.setLowerKnobVal(10);
	slider5.setUpperKnobVal(1014);
	slider5.setLowerIntVal(0);
	slider5.setUpperIntVal(255);
	slider5.setSamplesAveraged(10);

	slider6.setHardware(new ArduinoAnalogIn( A5 ));
	add( &slider6 );
	slider6.setLowerKnobVal(10);
	slider6.setUpperKnobVal(1014);
	slider6.setLowerIntVal(40);
	slider6.setUpperIntVal(208);
	slider6.setSamplesAveraged(10);

	reset();

}

void SlidersPanel::reset( void )
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
	
	currentBPM = 100;// knobTempo.getAsInt16() * 1000;
	targetBPM = currentBPM;
	glideRate = 50; //knob1.getAsInt16() * 1000;
	intMidiClock.setBPM(currentBPM / 1000);
	
}

void SlidersPanel::tickStateMachine( int msTicksDelta )
{
	freshenComponents( msTicksDelta );
	//***** PROCESS THE LOGIC *****//
//	MidiClock * clock = statusPanel.ClockSocket->getSocketedClock();
//	//Now do the states.
//	PStates nextState = state;
//	switch( state )
//	{
//	case PInit:
//		//Can't be running, if button pressed move on
//		reset();
//		nextState = PRunning;
//		break;
//	case PRunning:
//		break;
//	default:
//		nextState = PInit;
//		break;
//	}
//	state = nextState;
//
//	if( button1.serviceRisingEdge() )
//	{
//		Serial6.println("Button1");
//		glideEnabled = !glideEnabled;
//		if(glideEnabled)
//		{
//			led1.setState(LEDON);
//		}
//		else
//		{
//			led1.setState(LEDOFF);
//		}
//	}
//	if( button2.serviceRisingEdge() )
//	{
//		Serial6.println("Button2");
//		led2.toggle();
//		//emergencyRestart();
//	}
//	if( button3.serviceRisingEdge() )
//	{
//		Serial6.println("Button3");
//		led3.toggle();
//	}
//	if( button4.serviceRisingEdge() )
//	{
//		Serial6.println("Button4");
//		if(!timeMaster)
//		{
////			clock->stop();
//			switchToInternalClock();
//		}
//		else
//		{
//			switchToExternalClock();
//		}
//	}
//	if( buttonSelect.serviceRisingEdge() )
//	{
//		Serial6.println("Select");
//	}
//	if( play.serviceRisingEdge() )
//	{
//		Serial6.println("play");
//		
//		if( timeMaster )
//		{
//			switch( clock->getState() )
//			{
//				case OutputOff:
//				case Stopped:
//				{
//					clock->setState(Playing);
//					clock->setTickCount(0);
//					MIDI.sendRealTime(midi::Start);
//				}
//				break;
//				case Playing:
//				{
//					clock->setState(Paused);
//					MIDI.sendRealTime(midi::Stop);
//				}
//				break;
//				case Paused:
//				{
//					clock->setState(Playing);
//					MIDI.sendRealTime(midi::Continue);
//				}
//				break;
//				default:
//				break;
//
//			}
//		}
//		else
//		{
//			//Let midi do it
//		}
//	}
//	if( stop.serviceRisingEdge() )
//	{
//		if( timeMaster )
//		{
//			switch( clock->getState() )
//			{
//			case Paused:
//			case Playing:
//			{
//				clock->setState(Stopped);
//				MIDI.sendRealTime(midi::Stop);
//			}
//			break;
//			case Stopped:
//			{
//				clock->setState(OutputOff);
//				//just in case
//				MIDI.sendRealTime(midi::Stop);
//			}
//			break;
//			default:
//			break;
//			}
//		}
//		else
//		{
//			//Let midi do it
//		}
//	}
//	
//
//	if( knob1.serviceChanged() )
//	{
//		Serial6.print("knob1: ");
//		Serial6.print(knob1.getState());
//		Serial6.print(" Val: ");
//		Serial6.println(analogRead(A1));
//		int16_t newValue = knob1.getAsInt16();
//		if( newValue != lastKnob1 )
//		{
//			lastKnob1 = newValue;
//			sprintf( knob1Str, "%3d", newValue );
//			Segments.showNewValue(knob1Str);
//			glideRate = newValue * 1000;
//		}
//	}
//	if( knob3.serviceChanged() )
//	{
//		Serial6.print("knob3: ");
//		Serial6.print(knob3.getState());
//		Serial6.print(" Val: ");
//		Serial6.println(analogRead(A2));
//		int16_t newValue = knob3.getAsInt16();
//		if( newValue != lastKnob3 )
//		{
//			lastKnob3 = newValue;
//			sprintf( knob3Str, "%3d", newValue );
//			Segments.showNewValue(knob3Str);
//		}
//	}
//	if( knobTempo.serviceChanged() )
//	{
//		Serial6.print("knobTempo: ");
//		Serial6.print(knobTempo.getState());
//		Serial6.print(" Val: ");
//		Serial6.println(analogRead(A2));
//		int16_t newValue = knobTempo.getAsInt16();
//		if( newValue != lastKnobTempo )
//		{
//			lastKnobTempo = newValue;		
//			targetBPM = newValue * 1000;
//			sprintf( knobTempoStr, "%3d", newValue );
//			Segments.showNewValue(knobTempoStr);
//		}
//	}
//	
//	int32_t scaledGlide = glideRate / 100;
//	if( glideEnabled )
//	{
//		if( currentBPM > targetBPM )
//		{
//			currentBPM -= scaledGlide;
//			if( currentBPM < targetBPM )
//			{
//				currentBPM = targetBPM;
//			}
//			intMidiClock.setBPM(currentBPM / 1000);
//			Serial6.print("(-)currentBPM: ");
//			Serial6.print(currentBPM);
//			Serial6.print("   targetBPM: ");
//			Serial6.println(targetBPM);
//
//		}
//		if( currentBPM < targetBPM )
//		{
//			currentBPM += scaledGlide;
//			if( currentBPM > targetBPM )
//			{
//				currentBPM = targetBPM;
//			}
//			intMidiClock.setBPM(currentBPM / 1000);
//			Serial6.print("(+)currentBPM: ");
//			Serial6.print(currentBPM);
//			Serial6.print("   targetBPM: ");
//			Serial6.println(targetBPM);
//		}
//	}
//	else
//	{
//		if( currentBPM != targetBPM )
//		{
//			currentBPM = targetBPM;
//			intMidiClock.setBPM(currentBPM / 1000);
//			Serial6.print("(S)currentBPM: ");
//			Serial6.print(currentBPM);
//			Serial6.print("   targetBPM: ");
//			Serial6.println(targetBPM);
//		}
//	}
}

void SlidersPanel::switchToInternalClock( void )
{
	//led4.setState(LEDON);
	timeMaster = 1;
	clockSocket.SwitchMidiClock(&intMidiClock);
}

void SlidersPanel::switchToExternalClock( void )
{
	//led4.setState(LEDOFF);
	timeMaster = 0;
	clockSocket.SwitchMidiClock(&extMidiClock);
}

void SlidersPanel::printDebug( void )
{
	
	char buffer[200] = {0};
	sprintf(buffer, "Slider_Panel debug: %05d, %05d, %05d, %05d, %05d, %05d\n", slider1.getState(), slider2.getState(), slider3.getState(), slider4.getState(), slider5.getState(), slider6.getState());
	Serial6.print(buffer);
	sprintf(buffer, "                up:   %d      %d      %d      %d      %d\n", sw1Up.getState(), sw2Up.getState(), sw3Up.getState(), sw4Up.getState(), sw5Up.getState());
	Serial6.print(buffer);
	sprintf(buffer, "              down:   %d      %d      %d,     %d      %d\n", sw1Down.getState(), sw2Down.getState(), sw3Down.getState(), sw4Down.getState(), sw5Down.getState());
	Serial6.print(buffer);

}
