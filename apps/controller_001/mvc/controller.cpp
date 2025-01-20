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
    buttonClear.setHardware(new ArduinoDigitalIn( BUTTON_1 ), 1);
    add( &buttonClear );
    buttonRest.setHardware(new ArduinoDigitalIn( BUTTON_0 ), 1);
    add( &buttonRest );
    buttonRec.setHardware(new ArduinoDigitalIn( BUTTON_3 ), 1);
    add( &buttonRec );
    buttonPlay.setHardware(new ArduinoDigitalIn( BUTTON_2 ), 1);
    add( &buttonPlay );

    ledGate.setHardware(new ArduinoDigitalOut( LED_2 ), 1);
    add( &ledGate );
    ledPlay.setHardware(new ArduinoDigitalOut( LED_1 ), 1);
    add( &ledPlay );
    ledRec.setHardware(new ArduinoDigitalOut( LED_3 ), 1);
    add( &ledRec );
    ledStatus.setHardware(new ArduinoDigitalOut( LED_4 ), 1);
    add( &ledStatus );

    knobQuantize.setHardware(new ArduinoAnalogIn(0));
    knobQuantize.init(1024, 0, 5);
    add( &knobQuantize );
    knobMode.setHardware(new ArduinoAnalogIn(4));
    knobMode.init(1024, 0, 6);
    add( &knobMode );

    // Set default states
    ledGate.setState(LEDOFF);
    ledPlay.setState(LEDOFF);
    ledRec.setState(LEDOFF);
    ledStatus.setState(LEDOFF);

    //Setup SPI
    controlPanelSpi.bspPin = SPI_CS;
    controlPanelSpi.txData = spiTxBuf;
    controlPanelSpi.rxData = spiRxBuf;
    controlPanelSpi.len = 2;
    controlPanelSpi.pendingXfer = false;

    // Init data
    controlPanelSpi.rxData[0] = 0xFF;
    controlPanelSpi.rxData[1] = 0xFF;

    initialized = false;

    isThisActionARest = false;

    // Set default model data
    //   (keep this last in constructor)
}

void Controller::stopRecording(void)
{
    if(!model.recording) return;
    //It was recording.  Stop it
    if(player.seq->syncState == seqType::LENGTH_UNKNOWN)
    {
        //We now know how long the pattern is.
        if(model.beatKnown)
        {
            //quantize length to beat
            player.seq->length = qInputBeat.quantize(midiClock.ticks);
        }
        else
        {
            player.seq->length = midiClock.ticks;
        }
        localPrintf("Saving length: %d\n", player.seq->length);
        player.seq->syncState = seqType::LENGTH_SET;
    }
    ledRec.setState(LEDOFF);
    model.recording = false;
}

void Controller::startPlayingAbsolute(void)
{
    ledPlay.setState(LEDON);
    model.playing = true;
    player.start();
}

void Controller::startPlayingSynchronously(uint32_t clocks)
{
    ledPlay.setState(LEDON);
    model.playing = true;
    player.syncToClock(clocks);
    player.cont();
    arp.syncToClock(clocks);
}

void Controller::stopPlaying(void)
{
    ledPlay.setState(LEDOFF);
    model.playing = false;
    player.stop();
    stopRecording();
}

