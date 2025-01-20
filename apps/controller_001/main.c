/* Includes ------------------------------------------------------------------*/
//#include "main.h"
//#include "stm32f4xx_hal.h"
//#include "adc.h"
//#include "dma.h"
//#include "spi.h"
//#include "tim.h"
//#include "usart.h"
//#include "gpio.h"

#include <stdint.h>
#include <stdbool.h>
#include "bsp.h"
#include "main.h"
#include "os.h"

/* References ----------------------------------------------------------------*/

/* Prototypes ----------------------------------------------------------------*/
void SystemClock_Config(void);

/* Functions -----------------------------------------------------------------*/
int main(void)
{
	bspInit();

	//Init and run operating system.  This starts all tasks
	osInit();
	
	/* We should never get here as control is now taken by the scheduler */
	while (1)
	{
	}
}
