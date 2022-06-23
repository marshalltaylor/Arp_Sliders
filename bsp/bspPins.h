#ifndef BSP_PINS_H
#define BSP_PINS_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

/* Macros --------------------------------------------------------------------*/
#define PORTA_OFFSET 0x0000U
#define PORTB_OFFSET 0x0400U
#define PORTC_OFFSET 0x0800U
#define PORTD_OFFSET 0x0C00U
#define PORTE_OFFSET 0x1000U
#define PORTF_OFFSET 0x1400U
#define PORTG_OFFSET 0x1800U
#define PORTH_OFFSET 0x1C00U
#define PORTI_OFFSET 0x2000U
#define PORTJ_OFFSET 0x2400U
#define PORTK_OFFSET 0x2800U

//Analog names
#define MUX_A0       0
#define MUX_A1       1
#define MUX_A2       2
#define MUX_A3       3
#define MUX_B0       4
#define MUX_B1       5
#define MUX_B2       6
#define MUX_B3       7
#define MUX_C0       8
#define MUX_C1       9
#define MUX_C2       10
#define MUX_C3       11
#define MUX_D0       12
#define MUX_D1       13
#define MUX_D2       14
#define MUX_D3       15

/* Types ---------------------------------------------------------------------*/
typedef struct
{
	uint32_t port;
	uint32_t bit;
} PinData_t;

/* Variable References -------------------------------------------------------*/
extern const PinData_t pins[];

#ifdef __cplusplus
}
#endif

#endif /* _VARIANT_ARDUINO_STM32_ */
