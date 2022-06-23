#ifndef BSP_H
#define BSP_H

/* Includes ------------------------------------------------------------------*/

// This file is an interface and should not include other files
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "bspSpi.h"
#include "bspPins.h"

/* Types ---------------------------------------------------------------------*/
// Must match PinData_t const array order (bspPins.c)
typedef enum
{
    SPI_CS,
    ADC_SEL_0,
    ADC_SEL_1,
    NUMBER_OF_PINS
} PinNumber_t;

// Analog pins are indexed directly into a bsp producing array of data.

/* Variable References -------------------------------------------------------*/
extern const char boardName[];

/* Function Definitions ------------------------------------------------------*/
void bspInit(void);

void timer3setPeriod(uint32_t uSPeriod);

void bspIOToggleLED(void);

void bspIOPinInit(uint8_t pin, uint8_t pullMode);
void bspIOPinWrite(uint8_t pin, bool value);
bool bspIOPinRead(uint8_t pin);
uint32_t bspIOPinReadAnalog(uint8_t pin);

void bspEncInit(void);
int32_t bspIOEncRead(uint8_t index);
void bspIOEncWrite(uint8_t index, int32_t value);

//Dirty api.  This may be run from within the bsp or by an app...
void bspADCConvert(void);

/* dac */
void bspDACInit( void );

void bspDACSendBuffer(uint32_t * buffer, uint16_t len);
void bspDACStartDMA(void);

bool bspDACGetBufferStale(uint8_t ** output);
bool bspDACGetBufferCopy(uint8_t ** output);
bool bspDACGetBufferBlank(uint8_t ** output, uint8_t fill);

void bspDACSwapBuffers(void);
void bspDACInterlace(bool input);

/*

Timer Functions
* Register timer

Mostly defined in tim.c

*/
uint32_t millis(void);
void bspDelay(uint32_t delayInput);
uint32_t ulGetFastRunTimeTicks(void);

typedef void (*bspTimerCallback_t)(void);

extern bspTimerCallback_t timer3TickCallback;
extern bspTimerCallback_t sysTickCallbackPointer;

void bspRegisterSysTickCallback(bspTimerCallback_t cbFn);

/* Serial */
#define MAX_PRINTF_LEN 256

typedef enum
{
	COM0 = 0,
	COM1,
	COM2,
	NUM_COM_PORTS
} comPort_t;

typedef void (*bspPrintf_t)(const char* fmt, ...);
typedef void (*bspVPrintf_t)(const char* fmt, va_list args );

typedef uint8_t (*bspSerialPeek_t)(void);
typedef void (*bspSerialWrite_t)(uint8_t data);
typedef uint8_t (*bspSerialRead_t)(void);
typedef uint16_t (*bspSerialBytesAvailable_t)(void);

typedef struct
{
	bspSerialPeek_t peek;
	bspSerialWrite_t write;
	bspSerialRead_t read;
	bspSerialBytesAvailable_t bytesAvailable;
} comPortInterface_t;

void bspPrintf(const char* fmt, ...);

bspPrintf_t bspGetSerialConsolePrintf(void);
bspVPrintf_t bspGetSerialConsoleVPrintf(void);
void bspGetSerialFunctions(comPort_t port, comPortInterface_t * interface);

#ifdef __cplusplus
}
#endif

#endif
