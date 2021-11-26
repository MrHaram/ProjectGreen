#ifndef ServoPwmActuator_H
#define ServoPwmActuator_H

#include "mbed.h"
#include "Actuator.h"

class ServoPwmActuator : public Actuator
{
    private:
    float period;
    //Stores the period at which the servo works at. In milliseconds.
    
    public:
    ServoPwmActuator(PinName pin_, string name_, bool mode_, bool status_, float target_, float minTarget, float maxTarget, float period_);
    ~ServoPwmActuator();
    
    void setStatus(bool newStatus);
    void getPeriod();
};

#endif