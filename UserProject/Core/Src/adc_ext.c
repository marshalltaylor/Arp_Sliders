/* Includes ------------------------------------------------------------------*/
#include "adc.h"
//#include "adc_ext.h"

//#include "gpio.h"
uint16_t adcValues[10];
uint8_t conversionCounter;
void convertADC(void)
{
	conversionCounter = 0;
	if (HAL_ADC_Start_IT(&hadc1) != HAL_OK)
	{
		while(1);
	}	
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1)
{
  /* Get the converted value of regular channel */
  adcValues[conversionCounter] = HAL_ADC_GetValue(hadc1);
  if( conversionCounter < 3 )
  {
	  conversionCounter++;
  }
  
}