#include "globals.h"


uint32_t hwTimerInterval = 100;
//callbacks
/***** Hardware Timer Callbacks ***********************************************/

void hwTimerCallback(void)
{
    if(hwTimerInterval > 1000)bspPrintf("pip\n");
}

//Usage:
// This function doesn't have a prototype because the header that would
// have it (globals.h) has c++ linkage which will cause problems if included
// in c code.
//
// Add 'extern void globalsInit(void);' to calling file
extern "C" void globalsInit(void)
{
	// Write our function address into the hw timer
    timer3setPeriod(hwTimerInterval);
	timer3TickCallback = hwTimerCallback;
	

}

float power(float x, int y)
{
    float answer = x;
    if(y == 0) return 1.0;
    if(y == 1) return x;
    for(int i = 1; i < y; i++)
    {
        answer *= x;
    }
    return answer;
}
