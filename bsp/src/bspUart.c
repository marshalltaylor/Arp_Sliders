#include "bspUart.h"
#include "stm32f446xx.h"
#include "stm32f4xx_ll_usart.h"

extern UartInstance_t VCP_UART;
extern UartInstance_t D01_UART;
extern UartInstance_t D28_UART;

void bspUartConstruct(UartInstance_t * UART)
{
	if(UART)
	{
		UART->stats.ctrCharRx = 0;
		UART->stats.ctrCharTx = 0;
		UART->stats.ctrTXH = 0;
		UART->stats.ctrUncaught = 0;
		
		UART->txDataBuffer_head = 0;
		UART->txDataBuffer_next = 0;
		UART->txDataBuffer_tail = 0;
		
		UART->rxDataBuffer_first = 0;
		UART->rxDataBuffer_last = 0;
		UART->rxDataBuffer[0] = 0;
		
		UART->UartTxInProgress = false;
	}
	if(UART == &VCP_UART)
	{
		UART->hw = USART6;
	}
	else if(UART == &D01_UART)
	{
		UART->hw = USART2;
	}
	else if(UART == &D28_UART)
	{
		UART->hw = USART1;
	}
	else
	{
		//TODO: If the logic is correct this should never hit, so leave it here until after testing
		while(1); // if never hit remove 
	}

	LL_USART_EnableIT_RXNE(UART->hw);
	//LL_USART_EnableIT_TC(UART->hw);

}

void bspUartCallback(UartInstance_t * UART)
{
	if(UART)
	{
		if(LL_USART_IsActiveFlag_RXNE(UART->hw))
		{
			UART->stats.ctrCharRx++;
			
			UART->rxDataBuffer[UART->rxDataBuffer_last] = LL_USART_ReceiveData8(UART->hw);
			UART->rxDataBuffer_last++;
			if( UART->rxDataBuffer_last >= RX_BUFFER_SIZE )
			{
				UART->rxDataBuffer_last = 0;
			}
		}
		else if(LL_USART_IsActiveFlag_TXE(UART->hw))
		{
			UART->stats.ctrTXH++;
			//TX buffers are hungry for data
			
			if((UART->txDataBuffer_tail)!=(UART->txDataBuffer_head))
			{
				//there is data for consumption
				char c = *(uint8_t *)(UART->txDataBuffer + UART->txDataBuffer_head);
				LL_USART_TransmitData8(UART->hw, c);
				UART->txDataBuffer_head++;
				if(UART->txDataBuffer_head >= TX_BUFFER_SIZE)
				{
					UART->txDataBuffer_head = 0;
				}
			}
			else
			{
				//disable TXE interrupt
				LL_USART_DisableIT_TXE(UART->hw);
			}

		}
		else if(LL_USART_IsActiveFlag_TC(UART->hw))
		{
			LL_USART_ClearFlag_TC(UART->hw);
			UART->stats.ctrCharTx++;
			/* Set transmission flag: trasfer complete*/
			UART->UartTxInProgress = false;
			UART->txDataBuffer_head = UART->txDataBuffer_next;
			if(UART->txDataBuffer_next != UART->txDataBuffer_tail)
			{
				LL_USART_EnableIT_TXE(UART->hw);
			}
		}
		else
		{
			UART->stats.ctrUncaught++;
			//error?
		}
	}
	else
	{
		//TODO: If the logic is correct this should never hit, so leave it here until after testing
		while(1); // if never hit remove 
	}
}

uint8_t bspUartPeek(UartInstance_t * UART)
{
	return UART->rxDataBuffer[UART->rxDataBuffer_first];
}

void bspUartWrite(uint8_t data, UartInstance_t * UART)
{
	//Calculate the next offset if a byte were to be
	//written
	int next_tail = UART->txDataBuffer_tail;
	next_tail++;
	if( next_tail >= TX_BUFFER_SIZE )
	{
		next_tail = 0;
	}
	//If this is on the head, a loop is being attempted
	while( next_tail == UART->txDataBuffer_head )
	{
		//wait
	}
	UART->txDataBuffer[UART->txDataBuffer_tail] = data;
	UART->txDataBuffer_tail = next_tail;

	LL_USART_EnableIT_TXE(UART->hw);
}

uint8_t bspUartRead(UartInstance_t * UART)
{
	uint8_t c = bspUartPeek(UART);
	if(UART->rxDataBuffer_first != UART->rxDataBuffer_last)
	{
		UART->rxDataBuffer_first++;
		if( UART->rxDataBuffer_first >= RX_BUFFER_SIZE )
		{
			UART->rxDataBuffer_first = 0;
		}
	}
	return c;
}

uint16_t bspUartReadBytesAvailable(UartInstance_t * UART)
{
	uint16_t var = UART->rxDataBuffer_last - UART->rxDataBuffer_first;
	if( var < 0 )
	{
		var += RX_BUFFER_SIZE;
	}
	return var;
}
