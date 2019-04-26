#include <stdint.h>
#include <stdbool.h>
#include "spi.h"

#include "spiDisplay.h"

typedef struct spiDisplayObject
{
	bool txInProgress;
	uint8_t cmd;
} spiDisplayObject_t;

static spiDisplayObject_t spiDisplayData;

bool spiDisplayStartDMACommand(uint8_t command)
{
	if( spiDisplayData.txInProgress )
	{
		return false;
	}
	spiDisplayData.txInProgress = true;
	spiDisplayData.cmd = command;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 0);
	HAL_SPI_Transmit_IT(&hspi1, &spiDisplayData.cmd, 1);

	return true;
}

bool spiDisplayStartDMAData(uint8_t * data, uint16_t length)
{
	if( spiDisplayData.txInProgress )
	{
		return false;
	}
	spiDisplayData.txInProgress = true;
	HAL_SPI_Transmit_IT(&hspi1, data, length);

	return true;
}

void spiDisplayWriteData(uint8_t data)
{
	while( spiDisplayData.txInProgress ){};
	spiDisplayData.txInProgress = true;
	HAL_SPI_Transmit_IT(&hspi1, &data, 1);
	//while( spiDisplayData.txInProgress ){};
}

void spiDisplayWriteCommand(uint8_t data)
{
	while( spiDisplayData.txInProgress ){};
	spiDisplayData.txInProgress = true;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 0);
	HAL_SPI_Transmit_IT(&hspi1, &data, 1);
	
}

void spiDisplayDisable(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
}

void spiDisplayEnable(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if( hspi != &hspi1 )
	{
		return;
	}
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 1);
	spiDisplayData.txInProgress = false;
}

bool spiDisplayBusy(void)
{
	return spiDisplayData.txInProgress;
}
