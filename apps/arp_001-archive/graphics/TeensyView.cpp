/******************************************************************************
TeensyView.cpp
Main source code for the TeensyView Library

Marshall Taylor @ SparkFun Electronics, December 6, 2016
https://github.com/sparkfun/SparkFun_TeensyView_Arduino_Library

This has been modified from the SFE_MicroOLED library, Original contributors:

Jim Lindblom @ SparkFun Electronics, October 26, 2014
https://github.com/sparkfun/Micro_OLED_Breakout/tree/master/Firmware/Arduino/libraries/SFE_MicroOLED

Emil Varughese @ Edwin Robotics Pvt. Ltd. July 27, 2015
https://github.com/emil01/SparkFun_Micro_OLED_Arduino_Library/

GeekAmmo, (https://github.com/geekammo/MicroView-Arduino-Library)

Released under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

*****This file defines the hardware interface(s) for the TeensyView.*****

Development environment specifics:
Arduino IDE 1.6.12 w/ Teensyduino 1.31
Arduino IDE 1.8.1 w/ Teensyduino 1.35
TeensyView v1.0

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#include <stdbool.h>
#include "TeensyView.h"

// Add header of the fonts here.  Remove as many as possible to conserve FLASH memory.
#include "font5x7.h"
#include "font8x16.h"
#include "fontlargenumber.h"
#include "sevenSegment.h"
#include "spiDisplay.h"

// Change the total fonts included
#define TOTALFONTS		4

#define _BV(bit) (1 << (bit)) 

// Add the font name as declared in the header file.  Remove as many as possible to conserve FLASH memory.
const unsigned char *TeensyView::fontsPointer[]={
	font5x7
	,font8x16
	,sevensegment
	,fontlargenumber
};

/** \brief TeensyView screen buffer.

Page buffer is LCDMEMORYSIZE, defined as LCDWIDTH * LCDHEIGHT / 8
128 * 32 / = 512 bytes for the TeensyView

Page buffer is required because in SPI mode, the host cannot read the SSD1306's GDRAM of the controller.
This page buffer serves as a scratch RAM for graphical functions.  All drawing function will first be drawn
on this page buffer, only upon calling display() function will transfer the page buffer to the actual LCD
controller's memory.

As a default, this is filled with the SparkFun logo
*/

//SparkFun Electronics Logo
//
//LCD Memory organised in 128 horizontal pixel and 4 rows of bytes
//  Index:  0  1............127	 
//                                
//  Bits:  D0 D0.............D0  ----
//         D1 D1.............D1      |
//         D2 D2.............D2      |
//         D3 D3.............D3      | ROW 0
//         D4 D4.............D4      |
//         D5 D5.............D5      |
//         D6 D6.............D6      |
//         D7 D7.............D7  ----
//
static uint8_t screenmemory [LCDMEMORYSIZE] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x78, 0xF8, 0xFC, 0xF8, 0xE0, 0xE0,
0xE0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFC, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE,
0xFC, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC,
0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0C, 0x38, 0x70, 0x70,
0xE0, 0xE0, 0xE0, 0xE0, 0xFE, 0xFC, 0xF8, 0xF0, 0xF8, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xC0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x01, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x1F, 0x1F, 0x0F, 0x0F, 0x07, 0x03, 0x03,
0x03, 0x07, 0x0F, 0x0F, 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x3F,
0x1F, 0x0F, 0x0F, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x03, 0x0C,
0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xF1, 0xFF, 0xFF, 0xFF, 0x87, 0xCF, 0xFF,
0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x80, 0xC0, 0xFF, 0xFF, 0xFF, 0x1F, 0x8F, 0xC7, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/** \brief TeensyView Constructor -- SPI Mode

	Setup the TeensyView class, configure the display to be controlled via a
	SPI interface.
*/
TeensyView::TeensyView(uint8_t rst, uint8_t dc, uint8_t cs, uint8_t sck, uint8_t mosi)
{
	// Assign each of the parameters to a private class variable.
	rstPin = rst;
	dcPin = dc;
	csPin = cs;
	sckPin = sck;
	mosiPin = mosi;
	
	clockRateSetting = 1000000;//Default rate of 1 MHz
}

//Set a non-default clock rate.  Run this before begin if alt rate is desired.
void TeensyView::setClockRate( uint32_t inputClockRate )
{
	clockRateSetting = inputClockRate;
}

