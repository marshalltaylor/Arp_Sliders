/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/* Includes -- HAL -----------------------------------------------------------*/
#include "main_cubemx.h"
#include "tim.h"
#include "stm32f4xx_it.h"

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"
#include "bspSerial.h"

/* References ----------------------------------------------------------------*/
extern uint32_t msTicks; //from bspTime
extern uint32_t fastRunTimeTicks; //from tim.c

/* Functions -----------------------------------------------------------------*/
void bspInit(void)
{
	// Without this garbage sysTick_Handler won't be linked?
	volatile void * trash = SysTick_Handler;
	trash = EXTI15_10_IRQHandler;
	UNUSED(trash);

	main_cubemx();
	
	bspEncInit();
	
	bspSerialStart();
}

uint32_t millis(void)
{
	return msTicks;
};

uint32_t ulGetFastRunTimeTicks(void)
{
	return fastRunTimeTicks;
};

void bspDelay(uint32_t delayInput)
{
    uint32_t now = msTicks;
    uint32_t targetTicks = now + delayInput;
    if(targetTicks > now)
    {
        //No rolled
        while(msTicks < targetTicks);
    }
    else
    {
        //Rolled -- XOR two regions
        while(!(msTicks > now) != !(msTicks < targetTicks));
    }
}
