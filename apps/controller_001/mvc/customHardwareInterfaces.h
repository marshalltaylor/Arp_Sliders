#ifndef CUSTOMHARDWAREINTEFACES_H
#define CUSTOMHARDWAREINTEFACES_H

#include "src/HardwareInterfaces.h"

class SpiDigitalIn : public GenericHardwareDescription
{
public:
	SpiDigitalIn( uint8_t inputBit, int inputByte );
	void readHardware( void );
protected:
	int bitIndex;
	int byteIndex;
};

class SpiDigitalOut : public GenericHardwareDescription
{
public:
	SpiDigitalOut( uint8_t inputBit, int inputByte );
	void writeHardware( void );
protected:
	int bitIndex;
	int byteIndex;
};

class RotoryEncoderObject : public DataObject
{
public:
	RotoryEncoderObject( void ){
		size = 2;
		data = new uint8_t[size];
	};
	~RotoryEncoderObject( void ){
		delete[] data;
	}
};	

//This is the most basic hardware type, the Arduino digital input
class bspEncoderIn : public GenericHardwareDescription
{
public:
	bspEncoderIn( int index );
	void readHardware( void );
	void writeHardware( void );
protected:
	int bspIndex;
};

#endif
