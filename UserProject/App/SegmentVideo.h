#ifndef SEGMENTVIDEO_H
#define SEGMENTVIDEO_H
#include <stdint.h>

#define FB_WATERMARK 16
#define FB_FIFO_SIZE 40

class SegmentFrameBuffer
{
public:
	SegmentFrameBuffer(void);
	void write(const uint8_t *);
	bool ready( void );
	bool empty( void );
	uint8_t * read( void );
private:
	uint8_t buffer[FB_FIFO_SIZE][11];
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
};

#endif