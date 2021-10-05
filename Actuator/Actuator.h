#ifndef Actuator //Checks to see if the identifier has not been defined
#define Actuator //Used to establish the "Actuator" identifier

#include "mbed.h"
#include <string> //Library needed to create string variables

/*Establishes the "Actuator" class that will be used as a template for the
actuator components*/
class Actuator
{
public:
    bool returnStatus(void); /*Will be used to update the User if the actuators
    are on or off*/
private:
    std::string name; //Used to store the name of the component
    bool status; //Used to store the status of the component
    float target; //Used to store the target values
};
#endif //Ends the ifndef directives