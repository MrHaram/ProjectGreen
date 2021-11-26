#ifndef Actuator_H //Checks to see if the identifier has not been defined.
#define Actuator_H //Used to establish the "Actuator_H" identifier.

#include "mbed.h"
#include <string>

class Actuator
/*
Establishes the "Actuator" class that will be used as a template for the 
actuator components.
*/
{
protected:
    PinName pin; //An object of the PinName class used to store the pin used for the component.
    string name;
    bool mode;
    bool status;
    float target;
    float minTarget;
    float maxTarget;
/*
These are the protected elements within the Actuator Class. Sub classes are  
expected to inherit a copy these elements. The name, status and target  
attributes will be used to store their respective data types which will be used 
by the class method.
*/

public:
/*
These are the public elements within the Actuator Class. Sub classes are  
expected to inherit these elements. Mainly consists of "getters" and "setters" 
to manipuate the protected attributes.
*/
    Actuator(PinName pin_, string name_, bool mode_, bool status_, float target_, float minTarget_, float maxTarget);
    ~Actuator();
    //The Constructor and Destructor of the Actuator class.
        
    void setName(string newName);
    string getName();
    void setAutomation(bool newMode);
    bool getAutomation();
    void setStatus(bool newStatus);
    bool getStatus();
    void setTarget(float newTarget);
    float getTarget();
    float getMinTarget();
    float getMaxTarget();
};

#endif //Ends the ifndef directives