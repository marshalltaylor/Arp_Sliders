#ifndef BSP_UART
#define BSP_UART

#include <stdbool.h>
#include "stm32f446xx.h"

#define TX_BUFFER_SIZE 1024
#define RX_BUFFER_SIZE 1024

#define UARTS_MONITORED 7

//alignment may not be necessary
typedef __attribute__ ((aligned(4))) struct UartStatistics
{
	uint8_t ctrCharRx;
	uint8_t ctrCharTx;
	uint8_t ctrTXH;
	uint8_t ctrUncaught;
} UartStatistics_t;

typedef __attribute__ ((aligned(4))) struct UartInstance
{
	USART_TypeDef *hw;
	
	UartStatistics_t stats;
	bool UartTxInProgress;

	volatile uint8_t txDataBuffer[TX_BUFFER_SIZE+10];
	volatile int16_t txDataBuffer_head;
	volatile int16_t txDataBuffer_next;
	volatile int16_t txDataBuffer_tail;

	volatile uint8_t rxCharBuffer;
	volatile uint8_t rxDataBuffer[RX_BUFFER_SIZE+10];
	volatile int16_t rxDataBuffer_first;
	volatile int16_t rxDataBuffer_last;
} UartInstance_t;

void bspUartConstruct(UartInstance_t * UART);
void bspUartCallback(UartInstance_t * UART);

// Write a byte to the passed UartInstance_t
void bspUartWrite(uint8_t data, UartInstance_t * UART);

// Look at "first" rxDataBuffer value
uint8_t bspUartPeek(UartInstance_t * UART);

// Look at "first" and advance ptr if not end
uint8_t bspUartRead(UartInstance_t * UART);

// Get number of bytes read.
// This should be used to check if data is available before using
// the other usart routines. 
uint16_t bspUartReadBytesAvailable(UartInstance_t * UART);

#endif
