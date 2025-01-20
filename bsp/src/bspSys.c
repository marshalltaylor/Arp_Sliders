/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

/* Includes -- HAL -----------------------------------------------------------*/
#include "main_cubemx.h"
#include "tim.h"
#include "stm32f4xx_it.h"
#include "stm32f446xx.h"

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

#define USER_FLASH_ADDRESS 0x08060000

void bspFlashRead(uint8_t * pData, uint32_t len)
{
    memcpy(pData, (uint8_t*)USER_FLASH_ADDRESS, len);
}

void bspFlashWrite(uint8_t * pData, uint32_t len)
{
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR );
    FLASH_Erase_Sector(FLASH_SECTOR_7, VOLTAGE_RANGE_3);
    for (int i = 0; i < len; i++)
    {
       HAL_FLASH_Program(TYPEPROGRAM_BYTE, USER_FLASH_ADDRESS + i, pData[i]);
    }
    HAL_FLASH_Lock();
}