/** \brief Initialisation of TeensyView Library.

    Setup IO pins for SPI port then send initialisation commands to the SSD1306 controller inside the OLED.
*/
void TeensyView::begin()
{
	// default 5x7 font
	setFontType(0);
	setColor(WHITE);
	setDrawMode(NORM);
	setCursor(0,0);

	// Display reset routine
	delay(5);	// VDD (3.3V) goes high at start, lets just chill for 5 ms
	spiDisplayDisable();	// Set RST pin as OUTPUT
	delay(10);	// wait 10ms
	spiDisplayEnable();
	
	// Display Init sequence for 64x48 OLED module
	spiDisplayWriteCommand(DISPLAYOFF);			// 0xAE

	spiDisplayWriteCommand(SETDISPLAYCLOCKDIV);	// 0xD5
	spiDisplayWriteCommand(0x80);					// the suggested ratio 0x80

	spiDisplayWriteCommand(SETMULTIPLEX);			// 0xA8
//	spiDisplayWriteCommand(0x2F);
	spiDisplayWriteCommand(0x1F);

	spiDisplayWriteCommand(SETDISPLAYOFFSET);		// 0xD3
	spiDisplayWriteCommand(0x0);					// no offset

//	spiDisplayWriteCommand(SETSTARTLINE | 0x0);	// line #0
	spiDisplayWriteCommand(SETSTARTLINE | 0x40);	// for wide 128x32

	spiDisplayWriteCommand(CHARGEPUMP);			// enable charge pump
	spiDisplayWriteCommand(0x14);

	spiDisplayWriteCommand(SEGREMAP | 0x1);

	//Not sure what these are
//	spiDisplayWriteCommand(NORMALDISPLAY);			// 0xA6
//	spiDisplayWriteCommand(DISPLAYALLONRESUME);	// 0xA4/

	spiDisplayWriteCommand(COMSCANDEC);

	spiDisplayWriteCommand(SETCOMPINS);			// 0xDA
//	spiDisplayWriteCommand(0x12);
	spiDisplayWriteCommand(0x02);					// for wide 128x32
	
	spiDisplayWriteCommand(SETCONTRAST);			// 0x81
	spiDisplayWriteCommand(0x8F);

	spiDisplayWriteCommand(SETPRECHARGE);			// 0xd9
	spiDisplayWriteCommand(0xF1);

	spiDisplayWriteCommand(SETVCOMDESELECT);			// 0xDB
	spiDisplayWriteCommand(0x40);
	//spiDisplayWriteCommand(0x00);
	
	spiDisplayWriteCommand(DISPLAYON);				//--turn on oled panel
	clear(ALL);						// Erase hardware memory inside the OLED controller to avoid random data in memory.
}

void TeensyView::end()
{
	spiDisplayDisable();
}

/** \brief Set SSD1306 page address.

    Send page address command and address to the SSD1306 OLED controller.
*/
void TeensyView::setPageAddress(uint8_t add) {
	add=0xb0|add;
	spiDisplayWriteCommand(add);
	return;
}

/** \brief Set SSD1306 column address.

    Send column address command and address to the SSD1306 OLED controller.
*/
void TeensyView::setColumnAddress(uint8_t add) {
	//spiDisplayWriteCommand((0x10|(add>>4))+0x02);
	//spiDisplayWriteCommand((0x0f&add));
	spiDisplayWriteCommand(0x21);
	spiDisplayWriteCommand(add & 0x7F);
	spiDisplayWriteCommand(0x7F);
	
	return;
}

/** \brief Clear screen buffer or SSD1306's memory.

    To clear GDRAM inside the LCD controller, pass in the variable mode = ALL and to clear screen page buffer pass in the variable mode = PAGE.
*/
void TeensyView::clear(uint8_t mode) {
	if (mode==ALL) {
		for (int i=0;i<8; i++) {
			setPageAddress(i);
			setColumnAddress(0);
			for (int j=0; j<0x80; j++) {
				spiDisplayWriteData(0);
			}
		}
	}
	else
	{
		memset(screenmemory,0,LCDMEMORYSIZE);			// write zeros on MCU side buffer
		//display();
	}
}

