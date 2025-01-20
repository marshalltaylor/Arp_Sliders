/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
//#include <stdbool.h>
//#include <stdarg.h>
//#include <string.h>
//#include <stdio.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- modules -------------------------------------------------------*/
#include "logging.h"
//#include "MidiUtils.h"
#include "midiCommon.h"

/* References ----------------------------------------------------------------*/
//#define PRINT_DEBUG

//#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
#define localPrintf dummy.printf
extern Logging dummy;

#else
// Connect directly to bsp.
#define localPrintf bspPrintf
#endif

void printItfMessage(MidiMessage * pMsg)
{
    localPrintf("--MidiMessage--\n");
    localPrintf(" ctl:  %0x02X\n", pMsg->controlMask);
    localPrintf(" chan: % 4d\n", pMsg->channel);
    localPrintf(" data: % 4d\n", pMsg->value);
    localPrintf(" data: % 4d\n", pMsg->data);
    localPrintf(" tick: % 10d\n", pMsg->tick);
}

//**********************************************************************//
