#ifndef MIDITIMEQUANTIZE_H
#define MIDITIMEQUANTIZE_H
#include <stdio.h>


//Unusre of if/how this should interact with other objects, so written as a stand alone calc

class QCalculator
{
public:
    QCalculator(void);
    void setDivisorPower(uint8_t input);
    void setEnable(bool input);
    int32_t quantize(int32_t midiClocks);
private:
    bool enabled;
    uint8_t barDivisor;
    uint32_t quantizeLength;
};

#endif
