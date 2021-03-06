#include "Arduino.h"
#include "KnobPanel.h"

KnobPanel myCustomPanel;

uint16_t debugCounter = 0;

void setup()
{
	Serial.begin(115200);
	delay(2000);
	Serial.println("OK");
	//Go to fresh state
	myCustomPanel.reset();
	
}

//Set LOOP_DELAY to length of time between ticks of everything (in ms)
#define LOOP_DELAY 10

void loop()
{
	//Tick the machine
	myCustomPanel.tickStateMachine(LOOP_DELAY);
	
	//Delay the loop
	delay(LOOP_DELAY);
	
	//Increment a counter for debug stuff
	debugCounter = debugCounter + LOOP_DELAY;
	if( debugCounter > 10000 )
	{
		//Do debug stuff
		Serial.print("Free ram: ");
		Serial.println(freeRam()); 
		debugCounter = 0;
	}
	
}


int freeRam () {
#ifdef __AVR__
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
#else
  return 0;
#endif
}