/** \brief Clear or replace screen buffer or SSD1306's memory with a character.

	To clear GDRAM inside the LCD controller, pass in the variable mode = ALL with c character and to clear screen page buffer, pass in the variable mode = PAGE with c character.
*/
void TeensyView::clear(uint8_t mode, uint8_t c) {
	if (mode==ALL) {
		for (int i=0;i<8; i++) {
			setPageAddress(i);
			setColumnAddress(0);
			for (int j=0; j<0x80; j++) {
				spiDisplayWriteData(c);
			}
		}
	}
	else
	{
		memset(screenmemory,c,LCDMEMORYSIZE);			// write 'c' on MCU side buffer
		display();
	}
}

/** \brief Invert display.

    The WHITE color of the display will turn to BLACK and the BLACK will turn to WHITE.
*/
void TeensyView::invert(bool inv) {
	if (inv)
	spiDisplayWriteCommand(INVERTDISPLAY);
	else
	spiDisplayWriteCommand(NORMALDISPLAY);
}

/** \brief Set contrast.

    OLED contract value from 0 to 255. Note: Contrast level is not very obvious.
*/
void TeensyView::contrast(uint8_t contrast) {
	spiDisplayWriteCommand(SETCONTRAST);			// 0x81
	spiDisplayWriteCommand(contrast);
}

/** \brief Transfer display memory.

    Bulk move the screen buffer to the SSD1306 controller's memory so that images/graphics drawn on the screen buffer will be displayed on the OLED.
*/
void TeensyView::display(void) {
	uint8_t i, j;

	for (i=0; i<(LCDHEIGHT/8); i++) {
		setPageAddress(i);
		setColumnAddress(0);
		for (j=0;j<LCDWIDTH;j++) {
			spiDisplayWriteData(screenmemory[i*LCDWIDTH+j]);
		}
	}
}

void TeensyView::display(uint8_t i) {
	uint8_t j;

	setPageAddress(i);
	setColumnAddress(0);
	for (j=0;j<LCDWIDTH;j++) {
		spiDisplayWriteData(screenmemory[i*LCDWIDTH+j]);
	}
}

/** \brief Override Arduino's Print.

    Arduino's print overridden so that we can use uView.print().
*/
size_t TeensyView::write(uint8_t c) {
	if (c == '\n') {
		cursorY += fontHeight;
		cursorX  = 0;
	} else if (c == '\r') {
		// skip
	} else {
		drawChar(cursorX, cursorY, c, foreColor, drawMode);
		cursorX += fontWidth+1;
		if ((cursorX > (LCDWIDTH - fontWidth))) {
			cursorY += fontHeight;
			cursorX = 0;
		}
	}

	return 1;
}

/** \brief Set cursor position.

TeensyView's cursor position to x,y.
*/
void TeensyView::setCursor(uint8_t x, uint8_t y) {
	cursorX=x;
	cursorY=y;
}

/** \brief Draw pixel.

Draw pixel using the current fore color and current draw mode in the screen buffer's x,y position.
*/
void TeensyView::pixel(uint8_t x, uint8_t y) {
	pixel(x,y,foreColor,drawMode);
}

/** \brief Draw pixel with color and mode.

Draw color pixel in the screen buffer's x,y position with NORM or XOR draw mode.
*/
void TeensyView::pixel(uint8_t x, uint8_t y, uint8_t color, uint8_t mode) {
	if ((x<0) ||  (x>=LCDWIDTH) || (y<0) || (y>=LCDHEIGHT))
	return;

	if (mode==XOR) {
		if (color==WHITE)
		screenmemory[x+ (y/8)*LCDWIDTH] ^= _BV((y%8));
	}
	else {
		if (color==WHITE)
		screenmemory[x+ (y/8)*LCDWIDTH] |= _BV((y%8));
		else
		screenmemory[x+ (y/8)*LCDWIDTH] &= ~_BV((y%8));
	}
}

/** \brief Draw line.

Draw line using current fore color and current draw mode from x0,y0 to x1,y1 of the screen buffer.
*/
void TeensyView::line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	line(x0,y0,x1,y1,foreColor,drawMode);
}

