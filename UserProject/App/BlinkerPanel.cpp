//********************************************//
#include "BlinkerPanel.h"
#include "panelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>
#include "midiTime.h"
#include "StatusPanel.h"
#include "SegmentVideo.h"

extern MidiClock extMidiClock;
extern MidiClock intMidiClock;
extern MidiClockSocket clockSocket;
extern StatusPanel statusPanel;
extern SegmentVideo Segments;

const uint8_t FadePixelMap[16][11] =
{
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x23, 0x23, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x23, 0x23, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x63, 0x63, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x63, 0x63, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x67, 0x67, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x67, 0x67, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

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
	knob1.setSamplesAveraged(5);

	knob3.setHardware(new ArduinoAnalogIn( A0 ));
	add( &knob3 );
	knob3.setLowerKnobVal(10);
	knob3.setUpperKnobVal(1014);
	knob3.setLowerIntVal(0);
	knob3.setUpperIntVal(255);
	knob3.setSamplesAveraged(5);

	knobTempo.setHardware(new ArduinoAnalogIn( A2 ));
	add( &knobTempo );
	knobTempo.setLowerKnobVal(10);
	knobTempo.setUpperKnobVal(1014);
	knobTempo.setLowerIntVal(40);
	knobTempo.setUpperIntVal(208);
	knobTempo.setSamplesAveraged(5);

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
	displayState = init;
	
	timeMaster = 0;
	switchToExternalClock();
	
	currentBPM = knobTempo.getAsInt16() * 1000;
	targetBPM = currentBPM;
	glideRate = knob1.getAsInt16() * 1000;
	intMidiClock.setBPM(currentBPM / 1000);
	
}

void BlinkerPanel::tickStateMachine( int msTicksDelta )
{
	freshenComponents( msTicksDelta );
	displaySMTK.uIncrement( msTicksDelta * 1000 );
	//***** PROCESS THE LOGIC *****//
	//Now do the states.
	PStates nextState = state;
	switch( state )
	{
	case PInit:
		//Can't be running, if button pressed move on
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
		emergencyRestart();
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
		if(!timeMaster)
		{
			switchToInternalClock();
		}
		intMidiClock.play();
	}
	if( stop.serviceRisingEdge() )
	{
		Serial6.println("stop");
		intMidiClock.stop();
	}
	

	if( knob1.serviceChanged() )
	{
		//Serial6.print("knob1: ");
		//Serial6.println(knob1.getState());
		//Serial6.println(analogRead(A1));
		sprintf( knob1Str, "%3d", knob1.getAsInt16() );
		knobStrShown = SELECT_KNOB_1;
		glideRate = knob1.getAsInt16() * 1000;
		startShowingKnob = 1;
		displaySMTK.uClear();
	}
	if( knob3.serviceChanged() )
	{
		Serial6.print("knob3: ");
		Serial6.println(knob3.getState());
		Serial6.println(analogRead(A2));
		sprintf( knob3Str, "%3d", knob3.getAsInt16() );
		knobStrShown = SELECT_KNOB_3;
		startShowingKnob = 1;
		displaySMTK.uClear();
	}
	if( knobTempo.serviceChanged() )
	{
		Serial6.print("knobTempo: ");
		Serial6.println(knobTempo.getState());
		Serial6.println(analogRead(A2));
		targetBPM = knobTempo.getAsInt16() * 1000;
		//intMidiClock.setBPM(knobTempo.getAsInt16());
		displaySMTK.uClear();
		sprintf( knobTempoStr, "%3d", knobTempo.getAsInt16() );
		knobStrShown = SELECT_TEMPO;
		startShowingKnob = 1;
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

	switch( displayState )
	{
		case init:
		{
			displayState = idle;
		}
		break;
		case idle:
		{
			if(startShowingKnob)
			{
				displayState = fadeout;
				startShowingKnob = 0;
			}
		}
		break;
		case fadeout:
		{
			if( Segments.maskStream.ready() )
			{
				for( int i = 15; i >= 0; i--)
				{
					Segments.maskStream.write(&FadePixelMap[i][0]);
				}
				displayState = waitForFadeout;
			}
		}
		break;
		case waitForFadeout:
		{
			if(Segments.maskStream.empty())
			{
				displayState = showSelection;
				displaySMTK.uClear();				
			}
		}
		break;
		case fadeInSelection:
		{
			if( Segments.maskStream.ready() )
			{
				for( int i = 0; i < 16; i++)
				{
					Segments.maskStream.write(&FadePixelMap[i][0]);
				}
				displayState = fadeInSelection;
			}
		}
		break;
		case showSelection:
		{
			switch( knobStrShown )
			{
				case SELECT_KNOB_1:
				Segments.displayDrawValue(knob1Str);
				break;
				case SELECT_KNOB_3:
				Segments.displayDrawValue(knob3Str);
				break;
				case SELECT_TEMPO:
				Segments.displayDrawValue(knobTempoStr);
				break;
				default:
				break;
			}
			if( displaySMTK.uGet() > 1000000 )
			{
				displayState = fadeOutSelection;
			}
		}
		break;
		case fadeOutSelection:
		{
			if( Segments.maskStream.ready() )
			{
				for( int i = 0; i < 16; i++)
				{
					Segments.maskStream.write(&FadePixelMap[i][0]);
				}
				displayState = idle;
				displaySMTK.uClear();
				startShowingKnob = 0;
			}
		}
		break;
		default:
		displayState = init;
		break;
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
