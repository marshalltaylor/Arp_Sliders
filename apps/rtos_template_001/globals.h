//Header
#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- modules -------------------------------------------------------*/
#include "logging.h"

/* Includes -- App -----------------------------------------------------------*/

/* References ----------------------------------------------------------------*/

////Defined in bspTime.h
//extern uint8_t usTicksLocked;
//extern uint32_t usTicks;

extern uint32_t usTicks;

extern uint32_t testNumber;

extern uint32_t hwTimerInterval;
///* Functions -----------------------------------------------------------------*/

#define BSPPRINTF_FLOAT(x) \
{ \
    int lhs = (int)x; \
    char c = ' '; \
    if(lhs < 0) \
    { \
        lhs *= -1; \
    } \
    int rhs = (int)(1000000.0*(x-(int)x)); \
    if(rhs < 0) \
    { \
        rhs *= -1; \
        c = '-'; \
    } \
    bspPrintf("Float value: %c%d.%06d\n", c, lhs, rhs); \
} \

float power(float x, int y);

#endif // GLOBALS_H_INCLUDED