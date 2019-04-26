//#include "midiLoops.h"
//#include "globals.h"
#include "SequenceTeensyView.h"
#include "globals.h"
#include "MidiUtils.h"

// STM layer includes -- direct access to DMA
#include "spiDisplay.h"

void SequenceTeensyView::attachMainRegister( SequenceRegister * targetRegister )
{
	// Remove old association
	if( mainRegister )
	{
		mainRegister->removeUser(this);
	}
	// Subscribe to new register
	mainRegister = targetRegister->addUser(this);
}

//If the operation was a success, return true
bool SequenceTeensyView::drawFullScreen( void )
{
	//if( spiDisplayBusy() )
	//{
	//	return false;
	//}
	clearStaffData();
	drawStaff();
	if( isDirectAccessEnabled() )
	{
		DAState = DASTATE_SET_LINE_0;
	};
	
	//outputData();
	//display();
	
	//setCursor(0,0);
	//setPageAddress(0);
	//setColumnAddress(0);
	//if( !allowDirectAccess )
	//{
	//	return true; //act like all is OK
	//}

	return true;
}

void SequenceTeensyView::drawStaffData_bar( uint16_t offset, uint8_t topByte, uint8_t bottomByte )
{
	if( offset < 128 )
	{
		staffData[ROW0 + offset] |= topByte;
		staffData[ROW1 + offset] |= bottomByte;
	}
	else if( offset < 256 )
	{
		offset -= 128;
		staffData[ROW2 + offset] |= topByte;
		staffData[ROW3 + offset] |= bottomByte;
	}
}

void SequenceTeensyView::drawNotes( float pixelsPerTick )
{
	uint16_t tick = 0;
	Sequence * seq = mainRegister->getSequence();
	uint16_t currentStep = 0;
	bool gatedThisTick;
	uint16_t lastTickEdge = 0;
	uint8_t value;
	uint16_t stepLength = seq->ticksPerStep;
	uint16_t noteLength = stepLength / 2;
	uint16_t temp16bits = 0x0000;
	bool drawing = true;

	do
	{
		if( seq->step[currentStep].gated )
		{
			if( tick <= lastTickEdge + noteLength )
			{
				gatedThisTick = true;
			}
			else
			{
				gatedThisTick = false;
			}
		}
		
		if( gatedThisTick && (currentStep < seq->stepsUsed))
		{
			// Make 2 bytes for step
			value = seq->step[currentStep].value;
			if( (value >= 36)&&(value <= 84) )
			{
				value -= 36;
				value /= 4;  // now scaled 0 to 12
				value = value + 2; // 2 to 14
				temp16bits = 0x8000 >> value;		
		
				// Draw bytes
				drawStaffData_bar((float)tick * pixelsPerTick, ( temp16bits ), ( temp16bits >> 8 ));
			}
		}
		
		// increment
		tick++;
		
		switch( seq->loopingControl )
		{
			default:
			case LOOPING_DISABLED:
			{
				// Roll counters
				if( tick >= lastTickEdge + stepLength )
				{
					currentStep++;
					lastTickEdge += stepLength;
					if( currentStep == seq->stepsUsed )
					{
						drawing = false;
					}
				}
				break;
			}
			case LOOPING_AUTO:
			{
				// Roll counters
				if( tick >= lastTickEdge + stepLength )
				{
					currentStep++;
					lastTickEdge += stepLength;
					if( currentStep == seq->stepsUsed )
					{
						currentStep = 0;
					}
				}
				break;
			}
			case LOOPING_MANUAL:
			{
				// Roll counters
				if( tick >= lastTickEdge + stepLength )
				{
					currentStep++;
					lastTickEdge += stepLength;
					if( currentStep == seq->loopSequenceAtStep )
					{
						currentStep = 0;
					}
				}
				break;
			}
		}
		if( tick > seq->tapeLengthInTicks )
		{
			drawing = false;
		}
	}
	while( drawing );
}

