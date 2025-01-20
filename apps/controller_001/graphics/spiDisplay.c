#include <stdint.h>
#include <stdbool.h>
#include "bsp.h"

#include "spiDisplay.h"

uint8_t spiTxBuf[4];
uint8_t spiRxBuf[4];

spiDevice_t screen = {
    .bspPin = SPI_CS,
    .txData = spiTxBuf,
    .rxData = spiRxBuf,
    .len = 0,
    .pendingXfer = false,
};

void spiDisplayWriteData(uint8_t data)
{
    // Init data
    screen.txData[0] = data;
    screen.len = 1;
    //Choose function
    bspIOPinWrite(TV_DATA_NOT_COMMAND, 1);
    //Write
    bspSPIWriteWait(&screen);
}

void spiDisplayWriteCommand(uint8_t data)
{
    // Init data
    screen.txData[0] = data;
    screen.len = 1;
    //Choose function
    bspIOPinWrite(TV_DATA_NOT_COMMAND, 0);
    //Write
    bspSPIWriteWait(&screen);
}

void spiDisplayDisable(void)
{
    bspIOPinWrite(TV_NOT_RESET, 0);
}

void spiDisplayEnable(void)
{
    bspIOPinWrite(TV_NOT_RESET, 1);
}
