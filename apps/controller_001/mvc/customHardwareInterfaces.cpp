#include "customHardwareInterfaces.h"
#include "customPanelComponents.h"
#include "bsp.h"

using namespace std;

SpiDigitalIn::SpiDigitalIn( uint8_t inputBit, int inputByte )
{
	localData.size = 1;
	localData.data = new uint8_t[localData.size];
	bitIndex = inputBit;
	byteIndex = inputByte;

}

void SpiDigitalIn::readHardware( void )
{
	*localData.data = (SPIButtonData[byteIndex] >> bitIndex) & 0x01;
}

SpiDigitalOut::SpiDigitalOut( uint8_t inputBit, int inputByte )
{
	localData.size = 1;
	localData.data = new uint8_t[localData.size];
	bitIndex = inputBit;
	byteIndex = inputByte;

}

void SpiDigitalOut::writeHardware( void )
{
    uint8_t b = spiTxBuf[byteIndex];
    //clear bit
    b &= ~(0x01 << bitIndex);
    //write in state
    b |= ((*localData.data & 0x01) << bitIndex);
    spiTxBuf[byteIndex] = b;
}

bspEncoderIn::bspEncoderIn( int index )
{
	localData.size = 2;
	localData.data = new uint8_t[localData.size];
	bspIndex = index;
}

void bspEncoderIn::readHardware( void )
{
	int32_t temp = bspIOEncRead(bspIndex);
	*(int16_t *)localData.data = temp;
};

//This is set to just clear
void bspEncoderIn::writeHardware( void )
{
	bspIOEncWrite(bspIndex, 0);
};
