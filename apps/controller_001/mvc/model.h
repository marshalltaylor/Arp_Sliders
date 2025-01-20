#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>

#include "uCModules.h"

// Example complex data member with helper functions

//struct settingDataItem_t
//{
//    uint8_t level;
//    const char * name;
//};
//
//class settingsTree
//{
//public:
//    settingsTree(void);
//    void select(void);
// (...)


namespace modelType
{
    //example scoped type (use: 'modelType::V_POSITION')
    
    //typedef enum soft_encoder_options_t
    //{
    //    CURSOR_1 = 0,
    //    CURSOR_2,
    //    V_POSITION,
    //    V_SCALE,
    //    STORAGE_BASE,
    //    NUMBER_OPTIONS
    //} soft_encoder_options_t;
    
    typedef struct model_data_t
    {
        //Settings
        uint8_t outputChannel;
        bool clockThru;
        bool notesOnCtrlMidi;
        
        //Internal variables
        //settingsTree tree;
        bool playing;
        bool recording;
        bool beatKnown;
        bool beatKnownChanged;
        uint8_t modeOption; //TODO: add elsewhere
        int16_t modeValue;
        int noteLen;
        int32_t clkActiveTimeoutCtr_Main;
        int32_t clkActiveTimeoutCtr_Aux;
        int8_t clkActive; // -1 for none
        ledState_t baseStatusLED;
        bool muteInput;
        bool useAltSequence;
        
    } model_data_t;
    
    
};

extern modelType::model_data_t model;

#endif
