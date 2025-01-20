/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

/* Includes -- HAL -----------------------------------------------------------*/
#include "spi.h"

/* Includes -- BSP -----------------------------------------------------------*/
#include "bspPins.h"
#include "bsp.h"
#include "bspSpi.h"

/* References ----------------------------------------------------------------*/
#define localPrintf bspPrintf

spiControl_t spiCtrl;

/* Functions -----------------------------------------------------------------*/


bool bspSPIWrite(spiDevice_t * dev)
{
	//Dump if busy
	if(spiCtrl.userDevice != NULL)
	{
		return false;
	}
	spiCtrl.userDevice = dev;
	//Not busy, process write
	HAL_StatusTypeDef retVal;

	bspIOPinWrite(spiCtrl.userDevice->bspPin, 0);
	retVal = HAL_SPI_TransmitReceive_DMA(&hspi1, spiCtrl.userDevice->txData, spiCtrl.userDevice->rxData, spiCtrl.userDevice->len);

	if(retVal != HAL_OK)
	{
        localPrintf("ERROR SPI ! HAL_OK\n");
		spiCtrl.userDevice = NULL;
		return false;
	}
	else
	{
        //localPrintf("SPI HAL_OK, write %d\n", spiCtrl.userDevice->len);
		return true;
	}
}

void bspSPIWriteWait(spiDevice_t * dev)
{
	bspSPIWrite(dev);
	while(!bspSPIRead(dev));
}

bool bspSPIRead(spiDevice_t * dev)
{
	if(spiCtrl.userDevice != dev)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(spiCtrl.userDevice == NULL)
	{
		localPrintf("ERROR SPI callback\n");
	}
	else
	{
		bspIOPinWrite(spiCtrl.userDevice->bspPin, 1);
	}
	spiCtrl.userDevice = NULL;
}