void SequenceTeensyView::drawStaff( void )
{
	if( mainRegister == NULL )
	{
		return;
	}
	Sequence * seq = mainRegister->getSequence();
	

	// Calculate Geometric Scalar
	float pixelsPerTick = 1;
	bool drawExtraLineEnd1 = false;
	
	if( seq->tapeLengthInTicks >= 768 )
	{
		pixelsPerTick = 128.0 / 384.0;
		drawExtraLineEnd1 = true;
	}
	else if( seq->tapeLengthInTicks > 384 )
	{
		pixelsPerTick = 128.0 / 384.0;
		drawExtraLineEnd1 = true;
	}
	else if( seq->tapeLengthInTicks > 192 )
	{
		pixelsPerTick = 128.0 / 192.0;
		drawExtraLineEnd1 = true;
	}
	else if( seq->tapeLengthInTicks > 96 )
	{
		pixelsPerTick = 128.0 / 96.0;
		drawExtraLineEnd1 = true;
	}
	else
	{
		pixelsPerTick = 128.0 / 96.0;
	}
	
	drawNotes( pixelsPerTick );

	
	// Draw the start cap
	drawStaffData_bar(0, 0xFF, 0xFF);
	drawStaffData_bar(1, 0x01, 0x80);
	
	// Draw the bars
	int16_t bars = seq->tapeLengthInTicks / 97; // tuned float here
	float pixelsPerBar = pixelsPerTick * 96.0;
	int i;
	for( i = 1; (i < bars + 1) && (i < 8); i++ )
	{
		drawStaffData_bar(pixelsPerBar * i, 0xFE, 0x7F);
	}
	// ...and the extra bar for wrapping
	if( drawExtraLineEnd1 )
	{
		drawStaffData_bar(127, 0xFE, 0x7F);
	}
	// draw the bar itself
	uint16_t barOffset = ((float)seq->tapeLengthInTicks * (float)pixelsPerBar) / 96;
	
	// Draw the end cap
	drawStaffData_bar(barOffset - 2, 0x01, 0x80);
	drawStaffData_bar(barOffset - 1, 0xFF, 0xFF);
	
	barOffset = ((float)playHead * (float)pixelsPerBar) / 96;
	
	// Draw the play head
	drawStaffData_bar(barOffset - 2, 0x07, 0x00);
	drawStaffData_bar(barOffset - 1, 0xFC, 0x0F);
	drawStaffData_bar(barOffset - 0, 0x07, 0x00);

};

void SequenceTeensyView::outputData( void )
{
	setCursor(0,0);
	drawBitmap(staffData);
};

void SequenceTeensyView::clearStaffData( void )
{
	memset(staffData, 0, sizeof(staffData));
};

void SequenceTeensyView::setPlayHead( int16_t ticks )
{
	// TODO: make this safe against huge input numbers.
	Sequence * seq = mainRegister->getSequence();
	playHead = ticks;
	while( playHead > seq->tapeLengthInTicks )
	{
		playHead -= seq->tapeLengthInTicks;
	}
};

void SequenceTeensyView::enableDirectAccess( void )
{
	directAccessEnabled = true;
	DAState = DASTATE_SET_LINE_0;
};

void SequenceTeensyView::disableDirectAccess( void )
{
	directAccessEnabled = false;
};

bool SequenceTeensyView::isDirectAccessEnabled( void )
{
	return directAccessEnabled;
};


void SequenceTeensyView::processDirectAccess( void )
{
	if( !directAccessEnabled )
	{
		DAState = DASTATE_IDLE;
		return;
	}
	if( spiDisplayBusy() )
	{
		return;
	}
	switch( DAState )
	{
		default:
		case DASTATE_IDLE:
		{
			processingFrame = false;
			break;
		}
		case DASTATE_SET_LINE_0:
		{
			processingFrame = true;
			spiDisplayStartDMACommand(0xb0);
			DAState = DASTATE_SEND_LINE_0;
			break;
		}
		case DASTATE_SEND_LINE_0:
		{
			spiDisplayStartDMAData(staffData, 128);
			DAState = DASTATE_SET_LINE_1;
			break;
		}
		case DASTATE_SET_LINE_1:
		{
			spiDisplayStartDMACommand(0xb1);
			DAState = DASTATE_SEND_LINE_1;
			break;
		}
		case DASTATE_SEND_LINE_1:
		{
			spiDisplayStartDMAData(staffData + 128, 128);
			DAState = DASTATE_SET_LINE_2;
			break;
		}
		case DASTATE_SET_LINE_2:
		{
			spiDisplayStartDMACommand(0xb2);
			DAState = DASTATE_SEND_LINE_2;
			break;
		}
		case DASTATE_SEND_LINE_2:
		{
			spiDisplayStartDMAData(staffData + 256, 128);
			DAState = DASTATE_SET_LINE_3;
			break;
		}
		case DASTATE_SET_LINE_3:
		{
			spiDisplayStartDMACommand(0xb3);
			DAState = DASTATE_SEND_LINE_3;
			break;
		}
		case DASTATE_SEND_LINE_3:
		{
			spiDisplayStartDMAData(staffData + 384, 128);
			DAState = DASTATE_IDLE;
			break;
		}
	}
};

bool SequenceTeensyView::isProcessingFrame( void )
{
	return processingFrame;
};
