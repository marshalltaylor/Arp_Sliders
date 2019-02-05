#ifndef INTERFACE_H
#define INTERFACE_H

#include "stm32f4xx_hal.h"
#include "main.h"
#include "usart.h"
#include "HardwareSerial.h"

extern HardwareSerial Serial2;
extern HardwareSerial Serial6;

extern uint32_t usTicks;
extern uint8_t usTicksLocked;

void interface_init(void);
void delay(uint32_t delayInput);

#endif
