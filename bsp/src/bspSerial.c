/* Serial operates in two ways

1.  We have printf available as bspPrintf.  This defaults to com0 but can be
remapped.. somehow or wherex

2.  A serial object consisting of basic IO can be filled out pointed to a specific
port.  This allows users to use the IO without having to specify a port each time, just
at init.

*/

/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

/* Includes -- HAL -----------------------------------------------------------*/
//#include "usart.h"

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"
#include "bspUart.h"

/* References ----------------------------------------------------------------*/
UartInstance_t VCP_UART;
UartInstance_t D01_UART;
UartInstance_t D28_UART;
//
//UartInstance_t * comPorts[] =
//{
//	&VCP_UART,
//	&D01_UART,
//	NULL
//};

//UartInstance_t * comPorts[] =
//{
//	NULL,
//	NULL,
//	NULL
//};

char buffer[MAX_PRINTF_LEN];

// References to these functions given out through accessors:
static void bspVPrintf(const char* fmt, va_list args );

static uint8_t bspSerial_VCP_Peek();
static void bspSerial_VCP_Write(uint8_t data);
static uint8_t bspSerial_VCP_Read();
static uint16_t bspSerial_VCP_BytesAvailable();

static uint8_t bspSerial_D01_Peek();
static void bspSerial_D01_Write(uint8_t data);
static uint8_t bspSerial_D01_Read();
static uint16_t bspSerial_D01_BytesAvailable();

static uint8_t bspSerial_D28_Peek();
static void bspSerial_D28_Write(uint8_t data);
static uint8_t bspSerial_D28_Read();
static uint16_t bspSerial_D28_BytesAvailable();

/* Functions -----------------------------------------------------------------*/
// bspPrintf and bspVPrintf are both hardcoded to talk on a certain port.
void bspPrintf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	int i;
	for(i = 0; (i < MAX_PRINTF_LEN)&&buffer[i]!='\0'; i++)
	{
		bspSerial_VCP_Write(buffer[i]);
	}
	va_end(args);
}

void bspVPrintf(const char* fmt, va_list args )
{
	vsprintf(buffer, fmt, args);
	int i;
	for(i = 0; (i < MAX_PRINTF_LEN)&&buffer[i]!='\0'; i++)
	{
		bspSerial_VCP_Write(buffer[i]);
	}
}

// Accessors to get function names for dynamic usage
bspPrintf_t bspGetSerialConsolePrintf(void)
{
	return bspPrintf;
}

bspVPrintf_t bspGetSerialConsoleVPrintf(void)
{
	return bspVPrintf;
}

void bspSerialStart(void)
{
	bspUartConstruct(&VCP_UART);
	bspUartConstruct(&D01_UART);
	bspUartConstruct(&D28_UART);
}


// ----- Individual port functions ----- //
// I know, this is not flexible but it's easy for now.  I don't want to export
// the stm port objects past bsp.h, so this is the way it goes.

// VCP port
uint8_t bspSerial_VCP_Peek(void)
{
	return bspUartPeek(&VCP_UART);
}

void bspSerial_VCP_Write(uint8_t data)
{
	bspUartWrite(data, &VCP_UART);
}

uint8_t bspSerial_VCP_Read(void)
{
	return bspUartRead(&VCP_UART);
}

uint16_t bspSerial_VCP_BytesAvailable(void)
{
	return bspUartReadBytesAvailable(&VCP_UART);
}

// D01 port
uint8_t bspSerial_D01_Peek(void)
{
	return bspUartPeek(&D01_UART);
}

void bspSerial_D01_Write(uint8_t data)
{
	bspUartWrite(data, &D01_UART);
}

uint8_t bspSerial_D01_Read(void)
{
	return bspUartRead(&D01_UART);
}

uint16_t bspSerial_D01_BytesAvailable(void)
{
	return bspUartReadBytesAvailable(&D01_UART);
}

// D28 port
uint8_t bspSerial_D28_Peek(void)
{
	return bspUartPeek(&D28_UART);
}

void bspSerial_D28_Write(uint8_t data)
{
	bspUartWrite(data, &D28_UART);
}

uint8_t bspSerial_D28_Read(void)
{
	return bspUartRead(&D28_UART);
}

uint16_t bspSerial_D28_BytesAvailable(void)
{
	return bspUartReadBytesAvailable(&D28_UART);
}

// Finally, accessors to get the serial functions grouped to an object.
void bspGetSerialFunctions(comPort_t port, comPortInterface_t * interface)
{
	bspPrintf("com obj: 0x%X\n", port);
	switch(port) // this isn't really even used, what's going on with this crap.
	{
		case COM0:
		{
			interface->peek = bspSerial_VCP_Peek;
			interface->write = bspSerial_VCP_Write;
			interface->read = bspSerial_VCP_Read;
			interface->bytesAvailable = bspSerial_VCP_BytesAvailable;
		}
		break;
		case COM1:
		{
			interface->peek = bspSerial_D01_Peek;
			interface->write = bspSerial_D01_Write;
			interface->read = bspSerial_D01_Read;
			interface->bytesAvailable = bspSerial_D01_BytesAvailable;
		}
		break;
		case COM2:
		{
			interface->peek = bspSerial_D28_Peek;
			interface->write = bspSerial_D28_Write;
			interface->read = bspSerial_D28_Read;
			interface->bytesAvailable = bspSerial_D28_BytesAvailable;
		}
		break;
		default:
		break;
	}
}
