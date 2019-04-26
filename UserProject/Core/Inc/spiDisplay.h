#ifndef DISPLAY_CLOCK_H
#define DISPLAY_CLOCK_H
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif
bool spiDisplayStartDMACommand(uint8_t command);
bool spiDisplayStartDMAData(uint8_t * data, uint16_t length);
void spiDisplayWriteData(uint8_t data);
void spiDisplayWriteCommand(uint8_t data);
void spiDisplayDisable(void);
void spiDisplayEnable(void);
bool spiDisplayBusy(void);
#ifdef __cplusplus
 }
#endif

#endif