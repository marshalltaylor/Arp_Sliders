#ifndef SEGMENTVIDEO_H
#define SEGMENTVIDEO_H
#include <stdint.h>

#define FB_WATERMARK 16
#define FB_FIFO_SIZE 40

struct BufferChannels
{
	uint8_t * onBufferPtr;
	uint8_t * offBufferPtr;
};

class SegmentFrameBuffer
{
public:
	SegmentFrameBuffer(void);
	void write(const uint8_t *);
	bool ready( void );
	bool empty( void );
	BufferChannels read( void );
private:
	uint8_t onBuffer[FB_FIFO_SIZE][11];
	uint8_t offBuffer[FB_FIFO_SIZE][11];
	uint8_t outputPtr = 0;
	uint8_t nextToWrite = 1;
};

class SegmentVideo
{
public:
	SegmentVideo(void){};
	void displayDrawClockNums( const char * input );
	void displayDrawValue( const char * input );
	void setPlayIndicator( void );
	void clearPlayIndicator( void );
	void toggleClockColon(void);
	void writeNextFrame( void );
	
	uint8_t serialBuffer[11] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	SegmentFrameBuffer clockStream;
	SegmentFrameBuffer maskStream;
	SegmentFrameBuffer userStream;
private:
	uint8_t outputFrame[11];
};

#endif