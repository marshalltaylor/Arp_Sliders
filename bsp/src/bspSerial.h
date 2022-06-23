#ifndef BSP_SERIAL
#define BSP_SERIAL

#include "bspUart.h"

void bspSerialStart(void);

extern UartInstance_t VCP_UART;
extern UartInstance_t D01_UART;
extern UartInstance_t D28_UART;

#endif
