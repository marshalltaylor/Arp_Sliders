#include <stdint.h>
#include <math.h>

#include "bsp.h"

#include "model.h"
#include "view.h"
#include "controller.h"
#include "globals.h"
#include "customHardwareInterfaces.h"

#define USE_LOGGING
#ifdef USE_LOGGING
#include "logging.h"
// Create logging object and macro for local printf
extern Logging mvcDebug;
//#define localPrintf consoleDebug.printf

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif

//Send output to two devices
static void localPrintf(const char* fmt, ...)
{
    //Setup
    va_list args;
    va_start(args, fmt);
    //Send to serial
    if(mvcDebug.pVPrintf)
    {
        mvcDebug.pVPrintf(fmt, args);
    }
    va_end(args);
}

Controller::Controller(void)
{
    // Build panel objects
    button1.setHardware(new SpiDigitalIn( 0, 0 ), 1);
    add( &button1 );

    led1.setHardware(new SpiDigitalOut( 0, 1 ), 0);
    add( &led1 );

    enc1.setHardware(new bspEncoderIn(0));
    add( &enc1 );

    // Set default states
    led1.setState(LEDOFF);

    //Setup SPI
    controlPanelSpi.bspPin = SPI_CS;
    controlPanelSpi.txData = spiTxBuf;
    controlPanelSpi.rxData = spiRxBuf;
    controlPanelSpi.len = 2;
    controlPanelSpi.pendingXfer = false;

    // Init data
    controlPanelSpi.rxData[0] = 0xFF;
    controlPanelSpi.rxData[1] = 0xFF;

    // Set default model data
    //   (keep this last in constructor)
    pModel = &model;
    pModel->param = -1;
}

void Controller::tick(void)
{
    if( bspSPIRead(&controlPanelSpi) )
    {
        //device not in use by this data block
        memcpy(SPIButtonData, controlPanelSpi.rxData, sizeof(SPIButtonData));
    }
    freshenComponents( 50 );

    if(button1.serviceRisingEdge())
    {
        localPrintf("Button\n");
        led1.toggle();
    }

    //Start SPI transaction
    if(!bspSPIWrite(&controlPanelSpi))
    {
        localPrintf("SPI Fail\n");
    }
}

void Controller::setParam(int8_t value)
{
    pModel->param = value;
}

//TODO: Turn this into some default test for the encoder
//void Controller::updateMainKnob(int32_t delta)
//{
//    if(delta != 0)
//    {
//        bspIOEncWrite(0, bspIOEncRead(0) + delta);
//    }
//}
//
//int32_t Controller::getMainKnob(void)
//{
//    int32_t retVal = bspIOEncRead(0);
//    bspIOEncWrite(0, 0);
//    return retVal;
//}
