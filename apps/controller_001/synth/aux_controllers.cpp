#include "globals.h"
#include "aux_controllers.h"
#include "midiCommon.h"

#define localPrintf bspPrintf

#define UNUSED(x)

AuxController::AuxController(void)
{
    
}

#define CLEAR_REFACTORY_TICKS (24 * 2)
uint32_t clearBlockedUntil = 0;

void AuxController::input(MidiMessage * pMsg, uint8_t inPort)
{
    dPrintf("AuxController: input()\n");
    dPrintNoteStr(pMsg, inPort);
    dPrintf("\n");
    
    float value = (float)pMsg->data / 127.0;
    
    //Switch on message type
    switch (pMsg->controlMask)
    {
        case ControlChange: //main keyboard
        {
            dPrintf("-CC-");
            
            if (arp.quantizeStepLength)
            {
                if (value > 0.8)
                {
                    //32nds
                    arp.setQuantizeNotePeriod(3);
                }
                else if (value > 0.6)
                {
                    //16ths
                    arp.setQuantizeNotePeriod(6);
                }
                else if (value > 0.4)
                {
                    //8ths
                    arp.setQuantizeNotePeriod(12);
                }
                else if (value > 0.2)
                {
                    //quarters
                    arp.setQuantizeNotePeriod(24);
                }
            }
            else
            {
                arp.setQuantizeNotePeriod(32 - (value * 30));
            }
            break;
        }
        case PitchBend: //aux keyboard
        {
            dPrintf("-PB-");
            //localPrintf("pitch = %d\n", (int16_t)pMsg->data);
            //convert to float
            float bend = (float)((int16_t)pMsg->data)/8192.0;

            if (bend < -0.8)
            {
                if (pMsg->tick >= clearBlockedUntil)
                {
                    arp.clearPattern();
                    clearBlockedUntil = pMsg->tick + CLEAR_REFACTORY_TICKS;
                    localPrintf("Arp clear, blocked until %d\n", clearBlockedUntil);
                }
            }
            break;
        }
        default:
        if (debugEnabled)
        {
            dPrintf("AuxController: rejected input\n");
        }
        break;
    }
}