/** \brief Draw line with color and mode.

Draw line using color and mode from x0,y0 to x1,y1 of the screen buffer.
*/
void TeensyView::line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color, uint8_t mode) {
	uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	uint8_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int8_t err = dx / 2;
	int8_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;}

	for (; x0<x1; x0++) {
		if (steep) {
			pixel(y0, x0, color, mode);
		} else {
			pixel(x0, y0, color, mode);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

/** \brief Draw horizontal line.

Draw horizontal line using current fore color and current draw mode from x,y to x+width,y of the screen buffer.
*/
void TeensyView::lineH(uint8_t x, uint8_t y, uint8_t width) {
	line(x,y,x+width,y,foreColor,drawMode);
}

/** \brief Draw horizontal line with color and mode.

Draw horizontal line using color and mode from x,y to x+width,y of the screen buffer.
*/
void TeensyView::lineH(uint8_t x, uint8_t y, uint8_t width, uint8_t color, uint8_t mode) {
	line(x,y,x+width,y,color,mode);
}

/** \brief Draw vertical line.

Draw vertical line using current fore color and current draw mode from x,y to x,y+height of the screen buffer.
*/
void TeensyView::lineV(uint8_t x, uint8_t y, uint8_t height) {
	line(x,y,x,y+height,foreColor,drawMode);
}

/** \brief Draw vertical line with color and mode.

Draw vertical line using color and mode from x,y to x,y+height of the screen buffer.
*/
void TeensyView::lineV(uint8_t x, uint8_t y, uint8_t height, uint8_t color, uint8_t mode) {
	line(x,y,x,y+height,color,mode);
}

/** \brief Draw rectangle.

Draw rectangle using current fore color and current draw mode from x,y to x+width,y+height of the screen buffer.
*/
void TeensyView::rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
	rect(x,y,width,height,foreColor,drawMode);
}

/** \brief Draw rectangle with color and mode.

Draw rectangle using color and mode from x,y to x+width,y+height of the screen buffer.
*/
void TeensyView::rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color , uint8_t mode) {
	uint8_t tempHeight;

	lineH(x,y, width, color, mode);
	lineH(x,y+height-1, width, color, mode);

	tempHeight=height-2;

	// skip drawing vertical lines to avoid overlapping of pixel that will
	// affect XOR plot if no pixel in between horizontal lines
	if (tempHeight<1) return;

	lineV(x,y+1, tempHeight, color, mode);
	lineV(x+width-1, y+1, tempHeight, color, mode);
}

/** \brief Draw filled rectangle.

Draw filled rectangle using current fore color and current draw mode from x,y to x+width,y+height of the screen buffer.
*/
void TeensyView::rectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
	rectFill(x,y,width,height,foreColor,drawMode);
}

/** \brief Draw filled rectangle with color and mode.

Draw filled rectangle using color and mode from x,y to x+width,y+height of the screen buffer.
*/
void TeensyView::rectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color , uint8_t mode) {
	// TODO - need to optimise the memory map draw so that this function will not call pixel one by one
	for (int i=x; i<x+width;i++) {
		lineV(i,y, height, color, mode);
	}
}

/** \brief Draw circle.

    Draw circle with radius using current fore color and current draw mode at x,y of the screen buffer.
*/
void TeensyView::circle(uint8_t x0, uint8_t y0, uint8_t radius) {
	circle(x0,y0,radius,foreColor,drawMode);
}

/** \brief Draw circle with color and mode.

Draw circle with radius using color and mode at x,y of the screen buffer.
*/
void TeensyView::circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color, uint8_t mode) {
	//TODO - find a way to check for no overlapping of pixels so that XOR draw mode will work perfectly
	int8_t f = 1 - radius;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * radius;
	int8_t x = 0;
	int8_t y = radius;

	pixel(x0, y0+radius, color, mode);
	pixel(x0, y0-radius, color, mode);
	pixel(x0+radius, y0, color, mode);
	pixel(x0-radius, y0, color, mode);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		pixel(x0 + x, y0 + y, color, mode);
		pixel(x0 - x, y0 + y, color, mode);
		pixel(x0 + x, y0 - y, color, mode);
		pixel(x0 - x, y0 - y, color, mode);

		pixel(x0 + y, y0 + x, color, mode);
		pixel(x0 - y, y0 + x, color, mode);
		pixel(x0 + y, y0 - x, color, mode);
		pixel(x0 - y, y0 - x, color, mode);

	}
}

/** \brief Draw filled circle.

    Draw filled circle with radius using current fore color and current draw mode at x,y of the screen buffer.
*/
void TeensyView::circleFill(uint8_t x0, uint8_t y0, uint8_t radius) {
	circleFill(x0,y0,radius,foreColor,drawMode);
}

