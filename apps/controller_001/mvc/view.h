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
    void printModel(void);
    
private:
    
};

#endif
