/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- FreeRTOS system -----------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "os.h"

/* Includes -- FreeRTOS app --------------------------------------------------*/
#include "globals.h"
#include "taskMidi.h"
//#include "sequence.h"
//#include "synth.h"

/* Includes -- modules -------------------------------------------------------*/
#include "logging.h"

/* References ----------------------------------------------------------------*/
//#define USE_LOGGING
#ifdef USE_LOGGING
// Create logging object and macro for local printf
#define localPrintf logTaskLog.printf
Logging logTaskLog;

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif
/* References ----------------------------------------------------------------*/

#define MIDI_CTRL_TX_LOOP_TARGET 10

static uint32_t loopCtr = 0;

//Collection of tasks for midi

// 1. sequence player task

extern "C" void taskMidiWork(void * argument)
{
    
    //Choosing to use while(1) rather than single run tasks, in order to debug load better.
    while(1)
    {
        //Wait for condition to start process (Task is suspended)
        
        //Get delta sub
        
        
        //Send subs to sequence
        
        
        vTaskSuspend(NULL);
    }

}

extern "C" void taskMidiRx(void * argument)
{
    //initSynth();
    
    while(1)
    {
        //Check the BSP level serial.available(), if so, crank on the midi until serial is empty
        while( mainMidiInterface.available() > 0 )
        {
            if(xSemaphoreTake( xSerCtrl, 1 )) //try for 1ms
            {
                vTaskSuspendAll();
                MIDI.read();
                xSemaphoreGive( xSerCtrl );
                xTaskResumeAll();
            }
            else
            {
                bspPrintf("taskMidi miss\n");
            }
        }
        //Check the BSP level serial.available(), if so, crank on the midi until serial is empty
        while( ctrlMidiInterface.available() > 0 )
        {
            if(xSemaphoreTake( xSerCtrl, 1 )) //try for 1ms
            {
                vTaskSuspendAll();
                CtrlMIDI.read();
                xSemaphoreGive( xSerCtrl );
                xTaskResumeAll();
            }
            else
            {
                bspPrintf("taskMidi miss\n");
            }
        }
        loopCtr++;
        if (loopCtr >= MIDI_CTRL_TX_LOOP_TARGET)
        {
            loopCtr = 0;
            controllers.tick();
        }
        vTaskDelay( 1 );
    }
}