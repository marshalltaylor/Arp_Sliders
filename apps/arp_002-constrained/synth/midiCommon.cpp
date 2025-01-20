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




//**********************************************************************//
