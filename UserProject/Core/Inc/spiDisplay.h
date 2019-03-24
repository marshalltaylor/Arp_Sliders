#ifndef DISPLAY_CLOCK_H
#define DISPLAY_CLOCK_H
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif
void spiDisplayWriteData(uint8_t data);
void spiDisplayWriteCommand(uint8_t data);
void spiDisplayDisable(void);
void spiDisplayEnable(void);

#ifdef __cplusplus
 }
#endif

#endif