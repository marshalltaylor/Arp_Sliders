#include <stdint.h>
#include <math.h>

#include "bsp.h"

#include "model.h"
#include "view.h"
#include "controller.h"
#include "globals.h"
#include "customHardwareInterfaces.h"

//#define USE_LOGGING
#ifdef USE_LOGGING
#include "logging.h"
// Create logging object and macro for local printf
extern Logging mvcDebug;
//#define localPrintf consoleDebug.printf

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

#else
// Connect directly to bsp.
#define localPrintf bspPrintf

#endif

Controller::Controller(void)
{
    // Build panel objects

    ledGate.setHardware(new ArduinoDigitalOut( DEBUG_0 ), 1);
    add( &ledGate );

    //Example from looper:
    //knobQuantize.setHardware(new ArduinoAnalogIn(0));
    //knobQuantize.init(1024, 0, 5);
    //add( &knobQuantize );
    //knobMode.setHardware(new ArduinoAnalogIn(4));
    //knobMode.init(1024, 0, 6);
    //add( &knobMode );

    sw1Up.setHardware(new ArduinoDigitalIn( SW_HIGH_POS_0 ), 1);
    sw2Up.setHardware(new ArduinoDigitalIn( SW_HIGH_POS_1 ), 1);
    sw3Up.setHardware(new ArduinoDigitalIn( SW_HIGH_POS_2 ), 1);
    sw4Up.setHardware(new ArduinoDigitalIn( SW_HIGH_POS_3 ), 1);
    sw5Up.setHardware(new ArduinoDigitalIn( SW_HIGH_POS_4 ), 1);
    sw1Down.setHardware(new ArduinoDigitalIn( SW_LOW_POS_0 ), 1);
    sw2Down.setHardware(new ArduinoDigitalIn( SW_LOW_POS_1 ), 1);
    sw3Down.setHardware(new ArduinoDigitalIn( SW_LOW_POS_2 ), 1);
    sw4Down.setHardware(new ArduinoDigitalIn( SW_LOW_POS_3 ), 1);
    sw5Down.setHardware(new ArduinoDigitalIn( SW_LOW_POS_4 ), 1);
    add( &sw1Down );
    add( &sw2Down );
    add( &sw3Down );
    add( &sw4Down );
    add( &sw5Down );
    add( &sw1Up );
    add( &sw2Up );
    add( &sw3Up );
    add( &sw4Up );
    add( &sw5Up );
    
    slider1.setHardware(new ArduinoAnalogIn(0));
    add( &slider1 );
    slider1.setLowerKnobVal(10);
    slider1.setUpperKnobVal(1014);
    slider1.setLowerIntVal(0);
    slider1.setUpperIntVal(127);
    slider1.setSamplesAveraged(10);

    slider2.setHardware(new ArduinoAnalogIn(1));
    add( &slider2 );
    slider2.setLowerKnobVal(10);
    slider2.setUpperKnobVal(1014);
    slider2.setLowerIntVal(0);
    slider2.setUpperIntVal(127);
    slider2.setSamplesAveraged(10);

    slider3.setHardware(new ArduinoAnalogIn(2));
    add( &slider3 );
    slider3.setLowerKnobVal(10);
    slider3.setUpperKnobVal(1014);
    slider3.setLowerIntVal(0);
    slider3.setUpperIntVal(127);
    slider3.setSamplesAveraged(10);

    slider4.setHardware(new ArduinoAnalogIn(3));
    add( &slider4 );
    slider4.setLowerKnobVal(10);
    slider4.setUpperKnobVal(1014);
    slider4.setLowerIntVal(0);
    slider4.setUpperIntVal(127);
    slider4.setSamplesAveraged(10);

    slider5.setHardware(new ArduinoAnalogIn(4));
    add( &slider5 );
    slider5.setLowerKnobVal(10);
    slider5.setUpperKnobVal(1014);
    slider5.setLowerIntVal(1);
    slider5.setUpperIntVal(127);
    slider5.setSamplesAveraged(10);

    slider6.setHardware(new ArduinoAnalogIn(5));
    add( &slider6 );
    slider6.setLowerKnobVal(10);
    slider6.setUpperKnobVal(1014);
    slider6.setLowerIntVal(4);
    slider6.setUpperIntVal(127);
    slider6.setSamplesAveraged(10);

    // Set default states
    ledGate.setState(LEDOFF);

    ////Setup SPI
    //controlPanelSpi.bspPin = SPI_CS;
    //controlPanelSpi.txData = spiTxBuf;
    //controlPanelSpi.rxData = spiRxBuf;
    //controlPanelSpi.len = 2;
    //controlPanelSpi.pendingXfer = false;
    //
    //// Init data
    //controlPanelSpi.rxData[0] = 0xFF;
    //controlPanelSpi.rxData[1] = 0xFF;

    initialized = false;

    // Set default model data
    //   (keep this last in constructor)
}

