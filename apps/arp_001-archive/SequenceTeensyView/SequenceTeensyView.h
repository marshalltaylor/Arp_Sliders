#ifndef LUX_PANEL_H
#define LUX_PANEL_H

#include "SequenceCommon.h"
#include "MidiUtils.h"
#include "TeensyView.h"

#define ROW 128

#define ROW0 0
#define ROW1 128
#define ROW2 256
#define ROW3 384

typedef enum DAState_t
{
	DASTATE_IDLE,
	DASTATE_SET_LINE_0,
	DASTATE_SEND_LINE_0,
	DASTATE_SET_LINE_1,
	DASTATE_SEND_LINE_1,
	DASTATE_SET_LINE_2,
	DASTATE_SEND_LINE_2,
	DASTATE_SET_LINE_3,
	DASTATE_SEND_LINE_3
} STVState_t;

class SequenceTeensyView : public TeensyView
{
public:
	SequenceTeensyView(uint8_t rst, uint8_t dc, uint8_t cs, uint8_t sck, uint8_t mosi) : TeensyView(rst, dc, cs, sck, mosi) {};//(uint8_t rst, uint8_t dc, uint8_t cs, uint8_t sck, uint8_t mosi)
	
	void attachMainRegister( SequenceRegister * targetRegister );
	
	void drawByte(uint8_t x, uint8_t y, uint8_t c)//, uint8_t color, uint8_t mode);
	{
		uint8_t color = WHITE;
		uint8_t mode = NORM;
		for (int i =0;i<8;i++)
		{			// 8 is the LCD's page height (see datasheet for explanation)
			if (c & 0x1)
			{
				pixel(x, y+7-i, color,mode);
			}
			else
			{
				pixel(x, y+7-i, !color,mode);
			}
			c >>=1;
		}
	};
	void drawHeart( uint8_t xIn, uint8_t yIn )
	{
		drawByte(xIn + 0,yIn + 0,0x3C);
		drawByte(xIn + 1,yIn + 0,0x7F);
		drawByte(xIn + 2,yIn + 0,0xFF);
		drawByte(xIn + 3,yIn + 0,0xFF);
		drawByte(xIn + 4,yIn + 0,0xFF);
		drawByte(xIn + 5,yIn + 0,0xFF);
		drawByte(xIn + 6,yIn + 0,0x7F);
		drawByte(xIn + 7,yIn + 0,0x3F);
		drawByte(xIn + 8,yIn + 0,0x3F);
		drawByte(xIn + 9,yIn + 0,0x7F);
		drawByte(xIn + 10,yIn + 0,0xFF);
		drawByte(xIn + 11,yIn + 0,0xFF);
		drawByte(xIn + 12,yIn + 0,0xFF);
		drawByte(xIn + 13,yIn + 0,0xFF);
		drawByte(xIn + 14,yIn + 0,0x7F);
		drawByte(xIn + 15,yIn + 0,0x3C);

		drawByte(xIn + 0,yIn + 8,0x00);
		drawByte(xIn + 1,yIn + 8,0x00);
		drawByte(xIn + 2,yIn + 8,0x80);
		drawByte(xIn + 3,yIn + 8,0xC0);
		drawByte(xIn + 4,yIn + 8,0xE0);
		drawByte(xIn + 5,yIn + 8,0xF0);
		drawByte(xIn + 6,yIn + 8,0xF8);
		drawByte(xIn + 7,yIn + 8,0xFC);
		drawByte(xIn + 8,yIn + 8,0xFC);
		drawByte(xIn + 9,yIn + 8,0xF8);
		drawByte(xIn + 10,yIn + 8,0xF0);
		drawByte(xIn + 11,yIn + 8,0xE0);
		drawByte(xIn + 12,yIn + 8,0xC0);
		drawByte(xIn + 13,yIn + 8,0x80);
		drawByte(xIn + 14,yIn + 8,0x00);
		drawByte(xIn + 15,yIn + 8,0x00);	
	};

	uint8_t staffData[512];
	
	bool drawFullScreen( void );
	
	void clearStaffData( void );
	void drawStaffData_bar( uint16_t offset, uint8_t topByte, uint8_t bottomByte );
	void drawStaff( void );
	void drawNotes( float pixelsPerTick );
	void outputData( void );
	void setPlayHead( int16_t );
	void enableDirectAccess( void );
	void disableDirectAccess( void );
	bool isDirectAccessEnabled( void );
	void processDirectAccess( void );
	bool isProcessingFrame( void );
	
private:
	bool processingFrame = false;
	bool directAccessEnabled = false;
	DAState_t DAState = DASTATE_IDLE;
	int16_t lastSOC;
	int16_t lastChargerState = 0;
	int16_t playHead;
	SequenceRegister * mainRegister;
};

#endif