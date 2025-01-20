/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
//#include <stdbool.h>
//#include <stdarg.h>
//#include <string.h>
//#include <stdio.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- modules -------------------------------------------------------*/
#include "sequence.h"
#include "midiTime.h"
#include "midiTimeQuantize.h"
#include "customMath.h"

/* References ----------------------------------------------------------------*/

// Connect directly to bsp.
#define localPrintf bspPrintf

QCalculator::QCalculator(void)
{
    enabled = false;
    setDivisorPower(4);
}

// quantize period = bar / 2^input
void QCalculator::setDivisorPower(uint8_t input)
{
    if(input < 6) //allow period to 1/32th range
    {
        barDivisor = power(2, input);
        quantizeLength = (4 * PULSES_PER_QUARTER) / barDivisor; //Do now to unload calc
        localPrintf("Quantize set to 1/%d note\n", barDivisor);
    }
}

void QCalculator::setQuantizePeriod(uint32_t ticks)
{
    if (ticks == 0) ticks++;
    quantizeLength = ticks; //Do now to unload calc
    localPrintf("Quantize set to %d ticks\n", ticks);
}

void QCalculator::setEnable(bool input)
{
    enabled = input;
}

int32_t QCalculator::quantize(int32_t midiClocks)
{
    if(!enabled) return midiClocks;
    
    //We must assume '0' is down-beat
    int32_t output;

    if (midiClocks >= 0)
    {
        midiClocks += (quantizeLength/2);
    }
    else
    {
        midiClocks -= (quantizeLength/2);
    }

    output = midiClocks / (int32_t)quantizeLength;
    output *= quantizeLength;

    return output;
}