/** \brief Draw filled circle with color and mode.

    Draw filled circle with radius using color and mode at x,y of the screen buffer.
*/
void TeensyView::circleFill(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color, uint8_t mode) {
	// TODO - - find a way to check for no overlapping of pixels so that XOR draw mode will work perfectly
	int8_t f = 1 - radius;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * radius;
	int8_t x = 0;
	int8_t y = radius;

	// Temporary disable fill circle for XOR mode.
	if (mode==XOR) return;

	for (uint8_t i=y0-radius; i<=y0+radius; i++) {
		pixel(x0, i, color, mode);
	}

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		for (uint8_t i=y0-y; i<=y0+y; i++) {
			pixel(x0+x, i, color, mode);
			pixel(x0-x, i, color, mode);
		}
		for (uint8_t i=y0-x; i<=y0+x; i++) {
			pixel(x0+y, i, color, mode);
			pixel(x0-y, i, color, mode);
		}
	}
}

/** \brief Get LCD height.

    The height of the LCD return as byte.
*/
uint8_t TeensyView::getLCDHeight(void) {
	return LCDHEIGHT;
}

/** \brief Get LCD width.

    The width of the LCD return as byte.
*/
uint8_t TeensyView::getLCDWidth(void) {
	return LCDWIDTH;
}

/** \brief Get font width.

    The cucrrent font's width return as byte.
*/
uint8_t TeensyView::getFontWidth(void) {
	return fontWidth;
}

/** \brief Get font height.

    The current font's height return as byte.
*/
uint8_t TeensyView::getFontHeight(void) {
	return fontHeight;
}

/** \brief Get font starting character.

    Return the starting ASCII character of the currnet font, not all fonts start with ASCII character 0. Custom fonts can start from any ASCII character.
*/
uint8_t TeensyView::getFontStartChar(void) {
	return fontStartChar;
}

/** \brief Get font total characters.

    Return the total characters of the current font.
*/
uint8_t TeensyView::getFontTotalChar(void) {
	return fontTotalChar;
}

/** \brief Get total fonts.

    Return the total number of fonts loaded into the TeensyView's flash memory.
*/
uint8_t TeensyView::getTotalFonts(void) {
	return TOTALFONTS;
}

/** \brief Get font type.

    Return the font type number of the current font.
*/
uint8_t TeensyView::getFontType(void) {
	return fontType;
}

/** \brief Set font type.

    Set the current font type number, ie changing to different fonts base on the type provided.
*/
uint8_t TeensyView::setFontType(uint8_t type) {
	if ((type>=TOTALFONTS) || (type<0))
	return false;

	fontType=type;
	fontWidth=*(fontsPointer[fontType]+0);
	fontHeight=*(fontsPointer[fontType]+1);
	fontStartChar=*(fontsPointer[fontType]+2);
	fontTotalChar=*(fontsPointer[fontType]+3);
	fontMapWidth=(*(fontsPointer[fontType]+4)*100)+*(fontsPointer[fontType]+5); // two bytes values into integer 16
	return true;
}

/** \brief Set color.

    Set the current draw's color. Only WHITE and BLACK available.
*/
void TeensyView::setColor(uint8_t color) {
	foreColor=color;
}

/** \brief Set draw mode.

    Set current draw mode with NORM or XOR.
*/
void TeensyView::setDrawMode(uint8_t mode) {
	drawMode=mode;
}

/** \brief Draw character.

    Draw character c using current color and current draw mode at x,y.
*/
void  TeensyView::drawChar(uint8_t x, uint8_t y, uint8_t c) {
	drawChar(x,y,c,foreColor,drawMode);
}

