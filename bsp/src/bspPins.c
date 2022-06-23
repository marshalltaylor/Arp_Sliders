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
	{PORTB_OFFSET, GPIO_PIN_6},
	{PORTD_OFFSET, GPIO_PIN_2},
	{PORTC_OFFSET, GPIO_PIN_11},
};

#ifdef __cplusplus
}
#endif
