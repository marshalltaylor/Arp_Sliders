// This file holds misc stuff that an Arduino sketch might require.
#include <stdio.h>
//#include "main.h"
#include "usart.h"
#include "HardwareSerial.h"
#include "interface.h"

/* Private variables ---------------------------------------------------------*/
extern UartInstance_t VCP_UART;
extern UartInstance_t D01_UART;

HardwareSerial Serial2;
HardwareSerial Serial6;

/* Private function prototypes -----------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void interface_init(void)
{
	Serial2.init(&D01_UART);
	Serial6.init(&VCP_UART);
}

void delay(uint32_t delayInput)
{
	uint32_t targetTicks = millis() + delayInput; //OK for this to roll
	while(millis() > targetTicks)
	{
		//if we rolled, wait until millis rolls
	}
	while(millis() < targetTicks)
	{
		//nop
	}
}