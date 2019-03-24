#ifndef LUX_PANEL_H
#define LUX_PANEL_H

#include "TeensyView.h"

#define ROW 128

#define ROW0 0
#define ROW1 128
#define ROW2 256
#define ROW3 384

class OLEDFunctions : public TeensyView
{
public:
	OLEDFunctions(uint8_t rst, uint8_t dc, uint8_t cs, uint8_t sck, uint8_t mosi) : TeensyView(rst, dc, cs, sck, mosi) {};//(uint8_t rst, uint8_t dc, uint8_t cs, uint8_t sck, uint8_t mosi)
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
	void drawSeperator1( void ){
		line(0,8,127,8);
	};
	void drawRightArrow( void ){
		drawByte(122,0,0x10);
		drawByte(123,0,0x10);
		drawByte(124,0,0x7C);
		drawByte(125,0,0x38);
		drawByte(126,0,0x10);
	};
	void drawLeftArrow( void ){
		drawByte(0,0,0x10);
		drawByte(1,0,0x38);
		drawByte(2,0,0x7C);
		drawByte(3,0,0x10);
		drawByte(4,0,0x10);
	};
	void drawLeftBracket( uint8_t xIn, uint8_t yIn ){
		drawByte(xIn + 2,yIn + 0,0x7F);
		drawByte(xIn + 3,yIn + 0,0xFF);
		drawByte(xIn + 4,yIn + 0,0xC0);
		drawByte(xIn + 5,yIn + 0,0xC0);

		drawByte(xIn + 0,yIn + 8,0x20);
		drawByte(xIn + 1,yIn + 8,0x70);
		drawByte(xIn + 2,yIn + 8,0xFF);
		drawByte(xIn + 3,yIn + 8,0xDF);

		drawByte(xIn + 2,yIn + 16,0xF0);
		drawByte(xIn + 3,yIn + 16,0xF8);
		drawByte(xIn + 4,yIn + 16,0x18);
		drawByte(xIn + 5,yIn + 16,0x18);
	};
	void drawRightBracket( uint8_t xIn, uint8_t yIn ){
		drawByte(xIn + 0,yIn + 0,0xC0);
		drawByte(xIn + 1,yIn + 0,0xC0);
		drawByte(xIn + 2,yIn + 0,0xFF);
		drawByte(xIn + 3,yIn + 0,0x7F);

		drawByte(xIn + 2,yIn + 8,0xDF);
		drawByte(xIn + 3,yIn + 8,0xFF);
		drawByte(xIn + 4,yIn + 8,0x70);
		drawByte(xIn + 5,yIn + 8,0x20);

		drawByte(xIn + 0,yIn + 16,0x18);
		drawByte(xIn + 1,yIn + 16,0x18);
		drawByte(xIn + 2,yIn + 16,0xF8);
		drawByte(xIn + 3,yIn + 16,0xF0);
	};
	void eraseArrows( void ){
		rectFill(0,0,7,8,BLACK,NORM); //x,y,w,h,c,m
		rectFill(121,0,7,8,BLACK,NORM); //x,y,w,h,c,m
	};
	void eraseMenu( void ){
		rectFill(7,0,115,8,BLACK,NORM); //x,y,w,h,c,m
	};
	void eraseBracketArea( void ){
		rectFill(31,10,91,21,BLACK,NORM); //x,y,w,h,c,m
	};
	void eraseTypeArea( void ){
		rectFill(0,10,25,21,BLACK,NORM); //x,y,w,h,c,m
	};
	void eraseLowerArea( void ){
		rectFill(0,9,128,22,BLACK,NORM); //x,y,w,h,c,m
	};
	void eraseOuterLowerArea( void ){
		rectFill(0,9,24,30,BLACK,NORM); //x,y,w,h,c,m
	};
	void batteryStyle1( uint8_t xIn, uint8_t yIn, float percent )
	{
		drawByte(xIn + 0,yIn + 0,0xFE);
		drawByte(xIn + 1,yIn + 0,0x82);
		drawByte(xIn + 2,yIn + 0,0x82);
		drawByte(xIn + 3,yIn + 0,0x82);
		drawByte(xIn + 4,yIn + 0,0x82);
		drawByte(xIn + 5,yIn + 0,0x82);
		drawByte(xIn + 6,yIn + 0,0x82);
		drawByte(xIn + 7,yIn + 0,0x82);
		drawByte(xIn + 8,yIn + 0,0x82);
		drawByte(xIn + 9,yIn + 0,0xEE);
		drawByte(xIn + 10,yIn + 0,0x38);
		//line(xIn, yIn + 1, xIn + (percent * 10), yIn + 1);
		line(xIn + 2, yIn + 2, xIn + 2 + (percent * 6), yIn + 2);
		line(xIn + 2, yIn + 3, xIn + 2 + (percent * 6), yIn + 3);
		line(xIn + 2, yIn + 4, xIn + 2 + (percent * 6), yIn + 4);
		//line(xIn, yIn + 5, xIn + (percent * 10), yIn + 5);
		//rectFill(80 , 25, 84, 30);

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
	void drawPlug( uint8_t xIn, uint8_t yIn )
	{
		drawByte(xIn + 0,yIn + 0,0x28);
		drawByte(xIn + 1,yIn + 0,0x28);
		drawByte(xIn + 2,yIn + 0,0x7C);
		drawByte(xIn + 3,yIn + 0,0x44);
		drawByte(xIn + 4,yIn + 0,0x44);
		drawByte(xIn + 5,yIn + 0,0x38);
		drawByte(xIn + 6,yIn + 0,0x10);
		drawByte(xIn + 7,yIn + 0,0x10);
		drawByte(xIn + 8,yIn + 0,0x10);
	};
	void drawNo( uint8_t xIn, uint8_t yIn )
	{
		drawByte(xIn + 0,yIn + 0,0x38);
		drawByte(xIn + 1,yIn + 0,0x44);
		drawByte(xIn + 2,yIn + 0,0x8A);
		drawByte(xIn + 3,yIn + 0,0x92);
		drawByte(xIn + 4,yIn + 0,0xA2);
		drawByte(xIn + 5,yIn + 0,0x44);
		drawByte(xIn + 6,yIn + 0,0x38);
	};
	//int16_t ( void )
	uint8_t staffData[512];
	void clearStaffData( void );
	void drawStaff( void );
	void outputData( void );

private:
	int16_t lastSOC;
	int16_t lastChargerState = 0;
};

#endif