/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>

/* Includes -- HAL -----------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"
#include "bspPins.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Types ---------------------------------------------------------------------*/

/* Variable References -------------------------------------------------------*/
// Must match PinNumber_t enum order (bsp.h)
const PinData_t pins[NUMBER_OF_PINS] = {
	{PORTC_OFFSET, GPIO_PIN_8},
	{PORTC_OFFSET, GPIO_PIN_5},
	{PORTA_OFFSET, GPIO_PIN_12},
	{PORTA_OFFSET, GPIO_PIN_11},
	{PORTB_OFFSET, GPIO_PIN_12},
	{PORTB_OFFSET, GPIO_PIN_2},
	{PORTB_OFFSET, GPIO_PIN_1},
	{PORTB_OFFSET, GPIO_PIN_15},
	{PORTB_OFFSET, GPIO_PIN_14},
	{PORTB_OFFSET, GPIO_PIN_13},
	{PORTC_OFFSET, GPIO_PIN_13},
	{PORTB_OFFSET, GPIO_PIN_7},
};

#ifdef __cplusplus
}
#endif
