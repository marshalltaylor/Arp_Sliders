#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "main.h"
#include "debugPins.h"

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
