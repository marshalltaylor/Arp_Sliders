#ifndef BSPSYS_H
#define BSPSYS_H

#include <stdint.h>

void bspFlashRead(uint8_t * pData, uint32_t len);
void bspFlashWrite(uint8_t * pData, uint32_t len);

#endif
