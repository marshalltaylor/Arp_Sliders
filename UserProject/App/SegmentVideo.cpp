#include "display_clock.h"
#include "SegmentVideo.h"
#include "Arduino.h"

static uint8_t segmentMap[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x00, 0x00};

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


void SegmentFrameBuffer::write(const uint8_t * data)
{
	if(nextToWrite == outputPtr)
	{
		// full
		return;
	}
	for(int i = 0; i < 11; i++)
	{
		onBuffer[nextToWrite][i] = data[i];//*(data + i);
		offBuffer[nextToWrite][i] = data[i];
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



void SegmentVideo::displayDrawClockNums( const char * input )
{
	for( int i = 0; i < 4; i++ )
	{
		serialBuffer[i + 7] = 0x00;
		if( *(input + i) == '-' )
		{
			serialBuffer[i + 7] = 0x40;
		}
		else if((*(input + i) >= 0x30)&&(*(input + i) < 0x3A))
		{
			serialBuffer[i + 7] = segmentMap[*(input + i) - 0x30];
		}
	}
	clockStream.write(serialBuffer);
}

void SegmentVideo::displayDrawValue( const char * input )
{
	for( int i = 0; i < 3; i++ )
	{
		serialBuffer[i + 2] = 0x00;
		if((*(input + i) >= 0x30)&&(*(input + i) < 0x3A))
		{
			serialBuffer[i + 2] = segmentMap[*(input + i) - 0x30];
		}
	}
	clockStream.write(serialBuffer);
}

void SegmentVideo::setPlayIndicator( void )
{
	serialBuffer[0] |= 0x80;
	clockStream.write(serialBuffer);
}

void SegmentVideo::clearPlayIndicator( void )
{
	serialBuffer[0] &= ~0x80;
	clockStream.write(serialBuffer);
}

void SegmentVideo::toggleClockColon(void)
{
	serialBuffer[0] ^= 0x08;
	clockStream.write(serialBuffer);
}

void SegmentVideo::writeNextFrame(void)
{
	BufferChannels nextClockFrame;
	//uint8_t * nextMaskFrame;
	//maskStream.read(nextMaskFrame, dummy);
	//uint8_t * nextClockFrame;
	nextClockFrame = clockStream.read();
	for(int i = 0; i < 11; i++)
	{
		outputFrame[i] = nextClockFrame.onBufferPtr[i];
	}	
	writeDisplay(outputFrame);
}