void Controller::tick(int16_t deltaMs)
{
    if (!initialized)
    {
        reloadSettings();
        initialized = true;
    }
    int32_t clockAtEntry = midiClock.ticks;
    
    bool redrawStatus = false;
    
    if( bspSPIRead(&controlPanelSpi) )
    {
        //device not in use by this data block
        memcpy(SPIButtonData, controlPanelSpi.rxData, sizeof(SPIButtonData));
    }
    freshenComponents(deltaMs);

    if(buttonClear.serviceRisingEdge())
    {
        isThisActionARest = false;
        if(buttonRest.getState())
        {
            //Using rest as fn button
            //Function is toggle input mute
            model.muteInput = !model.muteInput;
            if(model.muteInput)
            {
                outputMixer.flushOutputList();
            }
            redrawStatus = true;
        }
        else
        {
            //Choose to clear some things
            stopRecording();
            if(player.undo())
            {
                localPrintf("Restored backup\n");
                player.syncToClock(clockAtEntry);
            }
            else
            {
                player.clearSequence();
                localPrintf("Cleared\n");
                model.playing = false;
                ledPlay.setState(LEDOFF);
                if(buttonPlay.getState())
                {
                    //Using play as fn button
                    //Function is clear arp input
                    localPrintf("Arp control clear\n");
                }
            }
        }
    }
    if(buttonRest.serviceRisingEdge())
    {
        isThisActionARest = true;
        checkFirstInput();
        localPrintf("rest try\n");
    }
    if(buttonRest.serviceFallingEdge())
    {
        if (isThisActionARest)
        {
            localPrintf("rest\n");
            arp.rest();
        }
    }
    if(buttonRec.serviceRisingEdge() ||
       midiCtlRec.serviceRisingEdge())
    {
        isThisActionARest = false;
        if(buttonRest.getState())
        {
            //Using rest as fn button
            model.useAltSequence = !model.useAltSequence;
            if(!model.useAltSequence)
            {
                player.setSequence(&mainSequence);
            }
            else
            {
                player.setSequence(&altSequence);
            }
        }
        else
        {
            localPrintf("rec\n");
            if(!model.recording)
            {
                ledRec.setState(LEDON);
                //Save a backup of the sequence
                if(player.saveForUndo())
                {
                    localPrintf("Saving backup data\n");
                }
                model.recording = true;
                //Decide what to about the play state
                if(model.playing)
                {
                    //Cool, we're already playing.
                }
                else
                {
                    if(player.seq->syncState == seqType::LENGTH_SET)
                    {
                        //We're not playing, but we know the length.
                        //Jump in.
                        localPrintf("Starting mid-pattern\n");
                        ledPlay.setState(LEDON);
                        model.playing = true;
                        //TODO: Check that this is doing something useful
                        player.syncToClock(clockAtEntry);
                        player.cont();
                    }
                    else
                    {
                        //We're not playing, and there is no valid pattern
                        localPrintf("clear and wait\n");
                        player.clearSequence();
                        ledPlay.setState(LEDON);
                        model.playing = true;
                        player.start();
                    }
                }
            }
            else
            {
                //It was recording.  Stop it
                stopRecording();
            }
        }//'fn' rest else
    }
    if(buttonPlay.serviceRisingEdge() ||
       midiCtlPlay.serviceRisingEdge())
    {
        isThisActionARest = false;
        localPrintf("play ");
        if(!model.playing)
        {
            //Choose type of start
            if(buttonRest.getState())
            {
                localPrintf("Absolute start\n");
                startPlayingAbsolute();
            }
            else
            {
                localPrintf("Synchronous start\n");
                startPlayingSynchronously(clockAtEntry);
            }
        }
        else
        {
            localPrintf("off\n");
            stopPlaying();
        }
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

    //Status LED
    if(model.beatKnownChanged)
    {
        redrawStatus = true;
        if(model.beatKnown)
        {
            model.baseStatusLED = LEDON;
        }
        else
        {
            model.baseStatusLED = LEDOFF;
        }
        model.beatKnownChanged = false;
    }
    if(redrawStatus)
    {
        if(model.muteInput)
        {
            ledStatus.setState(LEDFLASHINGFAST);
        }
        else
        {
            ledStatus.setState(model.baseStatusLED);
        }
    }

    if(knobQuantize.serviceChanged())
    {
        int qPower = knobQuantize.getState();
        localPrintf("Q: %d\n", qPower);
        if(qPower != 0)
        {
            qPattern.setDivisorPower(qPower);
            qPattern.setEnable(true);
            qPatternHalf.setDivisorPower(qPower + 1);
            qPatternHalf.setEnable(true);
        }
        else
        {
            qPattern.setEnable(false);
            qPatternHalf.setEnable(false);
        }
    }

    if(knobMode.serviceChanged())
    {
        // use note lengths as-is:
        // TODO: control this some other way (cli?)
        player.setNoteLen(0); 
        
        uint16_t knobState = knobMode.getState();
        localPrintf("M: %d\n", knobState);
        if (knobState == 0)
        {
            //Disable arp
            arp.enableArp(false);
            localPrintf("ARP OFF\n");
        }
        else if (knobState == 1)
        {
            //enable arp
            localPrintf("ARP ON\n");
            arp.enableArp(true);
            arp.quantizeStepLength = true;
        }
        else
        {
            localPrintf("ARP ON, analog\n");
            arp.enableArp(true);
            arp.quantizeStepLength = false;
        }
        //int16_t noteLen;
        //if (knobState > 1)
        //{
        //    //Set 'mode', 0 =
        //    float scalar = knobMode.getStateRanged(1, 4);
        //    noteLen = (int)(scalar * PULSES_PER_QUARTER);
        //    if(noteLen < 1) noteLen = 1;
        //}
        //player.setNoteLen(noteLen);
        //localPrintf(" Note period Q: %d\n", noteLen);
    }
    //Start SPI transaction
    if(!bspSPIWrite(&controlPanelSpi))
    {
        localPrintf("SPI Fail\n");
    }
    //Check the clock counters
    if(model.clkActiveTimeoutCtr_Main >= 0)
    {
        //Check if this is the first note and no clock is assigned.
        if(model.clkActive == -1)
        {
            //New clock selection
            model.clkActive = 0;
        }
        model.clkActiveTimeoutCtr_Main = model.clkActiveTimeoutCtr_Main + deltaMs;
        if(model.clkActiveTimeoutCtr_Main >= 1000)
        {
            model.clkActiveTimeoutCtr_Main = -1;
            localPrintf("Main clock idle\n");
            clockLost(0);
        }
    }
    if(model.clkActiveTimeoutCtr_Aux >= 0)
    {
        //Check if this is the first note and no clock is assigned.
        if(model.clkActive == -1)
        {
            //New clock selection
            model.clkActive = 1;
        }
        model.clkActiveTimeoutCtr_Aux = model.clkActiveTimeoutCtr_Aux + deltaMs;
        if(model.clkActiveTimeoutCtr_Aux >= 1000)
        {
            model.clkActiveTimeoutCtr_Aux = -1;
            localPrintf("Aux clock idle\n");
            clockLost(1);
        }
    }
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

    clockBad();

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

bool Controller::checkFirstInput(void)
{
    bool retVal = false;
    if((player.seq->syncState == seqType::NO_ZERO_TIME) &&
        (model.recording))
    {
        if(model.beatKnown)
        {
            //The looper is syncronous to the clock source.
            //This event could be at 0/4, or 3/4, etc.
            //Whatever, now is the down beat of the pattern.
            midiClock.ticks = midiClock.ticks % PULSES_PER_QUARTER;
            if(midiClock.ticks > (PULSES_PER_QUARTER/2))
            {
                midiClock.ticks -= PULSES_PER_QUARTER;
            }
            localPrintf("Set clock, player sync, t=%d\n", midiClock.ticks);
            player.syncToClock(midiClock.ticks);
            //
            ////Sync to the beat
            //int nearestBeat = qInputBeat.quantize(midiClock.ticks);
            //int tSync = nearestBeat - midiClock.ticks;
            //localPrintf("Set clock t=%d\n", tSync);
            ////When the nearest beat is in the future, tSync is (-)
            ////TODO: can tSync be negative?
            //midiClock.setTickCount(tSync);
            //localPrintf("Sync player to time\n");
            //player.syncToClock(tSync);
        }
        else
        {
            //Don't worry about it
            //Now is zero
            midiClock.setTickCount(0);
            localPrintf("Set clock t=0\n");
        }
        player.seq->syncState = seqType::LENGTH_UNKNOWN;
        ledPlay.setState(LEDON);
        model.playing = true;
        player.cont();
        retVal = true;
    }
    return retVal;
}

void Controller::clockGood(void)
{
    model.beatKnown = true;
    model.beatKnownChanged = true;
    localPrintf("Beat division known from input clock\n");
}

void Controller::clockBad(void)
{
    model.beatKnown = false;
    model.beatKnownChanged = true;
    localPrintf("Beat division unknown!\n");
}

void Controller::clockLost(uint8_t clkNum)
{
    if(model.clkActive == clkNum)
    {
        //We lost the active clock
        model.clkActive = -1;
        clockBad();
    }
}
