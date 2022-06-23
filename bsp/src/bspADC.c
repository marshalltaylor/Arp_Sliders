/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/* Includes -- HAL -----------------------------------------------------------*/
#include "adc.h"

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* References ----------------------------------------------------------------*/
uint16_t adcValues[16];
int8_t adcBank = 0;
uint8_t conversionCounter;

/* Functions -----------------------------------------------------------------*/
void bspADCConvert(void)
{
    //the adcBank has been selected at the end of the last conversion cycle.
    conversionCounter = 0;
    if (HAL_ADC_Start_IT(&hadc1) != HAL_OK)
    {
        while(1);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1)
{
    /* Get the converted value of regular channel */
    if( conversionCounter < 4 )
    {
        //take bank into account and generate index into 16 array
        //example:  If adcBank == 00,
        //conversions would be a0, b0, which would be 4 apart on the connector,
        //{a0, a1, a2, a3, b0, b1, ...}
        //so conversion counter adds 4 and adcBank adds 1
        uint8_t index = (conversionCounter << 2) | adcBank;
        adcValues[index] = HAL_ADC_GetValue(hadc1);
        if( conversionCounter == 3)
        {
            //step adcBank, the extra adc channels can still be read.
            adcBank++;
            if(adcBank >= 4) adcBank = 0;
            //Set hardware
            bspIOPinWrite(ADC_SEL_0, adcBank & 0x01);
            bspIOPinWrite(ADC_SEL_1, (adcBank & 0x02) >> 1);
        }
        conversionCounter++;
    }
}