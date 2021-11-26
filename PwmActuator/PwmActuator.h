#ifndef PwmActuator_H
#define PwmActuator_H

#include "mbed.h"
#include "Actuator.h"
#include <string>

class PwmActuator : public Actuator
/*
Establishes the "PwmActuator" sub-class. This sub-class inherits from the "Actuator" 
class. Has the addition of the "intensity" float variable which will be used to 
store the component's intensity.
*/
{
protected:
    float intensity;
    float calibration;
    float minCalibration;
    float maxCalibration;

public:
    PwmActuator(PinName pin_, string name_, bool mode_, bool status_, float target_, float minTarget_, float maxTarget_, float calibration_, float minCalibration, float maxCalibration, float intensity_);
    ~PwmActuator();
    //The constructor destructor of the class.
    
    void setStatus(bool newStatus);
    void setCalibration(float newCalibration);
    float getCalibration();
    float getMinCalibration();
    float getMaxCalibration();
    void setIntensity(float newIntensity);
    float getIntensity();
};

#endif //Ends the ifndef directives