/** \brief Draw character with color and mode.

    Draw character c using color and draw mode at x,y.
*/
void  TeensyView::drawChar(uint8_t x, uint8_t y, uint8_t c, uint8_t color, uint8_t mode) {
	// TODO - New routine to take font of any height, at the moment limited to font height in multiple of 8 pixels

	uint8_t rowsToDraw,row, tempC;
	uint8_t i,j,temp;
	uint16_t charPerBitmapRow,charColPositionOnBitmap,charRowPositionOnBitmap,charBitmapStartPosition;

	if ((c<fontStartChar) || (c>(fontStartChar+fontTotalChar-1)))		// no bitmap for the required c
	return;

	tempC=c-fontStartChar;

	// each row (in datasheet is call page) is 8 bits high, 16 bit high character will have 2 rows to be drawn
	rowsToDraw=fontHeight/8;	// 8 is LCD's page size, see SSD1306 datasheet
	if (rowsToDraw<=1) rowsToDraw=1;

	// the following draw function can draw anywhere on the screen, but SLOW pixel by pixel draw
	if (rowsToDraw==1) {
		for  (i=0;i<fontWidth+1;i++) {
			if (i==fontWidth) // this is done in a weird way because for 5x7 font, there is no margin, this code add a margin after col 5
			temp=0;
			else
			temp=*(fontsPointer[fontType]+FONTHEADERSIZE+(tempC*fontWidth)+i);

			for (j=0;j<8;j++) {			// 8 is the LCD's page height (see datasheet for explanation)
				if (temp & 0x1) {
					pixel(x+i, y+j, color,mode);
				}
				else {
					pixel(x+i, y+j, !color,mode);
				}

				temp >>=1;
			}
		}
		return;
	}

	// font height over 8 bit
	// take character "0" ASCII 48 as example
	charPerBitmapRow=fontMapWidth/fontWidth;  // 256/8 =32 char per row
	charColPositionOnBitmap=tempC % charPerBitmapRow;  // =16
	charRowPositionOnBitmap=int(tempC/charPerBitmapRow); // =1
	charBitmapStartPosition=(charRowPositionOnBitmap * fontMapWidth * (fontHeight/8)) + (charColPositionOnBitmap * fontWidth) ;

	// each row on LCD is 8 bit height (see datasheet for explanation)
	for(row=0;row<rowsToDraw;row++) {
		for (i=0; i<fontWidth;i++) {
			temp=*(fontsPointer[fontType]+FONTHEADERSIZE+(charBitmapStartPosition+i+(row*fontMapWidth)));
			for (j=0;j<8;j++) {			// 8 is the LCD's page height (see datasheet for explanation)
				if (temp & 0x1) {
					pixel(x+i,y+j+(row*8), color, mode);
				}
				else {
					pixel(x+i,y+j+(row*8), !color, mode);
				}
				temp >>=1;
			}
		}
	}

}

/** \brief Stop scrolling.

    Stop the scrolling of graphics on the OLED.
*/
void TeensyView::scrollStop(void){
	spiDisplayWriteCommand(DEACTIVATESCROLL);
}

/** \brief Right scrolling.

Set row start to row stop on the OLED to scroll right. Refer to http://learn.microview.io/intro/general-overview-of-microview.html for explanation of the rows.
*/
void TeensyView::scrollRight(uint8_t start, uint8_t stop){
	if (stop<start)		// stop must be larger or equal to start
	return;
	scrollStop();		// need to disable scrolling before starting to avoid memory corrupt
	spiDisplayWriteCommand(RIGHTHORIZONTALSCROLL);
	spiDisplayWriteCommand(0x00);
	spiDisplayWriteCommand(start);
	spiDisplayWriteCommand(0x7);		// scroll speed frames , TODO
	spiDisplayWriteCommand(stop);
	spiDisplayWriteCommand(0x00);
	spiDisplayWriteCommand(0xFF);
	spiDisplayWriteCommand(ACTIVATESCROLL);
}

/** \brief Vertical flip.

Flip the graphics on the OLED vertically.
*/
void TeensyView::flipVertical(bool flip) {
	if (flip) {
		spiDisplayWriteCommand(COMSCANINC);
	}
	else {
		spiDisplayWriteCommand(COMSCANDEC);
	}
}

/** \brief Horizontal flip.

    Flip the graphics on the OLED horizontally.
*/
void TeensyView::flipHorizontal(bool flip) {
	if (flip) {
		spiDisplayWriteCommand(SEGREMAP | 0x0);
	}
	else {
		spiDisplayWriteCommand(SEGREMAP | 0x1);
	}
}

/*
	Return a pointer to the start of the RAM screen buffer for direct access.
*/
uint8_t *TeensyView::getScreenBuffer(void) {
	return screenmemory;
}

/*
Draw Bitmap image on screen. The array for the bitmap can be stored in the Arduino file, so user don't have to mess with the library files.
To use, create uint8_t array that is 128x32 pixels (LCDMEMORYSIZE bytes). Then call .drawBitmap and pass it the array.
*/
void TeensyView::drawBitmap(uint8_t * bitArray)
{
  for (int i=0; i<LCDMEMORYSIZE; i++)
    screenmemory[i] = bitArray[i];
}
