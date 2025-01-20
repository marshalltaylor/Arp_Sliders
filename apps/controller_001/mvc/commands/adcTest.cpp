/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- FreeRTOS system -----------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"

/* Includes -- FreeRTOS app --------------------------------------------------*/
#include "commands.h"

// Connect directly to bsp.
#define localPrintf bspPrintf

int adcProgram(int argc, char *argv[]);

commandInfo_t adcCmd = {
    "adc",
    "ADC Test",
    " usage:\n  adc scan",
    &adcProgram
};

int adcProgram(int argc, char *argv[])
{
    if(0 == strcmp((const char*)argv[0], "scan")
        || 1 ) //Force true
    {
        while(1)
        {
            localPrintf("-ADC values- {A0,B0,C0,D0},{A1, ...]\n");
            localPrintf("%03d, %03d, %03d, %03d\n", bspIOPinReadAnalog(MUX_A0), bspIOPinReadAnalog(MUX_A1), bspIOPinReadAnalog(2), bspIOPinReadAnalog(3));
            localPrintf("%03d, %03d, %03d, %03d\n", bspIOPinReadAnalog(4), bspIOPinReadAnalog(5), bspIOPinReadAnalog(6), bspIOPinReadAnalog(7));
            localPrintf("%03d, %03d, %03d, %03d\n", bspIOPinReadAnalog(8), bspIOPinReadAnalog(9), bspIOPinReadAnalog(10), bspIOPinReadAnalog(11));
            localPrintf("%03d, %03d, %03d, %03d\n", bspIOPinReadAnalog(12), bspIOPinReadAnalog(13), bspIOPinReadAnalog(14), bspIOPinReadAnalog(15));
            
            vTaskDelay( 350 );
        }
    }
    else
    {
        localPrintf("Needs args\n");
    }
    return 0;
}
