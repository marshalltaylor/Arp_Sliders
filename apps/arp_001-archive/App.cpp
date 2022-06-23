#include "App.h"
#include "Arduino.h"
#include "interface.h"
#include "sketch.h"

int AppEntry(void)
{
	// Arduino (serial objects) init
	interface_init();
	
	setup();
	
	while(1)
	{
		loop();
	}
	
}
