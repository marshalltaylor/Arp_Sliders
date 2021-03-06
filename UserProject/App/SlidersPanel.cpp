//********************************************//
#include "SlidersPanel.h"
#include "panelComponents.h"
#include "HardwareInterfaces.h"
#include "midi_Defs.h"

#include "globals.h"

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
	slider2.setUpperIntVal(10);
	slider2.setSamplesAveraged(10);

	slider3.setHardware(new ArduinoAnalogIn( A2 ));
	add( &slider3 );
	slider3.setLowerKnobVal(10);
	slider3.setUpperKnobVal(1014);
	slider3.setLowerIntVal(0);
	slider3.setUpperIntVal(5);
	slider3.setSamplesAveraged(10);

	slider4.setHardware(new ArduinoAnalogIn( A3 ));
	add( &slider4 );
	slider4.setLowerKnobVal(10);
	slider4.setUpperKnobVal(1014);
	slider4.setLowerIntVal(0);
	slider4.setUpperIntVal(30);
	slider4.setSamplesAveraged(10);

	slider5.setHardware(new ArduinoAnalogIn( A4 ));
	add( &slider5 );
	slider5.setLowerKnobVal(10);
	slider5.setUpperKnobVal(1014);
	slider5.setLowerIntVal(1);
	slider5.setUpperIntVal(40);
	slider5.setSamplesAveraged(10);

	slider6.setHardware(new ArduinoAnalogIn( A5 ));
	add( &slider6 );
	slider6.setLowerKnobVal(10);
	slider6.setUpperKnobVal(1014);
	slider6.setLowerIntVal(4);
	slider6.setUpperIntVal(32);
	slider6.setSamplesAveraged(10);

	reset();

}

void SlidersPanel::reset( void )
{
	//Set explicit states
	state = PInit;
	recState = MCInit;
	
	// timeMaster = true; //overridden by switchTo...
	//switchToInternalClock();
	
	currentBPM = 100;// knobTempo.getAsInt16() * 1000;
	targetBPM = currentBPM;
	glideRate = 50; //knob1.getAsInt16() * 1000;
	intMidiClock.setBPM(currentBPM / 1000);
	
	myArpeggiator.configDrone(true);
}

