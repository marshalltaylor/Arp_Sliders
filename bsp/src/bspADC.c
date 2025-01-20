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
    if( conversionCounter < 6 )
    {
		//Simple conversion saving
        adcValues[conversionCounter] = HAL_ADC_GetValue(hadc1);
        conversionCounter++;
    }
}