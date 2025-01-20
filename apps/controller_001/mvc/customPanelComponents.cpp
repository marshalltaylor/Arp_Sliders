#include "stdint.h"
#include "customPanelComponents.h"
#include "bsp.h"

#define localPrintf bspPrintf

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

//---Ranged Selector---------------------------------------------
RangedSelector::RangedSelector( void )
{
    points = 10;
}

RangedSelector::~RangedSelector( void )
{
    delete[] thresholds;
}

// 8 bit resolution on the ADC should be fine.
void RangedSelector::init( uint16_t maxInput, uint16_t minInput, uint8_t pointsInput )
{
    if( pointsInput < 2 )
    {
        points = 1;
    }
    else
    {
        points = pointsInput - 1; //( by (n-1) not n )
    }
    thresholds = new uint16_t[points];
    maxValue = maxInput;
    minValue = minInput;
    //Set up the ranges
    stepHeight = ( maxValue - minValue ) / points;
    thresholds[0] = minValue + ( stepHeight / 2 );
    int i;
    for( i = 1; i < points; i++ )
    {
        thresholds[i] = thresholds[i - 1] + stepHeight;
    
    }

}

void RangedSelector::freshen( uint16_t msTickDelta )
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
        newData = true;
        lastState = state;
    }
    
    //Do the analog part
    int8_t tempSlope = 0;
    analogState = freshData;
    int8_t histDirTemp = 0;
    if( analogState > analogLastState )
    {
        tempSlope = 1;
        if( lastSlope == 1 ) histDirTemp = 1;
    }
    else if( analogState < analogLastState )
    {
        tempSlope = -1;
        if( lastSlope == -1 ) histDirTemp = -1;
    }
    if( tempSlope != 0 )
    {
        if( analogState > analogLastState + hysteresis || histDirTemp == 1)
        {
            //localPrintf("d: %d ld: %d s: %d ls: %d\n", analogState, analogLastState, tempSlope, lastSlope);
            analogNewData = true;
            analogLastState = analogState;
            lastSlope = tempSlope;
        }
        if( analogState < analogLastState - hysteresis || histDirTemp == -1 )
        {
            //localPrintf("-d: %d ld: %d s: %d ls: %d\n", analogState, analogLastState, tempSlope, lastSlope);
            analogNewData = true;
            analogLastState = analogState;
            lastSlope = tempSlope;
        }

    }
}

uint16_t RangedSelector::getState( void )
{
   return state;
}

float RangedSelector::getStateRanged( int16_t low, int16_t high )
{
    if(low != 0)
    {
        low = thresholds[low - 1];//convert index to value
    }
    if(high >= points)
    {
        high = maxValue;
    }
    else
    {
        high = thresholds[high];
    }
    if(analogState > high) return 1.0;
    if(analogState < low) return 0;
    float aRange = high - low;
    float var = analogState - low;
    var = var / aRange;
    return var;
}

bool RangedSelector::serviceChanged( void )
{
    bool returnVar = false;
    if( newData )
    {
        returnVar = true;
        newData = false;
    }
    return returnVar;
}

bool RangedSelector::serviceChangedAnalog( void )
{
    if( analogNewData )
    {
        analogNewData = false;
        return true;
    }
    else
    {
        return false;
    }
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
