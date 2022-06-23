#include "bsp.h"
#include "debugUtilities.h"
#include "tim.h"

uint32_t usTicks = 0;
uint32_t msTicks = 0;
int32_t msSubCounter = 0; //at uS, rolls
uint32_t fastRunTimeTicks = 0;

//  The lock works like this:
//
//    When the interrupt fires, the lock is removed.  Now
//    the main free-wheeling loop can update the change to
//    the timerModules.  Once complete, the lock is replaced
//    so that it can't update more than once per firing
//    of the interrupt

uint8_t usTicksLocked = 1; //start locked out
uint32_t maxTimer = 60000000;
uint32_t maxInterval = 2000000;

void timer3setPeriod(uint32_t uSPeriod);

bspTimerCallback_t timer3TickCallback;
bspTimerCallback_t sysTickCallbackPointer;

uint32_t adcUsCounter = 0;
uint32_t timer3UsInterval = 100;
uint32_t timer3UsCounter = 0;

void timer4TickCallback(void)
{
	return;
}

void timer3setPeriod(uint32_t uSPeriod)
{
    timer3UsInterval = uSPeriod;
}

void bspRegisterSysTickCallback(bspTimerCallback_t cbFn)
{
	//This needs to save the os tick callback
	sysTickCallbackPointer = cbFn;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//traceWrite(VIOLET, 1);

	if(htim->Instance == TIM2)
	{
		//fast arbitrary run time ticker
		fastRunTimeTicks++;

		//50us resolution timer
		if(usTicks >= ( maxTimer + maxInterval ))
		{
			usTicks = usTicks - maxTimer;
		}
		else
		{
			usTicks = usTicks + 50;
		}
		usTicksLocked = 0;  //unlock
        
        msSubCounter += 50;
        if(msSubCounter >= 1000)
        {
            while(msSubCounter >= 1000)
            {
                msSubCounter -= 1000;
            }
            msTicks++;
        }
	}
	else if(htim->Instance == TIM3)
	{
		adcUsCounter += 100;
		if(adcUsCounter > 10000)
		{
			//Schedule an ADC conversion
			bspADCConvert();
			adcUsCounter = 0;
		}

		timer3UsCounter += 100;
		if(timer3UsCounter > timer3UsInterval)
		{
			// Call into app
            if( timer3TickCallback != NULL )
            {
                timer3TickCallback();
            }
            while(timer3UsCounter > timer3UsInterval) timer3UsCounter -= timer3UsInterval;
        }
	}
	else if(htim->Instance == TIM4)
	{
		// Call into app
		//if( timer4TickCallback != NULL ) // Function defined locally
		{
			timer4TickCallback();
		}
	}
	else if (htim->Instance == TIM7)
	{
		HAL_IncTick();
	}
	//traceWrite(VIOLET, 0);
}

/* Defined in main.c. */
void vConfigureTimerForRunTimeStats( void )
{
	//This is called once, usually for HW timer config (done at bsp init)

}