void SlidersPanel::tickStateMachine( int msTicksDelta )
{
	freshenComponents( msTicksDelta );
	
	// Level set
	//if( sw1Up.serviceRisingEdge() )
	//{
	//	
	//}
	if( sw1Down.serviceRisingEdge() )
	{
		myRecorder.clearAndInit();
	}
	
	switch(recState)
	{
		default:
		case MCClear:
		{
//			pattern.clearCtrlNotes();
//			pattern.printPattern();
			recState = MCWait;
			break;
		}
		case MCWait:
		{
			if( !sw1Down.getState() )
			{
				recState = MCIdle;
			}
			break;
		}
		case MCIdle:
		{
			if( sw1Down.serviceRisingEdge() )
			{
				recState = MCClear;
			}
			else if( sw1Up.getState() )
			{
				recState = MCRecording;
			}
			break;
		}
		case MCRecording:
		{
			if( !sw1Up.getState() )
			{
				recState = MCIdle;
			}
			break;
		}
	}
	if( sw2Up.serviceRisingEdge() )
	{
		char buffer[200] = {0};
		sprintf(buffer, "controlNoteMixer inputNoteOnList\n");
		Serial6.print(buffer);
		controlNoteMixer.inputNoteOnList.printfMicroLL();
		sprintf(buffer, "controlNoteMixer outputNoteBuffer\n");
		Serial6.print(buffer);
		controlNoteMixer.outputNoteBuffer.printfMicroLL();
		sprintf(buffer, "outputNoteMixer keyboardInputNoteOnList\n");
		Serial6.print(buffer);
		outputNoteMixer.keyboardInputNoteOnList.printfMicroLL();
		sprintf(buffer, "outputNoteMixer playerInputNoteOnList\n");
		Serial6.print(buffer);
		outputNoteMixer.playerInputNoteOnList.printfMicroLL();
		sprintf(buffer, "outputNoteMixer outputNoteBuffer\n");
		Serial6.print(buffer);
		outputNoteMixer.outputNoteBuffer.printfMicroLL();
	}
	
	if( sw3Down.serviceRisingEdge() )
	{

	}
	if( sw3Up.serviceRisingEdge() )
	{
		myArpeggiator.configDrone(true);
	}
	if( sw3Up.serviceFallingEdge() )
	{
		myArpeggiator.configDrone(false);
	}
	if( sw4Up.serviceRisingEdge() ||
		sw4Up.serviceFallingEdge() ||
		sw4Down.serviceRisingEdge() ||
		sw4Down.serviceFallingEdge() )
	{
//		if( sw4Up.getState() )
//		{
//			// Switch is up
//			pattern.setDirection(1);
//		}
//		else if( sw4Down.getState() )
//		{
//			// Switch is down
//			pattern.setDirection(-1);
//		}
//		else
//		{
//			// Switch is neutral
//			pattern.setDirection(0);
//		}
	}
	if( sw5Up.serviceRisingEdge() )
	{
		myRecorder.setLoopingControl(LOOPING_MANUAL);
	}
	if( sw5Up.serviceFallingEdge() )
	{
		myRecorder.setLoopingControl(LOOPING_DISABLED);
	}
	if( sw5Down.serviceRisingEdge() )
	{
		myRecorder.setLoopingControl(LOOPING_AUTO);
		oled.disableDirectAccess();
		delay(100); // let dma clear
		oled.end();
		delay(10);
		//for( volatile uint16_t i = 0; i < 0xFFFE; i++ )
		//{
		//	i++;
		//	i--;
		//};
		oled.begin();    // Initialize the OLED
		oled.flipVertical(true);
		oled.flipHorizontal(true);
		oled.clear(ALL); // Clear the display's internal memory
		oled.display();  // Display what's in the buffer (splashscreen)
		oled.enableDirectAccess();
	}
	if( sw5Down.serviceFallingEdge() )
	{
		//oled.disableDirectAccess();
		//delay(100); // let dma clear
		//oled.setFontType(0);  // Set font to type 0
		//oled.setCursor(0, 0);
		//oled.print("Hello!");		
		//oled.drawHeart(1 * 18, 12);
		//oled.drawStaff();
		//oled.display();  // Display what's in the buffer (splashscreen)
		
		//Debug Recorder
		myRecorder.printDebug();
	}
	
//	if( slider1.serviceChanged() )
//	{
//		pattern.setDepth(slider1.getState() / 16);
//	}
//	if( slider2.serviceChanged() )
//	{
//		pattern.setOverclock(slider2.getState() / 2);
//	}
	if( slider3.serviceChanged() )
	{
	}
	if( slider4.serviceChanged() )
	{
		myRecorder.setStepLength( 3 + slider4.getAsInt16() ); //lowest is 32nd note
	}
	if( slider5.serviceChanged() )
	{
		myRecorder.setLoopAtStep( slider5.getAsInt16() );
	}
	if( slider6.serviceChanged() )
	{
		myRecorder.setTapeLengthInBeats(slider6.getAsInt16());
	}

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

bool SlidersPanel::isRecording( void )
{
	return (recState == MCRecording);
}

void SlidersPanel::printDebug( void )
{
	
	char buffer[200] = {0};
	sprintf(buffer, "Slider_Panel debug: %05d, %05d, %05d, %05d, %05d, %05d\n", slider1.getState(), slider2.getState(), slider3.getState(), slider4.getState(), slider5.getState(), slider6.getState());
	Serial6.print(buffer);
	sprintf(buffer, "                up:   %d      %d      %d      %d      %d\n", sw1Up.getState(), sw2Up.getState(), sw3Up.getState(), sw4Up.getState(), sw5Up.getState());
	Serial6.print(buffer);
	sprintf(buffer, "              down:   %d      %d      %d      %d      %d\n", sw1Down.getState(), sw2Down.getState(), sw3Down.getState(), sw4Down.getState(), sw5Down.getState());
	Serial6.print(buffer);
	//outputPlayer.printDebug();
}
