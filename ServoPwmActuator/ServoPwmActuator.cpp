#include "mbed.h"
#include "ServoPwmActuator.h"

ServoPwmActuator::ServoPwmActuator(PinName pin_, string name_, bool mode_, bool status_, float target_, float minTarget_, float maxTarget, float period_):Actuator(pin_, name_, mode_, status_, target_, minTarget_, maxTarget), period(period_) {}
ServoPwmActuator::~ServoPwmActuator() {}

void ServoPwmActuator::setStatus(bool newStatus)
{
    status = newStatus;
    PwmOut output(pin);
    if (status == false)
    {
        output.write(500/(period*1000.0));
    }
    /*
    When the status is set to false, it closes the servo. "500/(period*1000.0)" 
    Sets the angle to 0.
    */
    else
    {
        output.write(2500/(period*1000.0));
    }
    /*
    When the status is set to true, it closes the servo. "2500/(period*1000.0)" 
    Sets the angle to 180.
    */
}
void ServoPwmActuator::getPeriod()
{
    PwmOut output(pin);
    output.period(period/1000.0);
}