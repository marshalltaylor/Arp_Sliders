#ifndef SPI_DISPLAY_H
#define SPI_DISPLAY_H
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif
void spiDisplayWriteData(uint8_t data);
void spiDisplayWriteCommand(uint8_t data);
void spiDisplayDisable(void);
void spiDisplayEnable(void);
bool spiDisplayBusy(void);
#ifdef __cplusplus
 }
#endif

#endif