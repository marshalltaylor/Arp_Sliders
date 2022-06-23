#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "main_cubemx.h"
#include "debugUtilities.h"

void traceWrite(SCOPE_COLORS_T trace, bool state)
{
	//Don't debug
	return;
	
	//Debug
	GPIO_PinState value = GPIO_PIN_RESET;
	uint32_t port;
	uint32_t pin;
	switch(trace)
	{
		case YELLOW:
		port = AHB1PERIPH_BASE + PORTD_OFFSET;
		pin = GPIO_PIN_2;
		break;
		case CYAN:
		port = AHB1PERIPH_BASE + PORTC_OFFSET;
		pin = GPIO_PIN_11;
		break;
		case VIOLET:
		port = AHB1PERIPH_BASE + PORTB_OFFSET;
		pin = GPIO_PIN_0;
		break;
		case NAVY:
		port = AHB1PERIPH_BASE + PORTB_OFFSET;
		pin = GPIO_PIN_6;
		break;
		default:
		return;
		break;
	}
	if( state ) value = GPIO_PIN_SET;
	HAL_GPIO_WritePin((GPIO_TypeDef *) port, pin, value);
}

logRecords_t debugLog = {0};

void debugLogRecord(uint32_t line, uint32_t value, const char * tenChars)
{
	debugLog.last_written++;
	if(debugLog.last_written >= LOG_LENGTH)
	{
		debugLog.last_written = 0; 
	}
	debugLog.record[debugLog.last_written].lineNum = line;
	debugLog.record[debugLog.last_written].value = value;
	bool textEnded = false;
	for(int i = 0; i < 10; i++)
	{
		// Check for end of text
		// Subtly, don't access the index array if text has already ended...
		if((textEnded)||(tenChars[i] == '\0'))
		{
			textEnded = true;
		}
		// Also, make sure to fill unused data so it's not seen by the debugger
		if(textEnded)
		{
			debugLog.record[debugLog.last_written].text[i] = '\0';;
		}
		else
		{
			debugLog.record[debugLog.last_written].text[i] = tenChars[i];
		}
	}
}

void debugLogClear(void)
{
	debugLog.last_written = 0; 
	for(int i = 0; i < LOG_LENGTH; i++)
	{
		debugLog.record[i].lineNum = 0;
		debugLog.record[i].value = 0;
		for(int j = 0; j < 10; j++)
		{
			debugLog.record[i].text[j] = '\0';;
		}
	}
}

void printLog()
{
	int recordIndex = debugLog.last_written;
	recordIndex++;
	if(recordIndex >= LOG_LENGTH)
	{
		recordIndex = 0; 
	}
	return;
}
