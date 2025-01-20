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

    bool initialized;
    
private:
    Led ledGate;
    Led ledRec;
    Led ledStatus;
    
    Button sw1Up;
    Button sw2Up;
    Button sw3Up;
    Button sw4Up;
    Button sw5Up;
    Button sw1Down;
    Button sw2Down;
    Button sw3Down;
    Button sw4Down;
    Button sw5Down;
    
    Windowed10BitKnob slider1;
    Windowed10BitKnob slider2;
    Windowed10BitKnob slider3;
    Windowed10BitKnob slider4;
    Windowed10BitKnob slider5;
    Windowed10BitKnob slider6;
    
    uint16_t gateLedTimeToLive = 0;
    
};

#endif
