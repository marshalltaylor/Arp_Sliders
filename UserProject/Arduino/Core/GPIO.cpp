/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"
//#include "PinConfigured.h"
#include "interface.h"
#include "STM_ArduinoPins.cpp"
#ifdef __cplusplus
extern "C" {
#endif


//This is the list of the IOs configured
//uint32_t g_digPinConfigured[MAX_NB_PORT] = {0};
//extern uint32_t g_anOutputPinConfigured[MAX_NB_PORT];


void pinMode(uint32_t ulPin, uint32_t ulMode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = DxToPin[ulPin];
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init((GPIO_TypeDef *) (AHB1PERIPH_BASE + DxToPort[ulPin]), &GPIO_InitStruct);
	

	
//  PinName p = digitalPinToPinName(ulPin);
//
//  if (p != NC) {
//    // If the pin that support PWM or DAC output, we need to turn it off
//    if (is_pin_configured(p, g_anOutputPinConfigured)) {
//#ifdef HAL_DAC_MODULE_ENABLED
//      if (pin_in_pinmap(p, PinMap_DAC)) {
//        dac_stop(p);
//      } else
//#endif //HAL_DAC_MODULE_ENABLED
//        if (pin_in_pinmap(p, PinMap_PWM)) {
//          pwm_stop(p);
//        }
//      reset_pin_configured(p, g_anOutputPinConfigured);
//    }
//
//    switch (ulMode) {
//      case INPUT: /* INPUT_FLOATING */
//        pin_function(p, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
//        break;
//      case INPUT_PULLUP:
//        pin_function(p, STM_PIN_DATA(STM_MODE_INPUT, GPIO_PULLUP, 0));
//        break;
//      case INPUT_PULLDOWN:
//        pin_function(p, STM_PIN_DATA(STM_MODE_INPUT, GPIO_PULLDOWN, 0));
//        break;
//      case INPUT_ANALOG:
//        pin_function(p, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
//        break;
//      case OUTPUT:
//        pin_function(p, STM_PIN_DATA(STM_MODE_OUTPUT_PP, GPIO_NOPULL, 0));
//        break;
//      case OUTPUT_OPEN_DRAIN:
//        pin_function(p, STM_PIN_DATA(STM_MODE_OUTPUT_OD, GPIO_NOPULL, 0));
//        break;
//      default:
//        Error_Handler();
//        break;
//    }
//    set_pin_configured(p, g_digPinConfigured);
//  }
}

void digitalWrite(uint32_t ulPin, uint32_t ulVal)
{
	//void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
	GPIO_PinState value = GPIO_PIN_RESET;
	if( ulVal ) value = GPIO_PIN_SET;
	HAL_GPIO_WritePin((GPIO_TypeDef *) (AHB1PERIPH_BASE + DxToPort[ulPin]), DxToPin[ulPin], value);
	//HAL_GPIO_TogglePin(GPIOJ, GPIO_PIN_13);
//  digitalWriteFast(digitalPinToPinName(ulPin), ulVal);
}

int digitalRead(uint32_t ulPin)
{
	//GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	if( HAL_GPIO_ReadPin((GPIO_TypeDef *) (AHB1PERIPH_BASE + DxToPort[ulPin]), DxToPin[ulPin]) == GPIO_PIN_SET ){
		return 1;
	}
	return 0;
	
}

void digitalToggle(uint32_t ulPin)
{
//  digitalToggleFast(digitalPinToPinName(ulPin));
}

#ifdef __cplusplus
}
#endif
