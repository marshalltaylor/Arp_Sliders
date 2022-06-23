#include "stdint.h"
#include "customPanelComponents.h"

//Data for getting SPI data
//TODO: Should be somewhere else?  "SPI IO module" or something
spiDevice_t controlPanelSpi;
uint8_t spiTxBuf[4];
uint8_t spiRxBuf[4];
uint8_t SPIButtonData[4];
uint8_t SPILedData[4];

//---Selector---------------------------------------------------------
Selector::Selector( void )
{
	points = 10;
}

Selector::~Selector( void )
{
	delete[] thresholds;
}

// 8 bit resolution on the ADC should be fine.
void Selector::init( uint16_t maxInput, uint16_t minInput, uint8_t pointsInput )
{
	if( pointsInput < 2 )
	{
		points = 2;
	}
	else
	{
		points = pointsInput - 1; //( by (n-1) not n )
	}
	thresholds = new uint16_t[points];

	//Set up the ranges
	uint16_t stepHeight = ( maxInput - minInput ) / points;
	thresholds[0] = minInput + ( stepHeight / 2 );
	int i;
	for( i = 1; i < points; i++ )
	{
		thresholds[i] = thresholds[i - 1] + stepHeight;
	
	}

}

void Selector::freshen( uint16_t msTickDelta )
{
	//Throw away input
	//Cause the interface to get the data
	hardwareInterface->readHardware();
	//Collect the data
	KnobDataObject tempObject;
	hardwareInterface->getData(&tempObject);
	
	uint16_t freshData = *(uint16_t *)tempObject.data;

	state = 0;
	//Seek the position
	int i;
	for( i = 0; i < points; i++ )
	{
		if( freshData > thresholds[i] )
		{
			state = i + 1; //It's this or higher
		}
	}
	//Check if new
	if( state != lastState )
	{
		newData = 1;
		lastState = state;
	}

}

uint16_t Selector::getState( void )
{
   return state;
}

//---Knob--------------------------------------------------------
RotoryEncoder::RotoryEncoder( void )
{
	newData = 0;
}

void RotoryEncoder::setHardware( GenericHardwareDescription * input )
{
	hardwareInterface = input;
}

void RotoryEncoder::freshen( uint16_t msTickDelta )
{
	//Throw away input

	//Cause the interface to get the data
	hardwareInterface->readHardware();
	
	//Collect the data
	KnobDataObject tempObject;
	hardwareInterface->getData(&tempObject);
	
	int16_t clicks = *(int16_t *)tempObject.data / 4;

	if(state != clicks)
	{
		//bspPrintf("BSP enc: %d, %d, %d\n", state, clicks, *(int16_t *)tempObject.data);
		state = clicks;
		newData = 1;
	}
}

void RotoryEncoder::clear( void )
{
	hardwareInterface->writeHardware();
	state = 0;
}

uint8_t RotoryEncoder::serviceChanged( void )
{
	uint8_t returnVar = 0;
	if( newData == 1 )
	{
		returnVar = 1;
		newData = 0;
	}
	return returnVar;
}

int16_t RotoryEncoder::getState_clicks( void )
{
	return state;
}

int16_t RotoryEncoder::getState_clicks_delta( void )
{
	int16_t retValue = 0;
	retValue = state - lastValueForDelta;
	lastValueForDelta = state;
	return retValue;
}