void Controller::tick(int16_t deltaMs)
{
    if (!initialized)
    {
        reloadSettings();

        oled.begin();    // Initialize the OLED
        oled.flipVertical(true);
        oled.flipHorizontal(true);
        oled.clear(ALL); // Clear the display's internal memory
        oled.display();  // Display what's in the buffer (splashscreen)
        //oled.enableDirectAccess();

        initialized = true;
    }
    
    
    //if( bspSPIRead(&controlPanelSpi) )
    //{
    //    //device not in use by this data block
    //    memcpy(SPIButtonData, controlPanelSpi.rxData, sizeof(SPIButtonData));
    //}
    freshenComponents(deltaMs);


    static MidiMessage msg = {
        .channel = 14,
        .tick = 0,
    };

    // Switch 1
    if(sw1Up.serviceRisingEdge())
    {
        msg.controlMask = NoteOn;
        msg.value = 0;
        msg.data = 64;
        merger.input(&msg, 2);
    }
    if(sw1Up.serviceFallingEdge())
    {
        msg.controlMask = NoteOff;
        msg.value = 0;
        msg.data = 0;
        merger.input(&msg, 2);
    }
    if(sw1Down.serviceRisingEdge())
    {
        msg.controlMask = NoteOn;
        msg.value = 1;
        msg.data = 64;
        merger.input(&msg, 2);
    }
    if(sw1Down.serviceFallingEdge())
    {
        msg.controlMask = NoteOff;
        msg.value = 1;
        msg.data = 0;
        merger.input(&msg, 2);
    }

    // Switch 2
    if(sw2Up.serviceRisingEdge())
    {
        msg.controlMask = NoteOn;
        msg.value = 2;
        msg.data = 64;
        merger.input(&msg, 2);
    }
    if(sw2Up.serviceFallingEdge())
    {
        msg.controlMask = NoteOff;
        msg.value = 2;
        msg.data = 0;
        merger.input(&msg, 2);
    }
    if(sw2Down.serviceRisingEdge())
    {
        msg.controlMask = NoteOn;
        msg.value = 3;
        msg.data = 64;
        merger.input(&msg, 2);
    }
    if(sw2Down.serviceFallingEdge())
    {
        msg.controlMask = NoteOff;
        msg.value = 3;
        msg.data = 0;
        merger.input(&msg, 2);
    }

    // Switch 3
    if(sw3Up.serviceRisingEdge())
    {
        msg.controlMask = NoteOn;
        msg.value = 4;
        msg.data = 64;
        merger.input(&msg, 2);
    }
    if(sw3Up.serviceFallingEdge())
    {
        msg.controlMask = NoteOff;
        msg.value = 4;
        msg.data = 0;
        merger.input(&msg, 2);
    }
    if(sw3Down.serviceRisingEdge())
    {
        msg.controlMask = NoteOn;
        msg.value = 5;
        msg.data = 64;
        merger.input(&msg, 2);
    }
    if(sw3Down.serviceFallingEdge())
    {
        msg.controlMask = NoteOff;
        msg.value = 5;
        msg.data = 0;
        merger.input(&msg, 2);
    }

    // Switch 4
    if(sw4Up.serviceRisingEdge())
    {
        msg.controlMask = NoteOn;
        msg.value = 6;
        msg.data = 64;
        merger.input(&msg, 2);
    }
    if(sw4Up.serviceFallingEdge())
    {
        msg.controlMask = NoteOff;
        msg.value = 6;
        msg.data = 0;
        merger.input(&msg, 2);
    }
    if(sw4Down.serviceRisingEdge())
    {
        msg.controlMask = NoteOn;
        msg.value = 7;
        msg.data = 64;
        merger.input(&msg, 2);
    }
    if(sw4Down.serviceFallingEdge())
    {
        msg.controlMask = NoteOff;
        msg.value = 7;
        msg.data = 0;
        merger.input(&msg, 2);
    }

    // Switch 5
    if(sw5Up.serviceRisingEdge())
    {
        msg.controlMask = NoteOn;
        msg.value = 8;
        msg.data = 64;
        merger.input(&msg, 2);
    }
    if(sw5Up.serviceFallingEdge())
    {
        msg.controlMask = NoteOff;
        msg.value = 8;
        msg.data = 0;
        merger.input(&msg, 2);
    }
    if(sw5Down.serviceRisingEdge())
    {
        msg.controlMask = NoteOn;
        msg.value = 9;
        msg.data = 64;
        merger.input(&msg, 2);
    }
    if(sw5Down.serviceFallingEdge())
    {
        msg.controlMask = NoteOff;
        msg.value = 9;
        msg.data = 0;
        merger.input(&msg, 2);
    }


    if(slider1.serviceChanged())
    {
        localPrintf("Slider 1: %d\n", slider1.getAsInt16());
        controllers.setValue(0, slider1.getAsInt16());
    }
    if(slider2.serviceChanged())
    {
        controllers.setValue(1, slider2.getAsInt16());
    }
    if(slider3.serviceChanged())
    {
        controllers.setValue(2, slider3.getAsInt16());
    }
    if(slider4.serviceChanged())
    {
        controllers.setValue(3, slider4.getAsInt16());
    }
    if(slider5.serviceChanged())
    {
        controllers.setValue(4, slider5.getAsInt16());
    }
    if(slider6.serviceChanged())
    {
        controllers.setValue(5, slider6.getAsInt16());
    }



    //Count for gate LED
    if(gateLedTimeToLive > 0)
    {
        gateLedTimeToLive--;
        if(gateLedTimeToLive == 0)
        {
            ledGate.setState(LEDOFF);
        }
    }

    ////Status LED
    //if(model.beatKnownChanged)
    //{
    //    redrawStatus = true;
    //    if(model.beatKnown)
    //    {
    //        model.baseStatusLED = LEDON;
    //    }
    //    else
    //    {
    //        model.baseStatusLED = LEDOFF;
    //    }
    //    model.beatKnownChanged = false;
    //}
    //if(redrawStatus)
    //{
    //    if(model.muteInput)
    //    {
    //        ledStatus.setState(LEDFLASHINGFAST);
    //    }
    //    else
    //    {
    //        ledStatus.setState(model.baseStatusLED);
    //    }
    //}


    ////Start SPI transaction
    //if(!bspSPIWrite(&controlPanelSpi))
    //{
    //    localPrintf("SPI Fail\n");
    //}

}

void Controller::gateLedOneShot(void)
{
    gateLedTimeToLive = 3;
    ledGate.setState(LEDON);
}

void Controller::reloadSettings(void)
{
    localPrintf("Loading from flash\n");
    bspFlashRead((uint8_t*)&model, sizeof(model));

}

void Controller::saveSettingsToFlash(void)
{
    localPrintf("Saving to flash...\n");
    bspFlashWrite((uint8_t*)&model, sizeof(model));
    localPrintf(" Done.\n");
}

void Controller::resetDefaultSettings(void)
{
    // Set default model data
    // 1: proto-8/yamaha
    // 2: bass machine
    // 3: macchiato
    // 13: pro-one
    model.outputChannel = 13;
    model.clockThru = true;
    model.notesOnCtrlMidi = false;
    
    model.playing = false;
    model.recording = false;
    
    model.beatKnown = false;
    model.beatKnownChanged = false;
    
    model.clkActiveTimeoutCtr_Main = -1;
    model.clkActiveTimeoutCtr_Aux = -1;
    model.clkActive = -1;
    
    model.muteInput = false;
    model.baseStatusLED = LEDOFF;
    
    model.useAltSequence = false;
    
    saveSettingsToFlash();
    reloadSettings();
}

