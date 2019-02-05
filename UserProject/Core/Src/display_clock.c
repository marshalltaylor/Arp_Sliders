#include <stdint.h>
#include <stdbool.h>
#include "spi.h"

#include "display_clock.h"

static uint8_t serialBuffer[11];

void writeDisplay(uint8_t * inputBuffer)
{
	for( int i = 0; i < 11; i++ )
	{
		serialBuffer[i] = ~*(inputBuffer + i);
	}
	displayTransmitFrame();
}

void displayTransmitFrame(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_SPI_Transmit_IT(&hspi1, serialBuffer, 11);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}
