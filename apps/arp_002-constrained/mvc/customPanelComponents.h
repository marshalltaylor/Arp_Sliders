#ifndef CUSTOMPANELCOMPONENTS_H
#define CUSTOMPANELCOMPONENTS_H

#include "bsp.h"
#include "stdint.h"
#include "uCModules.h"

//Data for getting SPI data
//TODO: Should be somewhere else?  "SPI IO module" or something
extern spiDevice_t controlPanelSpi;
extern uint8_t spiTxBuf[4];
extern uint8_t spiRxBuf[4];
extern uint8_t SPIButtonData[4];
extern uint8_t SPILedData[4];

//---Selector----------------------------------------------------
class Selector : public KnobParentClass
{
public:
	Selector( void );
	~Selector( void );
	virtual void freshen( uint16_t msTickDelta );
	uint16_t getState( void );
	void init( uint16_t, uint16_t, uint8_t);//maxInput, minInput, number of points

protected:
private:
	uint16_t state;
	uint16_t lastState;
	uint16_t * thresholds;
	uint8_t points;
};

//---Knob--------------------------------------------------------
class RotoryEncoder : public PanelComponent
{
public:
	RotoryEncoder( void );
	void setHardware( GenericHardwareDescription * input );
	void freshen( uint16_t msTickDelta );
	void clear( void );
	uint8_t serviceChanged( void );
	int16_t getState_clicks( void );
	int16_t getState_clicks_delta( void );
private:
	int16_t state;
	uint8_t newData;
	int8_t lastValueForDelta;

};
#endif
