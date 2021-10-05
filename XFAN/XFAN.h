#ifndef XFAN
#define XFAN

#include "Actuator.h"

class XFAN : public Actuator {
public:
    XFAN(PinName pin);
    ~XFAN();
private:
    int intensity;
};

#endif