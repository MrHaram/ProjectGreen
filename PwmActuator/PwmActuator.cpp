#include "mbed.h"
#include "PwmActuator.h"
#include <string>

PwmActuator::PwmActuator(PinName pin_, string name_, bool mode_, bool status_, float target_, float minTarget_, float maxTarget_, float calibration_, float minCalibration_, float maxCalibration_, float intensity_):Actuator(pin_, name_, mode_, status_, target_, minTarget_, maxTarget_), calibration(calibration_), minCalibration(minCalibration_), maxCalibration(maxCalibration_), intensity(intensity_) {}
PwmActuator::~PwmActuator() {}
/*
This is the PwmActuator sub-class contructor and destructor. Because this is a 
sub-class, it is required to also initialise the Actuataor's constructor.
*/

void PwmActuator::setStatus(bool newStatus)
{
    status = newStatus;
    PwmOut output(pin);
    if (status == false)
    {
        output.write(0);
    }
    else
    {
        output.write(intensity);
    }
}
/*
This method is a polymorphic version of the Actuator's "setStatus" method. It 
works by first saving the status variable, then displaying the staus to the 
pre-assigned LED. It then create a "PwmOut" object called "outut". 
This is then used to output the intensity to the pre-assigned pin.
*/
void PwmActuator::setCalibration(float newCalibration)
{
    calibration = newCalibration;
}
/*
Will allow control over the rate of increase of the intensity of the actuators. 
Works proportionally based on the difference between the input data, and the 
target.
*/
float PwmActuator::getCalibration()
{
    return calibration;
}
float PwmActuator::getMinCalibration()
{
    return minCalibration;
}
float PwmActuator::getMaxCalibration()
{
    return maxCalibration;
}
/*
Will be used to ensure that the user does not set the calibration value beyond 
what the system can reliably measure.
*/
void PwmActuator::setIntensity(float newIntensity)
{
    intensity = newIntensity;
}
float PwmActuator::getIntensity()
{
    return intensity;
}