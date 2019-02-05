#ifndef DISPLAY_CLOCK_H
#define DISPLAY_CLOCK_H
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif
	void writeDisplay(uint8_t *);
	void displayTransmitFrame(void);
#ifdef __cplusplus
 }
#endif

#endif