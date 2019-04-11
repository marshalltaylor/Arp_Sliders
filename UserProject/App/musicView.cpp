//#include "midiLoops.h"
//#include "globals.h"
#include "musicView.h"

void OLEDFunctions::drawStaff( void )
{
	//Disabled because Pattern has been changed
	
//	PatternData * pat = pattern.curPattern();
//	int16_t bars = pat->patternLength / 96;
//	int16_t beats = pat->patternLength % 96;
//	beats = ( beats * 32 ) / 96;
//	{
//		//char buffer[200] = {0};
//		//sprintf(buffer, "LEN == %d, beats == %d, bars == %d\n", pat->patternLength, beats, bars);
//		//Serial6.print(buffer);
//	}
//	staffData[ROW0] = 0xFF;
//	staffData[ROW1] = 0xFF;
//	staffData[ROW0 + 1] = 0x01;
//	staffData[ROW1 + 1] = 0x80;
//	switch( bars )
//	{
//		default:
//			staffData[ROW2 + 96] = 0xFE;
//			staffData[ROW3 + 96] = 0x7F;
//		case 6:
//			staffData[ROW2 + 64] = 0xFE;
//			staffData[ROW3 + 64] = 0x7F;
//		case 5:
//			staffData[ROW2 + 32] = 0xFE;
//			staffData[ROW3 + 32] = 0x7F;
//		case 4:
//			staffData[ROW0 + 127] = 0xFE;
//			staffData[ROW1 + 127] = 0x7F;
//		case 3:
//			staffData[ROW0 + 96] = 0xFE;
//			staffData[ROW1 + 96] = 0x7F;
//		case 2:
//			staffData[ROW0 + 64] = 0xFE;
//			staffData[ROW1 + 64] = 0x7F;
//		case 1:
//			staffData[ROW0 + 32] = 0xFE;
//			staffData[ROW1 + 32] = 0x7F;
//		case 0:
//		break;
//	}
//	int16_t xPos, yPos;
//	if( pat->patternLength < 384 )
//	{
//		xPos = (bars * 32) + beats;
//		yPos = ROW0;
//	}
//	else if( pat->patternLength == 384 )
//	{
//		xPos = 127;
//		yPos = 0;
//	}
//	else
//	{
//		// Draw extra left hand bar
//		staffData[ROW2 + 0] = 0xFE;
//		staffData[ROW3 + 0] = 0x7F;		
//		if( pat->patternLength < 768 )
//		{
//			bars -= 4;
//			xPos = (bars * 32) + beats;
//			yPos = ROW2;
//		}
//		else if( pat->patternLength == 768 )
//		{
//			xPos = 127;
//			yPos = ROW2;
//		}
//	}
//	staffData[yPos + xPos - 1] = 0x01;
//	staffData[yPos + ROW + xPos - 1] = 0x80;		
//	staffData[yPos + xPos + 0] = 0xFF;
//	staffData[yPos + ROW + xPos + 0] = 0xFF;		
};

void OLEDFunctions::outputData( void )
{
	setCursor(0,0);
	drawBitmap(staffData);
};

void OLEDFunctions::clearStaffData( void )
{
	memset(staffData, 0, sizeof(staffData));
};
