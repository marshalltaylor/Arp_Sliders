#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdint.h>

#include "uCModules.h"
#include "customPanelComponents.h"
#include "model.h"
#include "view.h"

class Controller : public Panel
{
public:
    Controller(void);
    void tick(void);
    void setParam(int8_t value);
    
private:
    modelType::model_data_t * pModel;
    
    Led led1;
    
    Button button1;
    
    RotoryEncoder enc1;
};

#endif
