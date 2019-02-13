#include "display_clock.h"
#include "SegmentVideo.h"
#include "Arduino.h"

SegmentFrameBuffer::SegmentFrameBuffer(void){
	for(int i = 0; i < 11; i++)
	{
		onBuffer[0][i] = 0xF + i;
		offBuffer[0][i] = 0xF + i;
	}
}

uint8_t testData[11] = {1,2,0,0,0xFF,0xF0,0xFF,8,9,10,11};
BufferChannels SegmentFrameBuffer::read( void )
{
	BufferChannels retVar;
	retVar.onBufferPtr = &(onBuffer[outputPtr][0]);
	retVar.offBufferPtr = &(offBuffer[outputPtr][0]);

	//retVar.onBufferPtr = testData;
	//retVar.offBufferPtr = testData;
	if( ((outputPtr == FB_FIFO_SIZE - 1)&&( nextToWrite == 0 )) || (outputPtr + 1 == nextToWrite) )
	{
		// no new data, special case
		return retVar;
	}
	outputPtr++;
	if( outputPtr >= FB_FIFO_SIZE )
	{
		outputPtr = 0;
	}	
	return retVar;
}


void SegmentFrameBuffer::write(const uint8_t * onData, const uint8_t * offData)
{
	if(nextToWrite == outputPtr)
	{
		// full
		return;
	}
	for(int i = 0; i < 11; i++)
	{
		onBuffer[nextToWrite][i] = onData[i];
		offBuffer[nextToWrite][i] = offData[i];
		//onBuffer[nextToWrite][i] = testData[i];
		//offBuffer[nextToWrite][i] = testData[i];
		//onBuffer[nextToWrite][i] = 0x03;
		//offBuffer[nextToWrite][i] = 0x03;
	}
	nextToWrite++;
	if( nextToWrite >= FB_FIFO_SIZE )
	{
		nextToWrite = 0;
	}
	//Serial6.print("outputPtr = ");
	//Serial6.println(outputPtr);
	//Serial6.print("nextToWrite = ");
	//Serial6.println(nextToWrite);
}

bool SegmentFrameBuffer::ready( void )
{
	int16_t length;
	if( outputPtr > nextToWrite )
	{
		length = (nextToWrite + FB_FIFO_SIZE) - outputPtr - 1;
	}
	else
	{
		length = nextToWrite - outputPtr - 1;
	}
	return ( FB_FIFO_SIZE - length > FB_WATERMARK );
}

bool SegmentFrameBuffer::empty( void )
{
	int16_t length;
	if( outputPtr > nextToWrite )
	{
		length = (nextToWrite + FB_FIFO_SIZE) - outputPtr - 1;
	}
	else
	{
		length = nextToWrite - outputPtr - 1;
	}
	if (length == 0)
	{
		return true;
	}
	return false;
}



void SegmentVideo::writeNextFrame(void)
{
	BufferChannels nextValueMaskFrame;
	nextValueMaskFrame = valueMask_layer.read();
	BufferChannels nextFGFrame;
	nextFGFrame = fg_layer.read();
	BufferChannels nextNoiseFrame;
	nextNoiseFrame = noise_layer.read();
	
	for(int i = 0; i < 11; i++)
	{
		outputFrame[i] = textBitmap[i];
		outputFrame[i] &= ~nextValueMaskFrame.offBufferPtr[i];
		outputFrame[i] |= nextValueMaskFrame.onBufferPtr[i];
		//outputFrame[i] = nextFGFrame.onBufferPtr[i];
		outputFrame[i] &= ~nextNoiseFrame.offBufferPtr[i];
		outputFrame[i] |= nextNoiseFrame.onBufferPtr[i];
	}	
	writeDisplay(outputFrame);
}
