#include "bsp.h"
#include "commands.h"

// Connect directly to bsp.
#define localPrintf bspPrintf

extern spiDevice_t controlPanelSpi;

int spiProgram(int argc, char *argv[]);

commandInfo_t spiCmd = {
    "spi",
    "Spi Test",
    " usage:\n  spi tx 4F12\n  spi test\n  spi scan\n  spi sniff",
    &spiProgram
};

int spiProgram(int argc, char *argv[])
{
    if(0 == strcmp((const char*)argv[0], "tx"))
    {
        int val = strtol(argv[1], NULL, 16);
        localPrintf("value: %d, 0x%X\n", val, val);
        controlPanelSpi.txData[0] = (uint8_t)val;
        controlPanelSpi.txData[1] = (uint8_t)val;
        bspSPIWriteWait(&controlPanelSpi);
        localPrintf("Rx:");
        for(int i = 0; i < 4; i++)
        {
            localPrintf("%X", controlPanelSpi.rxData[i]);
        }
        localPrintf("\n");
    }
    else if(0 == strcmp((const char*)argv[0], "test"))
    {
        for(int i = 0; i < 256; i++)
        {
            controlPanelSpi.txData[0] = (uint8_t)i;
            controlPanelSpi.txData[1] = (uint8_t)i;
            if(!bspSPIWrite(&controlPanelSpi))
            {
                localPrintf("Fail\n");
            }
            uint32_t now = millis();
            while(millis() < now + 300){};
        }
    }
    else if(0 == strcmp((const char*)argv[0], "scan"))
    {
        uint8_t data[2];
        while(1)
        {
            controlPanelSpi.txData[0] = 0x12;
            controlPanelSpi.txData[1] = 0x34;
            bspSPIWriteWait(&controlPanelSpi);
            
            if((controlPanelSpi.rxData[0] != data[0])||(controlPanelSpi.rxData[1] != data[1]))
            {
                data[0] = controlPanelSpi.rxData[0];
                data[1] = controlPanelSpi.rxData[1];
                localPrintf("Rx:");
                for(int i = 0; i < 2; i++)
                {
                    localPrintf("%X", controlPanelSpi.rxData[i]);
                }
                localPrintf("\n");
            }
        }
    }
    else if(0 == strcmp((const char*)argv[0], "sniff"))
    {
        uint8_t sz = controlPanelSpi.len;
        uint8_t txData[sz];
        uint8_t rxData[sz];
        while(1)
        {
            bspSPIWriteWait(&controlPanelSpi);
            bool isMatch = true;
            for(int i = 0; i < sz; i++)
            {
                if(controlPanelSpi.rxData[i] != rxData[i])
                {
                    isMatch = false;
                    rxData[i] = controlPanelSpi.rxData[i];
                }
                else if(controlPanelSpi.txData[i] != txData[i])
                {
                    isMatch = false;
                    txData[i] = controlPanelSpi.txData[i];
                }
            }
            if(!isMatch)
            {
                localPrintf("Tx:");
                for(int i = 0; i < sz; i++)
                {
                    localPrintf("%X", txData[i]);
                }
                localPrintf("\n");
                localPrintf("Rx:");
                for(int i = 0; i < sz; i++)
                {
                    localPrintf("%X", rxData[i]);
                }
                localPrintf("\n");
                
            }
            //vTaskDelay( 350 );
        }
    }
    else
    {
        localPrintf("Needs args\n");
    }
    return 0;
}
