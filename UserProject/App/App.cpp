#include "App.h"
#include "Arduino.h"
#include "interface.h"
#include "sketch.h"
int AppEntry(void)
{
	setup();
	
	while(1)
	{
		loop();
	}
	
}
