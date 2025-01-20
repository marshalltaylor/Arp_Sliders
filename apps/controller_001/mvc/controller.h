#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdint.h>

#include "uCModules.h"
#include "customPanelComponents.h"
//#include "flagMessaging.h"
#include "model.h"
#include "view.h"

class Controller : public Panel
{
public:
    Controller(void);
    void tick(int16_t deltaMs);
    void reloadSettings(void);
    void saveSettingsToFlash(void);
    void resetDefaultSettings(void);
    void gateLedOneShot(void);
    Led ledGate;
    bool checkFirstInput(void);
    void stopRecording(void);
    void clockGood(void);
    void clockBad(void);
    void clockLost(uint8_t clkNum);
    //Flags that emulate button functions
    MessagingFlag midiCtlRec;
    MessagingFlag midiCtlPlay;
    void startPlayingAbsolute(void);
    void startPlayingSynchronously(uint32_t clocks);
    void stopPlaying(void);

    bool initialized;
    
private:
    Led ledPlay;
    Led ledRec;
    Led ledStatus;
    
    Button buttonClear;
    Button buttonRest;
    Button buttonRec;
    Button buttonPlay;
    
    Selector knobQuantize;
    RangedSelector knobMode;
    
    uint16_t gateLedTimeToLive = 0;
    
    bool isThisActionARest;
};

#endif
