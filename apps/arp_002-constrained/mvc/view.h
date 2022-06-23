#ifndef VIEW_H
#define VIEW_H

#include <stdint.h>

#include "model.h"
#include "controller.h"

class View
{
public:
    View(void);
    void tick(void);
    
private:
    modelType::model_data_t * pModel;
    
};

#endif
