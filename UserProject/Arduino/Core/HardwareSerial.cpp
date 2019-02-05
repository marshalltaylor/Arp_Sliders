/*
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <inttypes.h>
//#include <assert.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#include "interface.h"

#define PinName int
// Constructors ////////////////////////////////////////////////////////////////
HardwareSerial::HardwareSerial(void)
{
}

void HardwareSerial::init(UartInstance* HAL_UART)
{
	_serial = HAL_UART;
}

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(unsigned long baud, uint8_t config)
{
	switch( config )
	{
		case 1:
		break;
		case 6:
		break;
		default:
		break;
	}
}

void HardwareSerial::end()
{
  flush();
}

int HardwareSerial::available(void)
{
  return halUartReadBytesAvailable(_serial);
}

int HardwareSerial::peek(void)
{
  return halUartPeek(_serial);
}

int HardwareSerial::read(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  return halUartRead(_serial);
}

int HardwareSerial::availableForWrite(void)
{
  return 256;
}

void HardwareSerial::flush()
{
}

size_t HardwareSerial::write(uint8_t c)
{
  halUartWrite(c, _serial);
  return 1;
}
