#include "mbed.h"
#include "Actuator.h"
//Includes the "Actuator.h" header file to gain access to the declared attributes and methods
#include <string>

Actuator::Actuator(PinName pin_, string name_, bool mode_, bool status_, float target_, float minTarget_, float maxTarget_):pin(pin_), name(name_), mode(mode_), status(status_), target(target_), minTarget(minTarget_), maxTarget(maxTarget_) {}
Actuator::~Actuator() {}
/*
The Actuator Contructor paired with the Destructor. The Constructor has default 
inputs which it then assigns to the attributes. It also initialises 
*/

bool automation = true;

void Actuator::setName(string newName)
{
    name = newName;
}
string Actuator::getName()
{
    return name;
}
void Actuator::setStatus(bool newStatus)
{
    status = newStatus;
    DigitalOut output(pin);
    output.write(status);
}
/*
Used to both change the "status" varialble, and outputing data on the 
pre-assigned pin
*/
bool Actuator::getStatus(void)
{
    return status;
}
void Actuator::setAutomation(bool newMode)
{
    mode = newMode;
}
bool Actuator::getAutomation(void)
{
    return mode;
}
//Will be used to allow manual control over the system if need be.

void Actuator::setTarget(float newTarget)
{
    target = newTarget;
}
float Actuator::getTarget()
{
    return target;
}
float Actuator::getMinTarget()
{
    return minTarget;
}
float Actuator::getMaxTarget()
{
    return maxTarget;
}
/*
Will be used to ensure that the user does not set the target value beyond what
the system can reliably measure.
*/