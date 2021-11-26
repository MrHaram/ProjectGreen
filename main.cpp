#include "mbed.h" //Library used to control the mbed.
#include "C12832.h" //Library used to control the LCD.
#include "DHT11.h" //Library used to control the Themometer and Humidity Sensor.
#include "Adafruit_SGP30.h" //Library used to control the Gas Sensor.
#include "PwmActuator.h" //Custom library used to control the Humidifier, Fan and LEDs.
#include "ServoPwmActuator.h" //Custom library used to control the Water and Gas Valves.
#include <string> //Libraries needed to add functionality to make and manipulate string variables.
//#include <algorithm> //Adds extra arithmetic capabilities.

C12832 lcd(p5, p7, p6, p8, p11); //Calls LCD library and creates a "lcd" object.

//Pins allocated for the joycon
DigitalIn joyRight(p16);
DigitalIn joyLeft(p13);
DigitalIn joyClick(p14);
DigitalIn joyUp(p15);
DigitalIn joyDown(p12);

DHT11 dht(p17); //Calls "DHT11" library and assigns pins and creates a "dht" object.
AnalogIn moistureSensor(p19); //Calls "AnalogIn" library and creates a "moistureSensor" object.
Adafruit_SGP30 gasSensor(p9, p10); //Calls "Adafruit_SGP30" library and creates a "gasSensor" object.
AnalogIn ldr(p18); //Calls "AnalogIn" library and creates a "ldr" object.

Actuator humidifier(LED1, "Humidifier", true, false, 85, 20, 90);
//(PinName pin_, string name_, bool mode_, bool status_, float target_, float minTarget_, float maxTarget_)
PwmActuator fan(p24, "Fan", true, false, 21, 0, 50, 0.20, 0.01, 1.01, 0);
//(PinName pin_, string name_, bool mode_, bool status_, float target_, float minTarget_, float maxTarget_, float calibration_, float minCalibration_, float maxCalibration_, float intensity_)
ServoPwmActuator waterValve(p21, "Water Valve", true, false, 0.4, 0, 0.95, 20);
//(PinName pin_, string name_, bool mode_, bool status_, float target_, float minTarget_, float maxTarget_, float period_)
ServoPwmActuator gasValve(p22, "Gas Valve", true, false, 500, 400, 60000, 20);
//(PinName pin_, string name_, bool mode_, bool status_, float target_, float minTarget_, float maxTarget_, float period_)
PwmActuator led(p23, "Light", true, false, 0.8, 0, 1, 1.25, 0.01, 100, 0);
//(PinName pin_, string name_, bool mode_, bool status_, float target_, float minTarget_, float maxTarget_, float calibration_, float minCalibration_, float maxCalibration_, float intensity_)

//Menu variables.
int menuCounter;
//Used to keep track of what page the user is on, in the menu.

//DHT and Humidifier variables.
int dhtError, humidifierMode = humidifier.getAutomation(), newHumidifierMode = humidifier.getAutomation(), humidifierStatus = humidifier.getStatus(), newHumidifierStatus = humidifier.getStatus();
float humidityData, humidityTarget = humidifier.getTarget(), minHumidityTarget = humidifier.getMinTarget(), maxHumidityTarget = humidifier.getMaxTarget(), newHumidityTarget = humidifier.getTarget();
string humidifierName = humidifier.getName();

//Fan Variables.
int fanMode = fan.getAutomation(), newFanMode = fan.getAutomation(), fanStatus = fan.getStatus(), newFanStatus = fan.getStatus();
float temperatureData, temperatureTarget = fan.getTarget(), minTemperatureTarget = fan.getMinTarget(), maxTemperatureTarget = fan.getMaxTarget(), newTemperatureTarget = fan.getTarget(), fanCalibration = fan.getCalibration(), minFanCalibration = fan.getMinCalibration(), maxFanCalibration = fan.getMaxCalibration(), newFanCalibration = fan.getCalibration(), fanIntensity = fan.getIntensity();
string fanName = fan.getName();

//Moisture Sensor and Water Valve.
int waterValveMode = waterValve.getAutomation(), newWaterValveMode = waterValve.getAutomation(), waterValveStatus = waterValve.getStatus(), newWaterValveStatus = waterValve.getStatus();
float moistureData, moistureTarget = waterValve.getTarget(), minMoistureTarget = waterValve.getMinTarget(), maxMoistureTarget = waterValve.getMaxTarget(), newMoistureTarget = waterValve.getTarget();
string waterValveName = waterValve.getName();

//Gas Sensor and Valve.
int gasError1, gasError2, gasValveMode = gasValve.getAutomation(), newGasValveMode = gasValve.getAutomation(), gasValveStatus = gasValve.getStatus(), newGasValveStatus = gasValve.getStatus();
uint16_t gasData;
float gasValveData, gasTarget = gasValve.getTarget(), minGasTarget = gasValve.getMinTarget(), maxGasTarget = gasValve.getMaxTarget(), newGasTarget = gasValve.getTarget();
string gasValveName = gasValve.getName();

//Light Sensor and LED.
int lightCounter, ledMode = led.getAutomation(), newLedMode = led.getAutomation(), ledStatus = led.getStatus(), newLedStatus = led.getStatus();
float lightData, lightAverage, initialLightData, lightTarget = led.getTarget(), minLightTarget = led.getMinTarget(), maxLightTarget = led.getMaxTarget(), newLightTarget = led.getTarget(), ledCalibration = led.getCalibration(), minLedCalibration = led.getMinCalibration(), maxLedCalibration = led.getMaxCalibration(), newLedCalibration = led.getCalibration(), ledIntensity = led.getIntensity();
string ledName = led.getName();
/*
This section of the code is dedicated to initialising variables that will be
used later on in the code.
*/

int main()
{
    
    
    fan.setStatus(false);
    /*
    Due to a weird feature that the application board has, pins 23 to 25 needs 
    to be set to 0 at system initialisation. This is because a "0" signal is 
    used to turn on the RGB LED, so the system outputs a "1" signal to set the 
    LED off at initialisation. This needs to be adjusted so the fan is off by 
    default.
    */

    gasError1 = gasSensor.begin();
    gasError2 = gasSensor.IAQinit();
    /*The gas sensor is required to use these methods to set up the hardware,
    and intialise the algorithm used.
    */

    initialLightData = 1 - ldr.read();
    float lightLevels[10] = { initialLightData, initialLightData, initialLightData, initialLightData, initialLightData, initialLightData, initialLightData, initialLightData, initialLightData, initialLightData };
    /*Used to get an initial set of Light values for the array. This array is
    used to take an average, so that the light's intensity doesn't vary so
    quickly.
    */

    lcd.locate(0, 0);
    lcd.printf("Project Leaf");
    wait(2);
    //Used to display project name.
    
    lcd.cls();
    lcd.locate(0, 0);
    lcd.printf("Initialising System...");
    //Message displayed while the system stalls for time for the DHT Sensor.
    
    /*
    This while loop is used to infinitely repeat the functionality of the
    program. It works by reading the data provided by the sensors, then
    controling the actuators based on that given input.
    */
    while (1)
    {
        dhtError = dht.readData(), humidityData = dht.readHumidity(), humidifierMode = humidifier.getAutomation(), humidifierStatus = humidifier.getStatus(), humidityTarget = humidifier.getTarget();
        temperatureData = dht.readTemperature(), fanMode = fan.getAutomation(), fanStatus = fan.getStatus(), temperatureTarget = fan.getTarget(), fanCalibration = fan.getCalibration(), fanIntensity = fan.getIntensity();
        /*
        Declares variables that will be used to store the data retunred by
        the class methods. Makes it so each method does not need to be
        called so often.
        */

        while (dhtError != 0)
        {
            wait(2);
            dhtError = dht.readData();
            lcd.cls();
            lcd.locate(0, 0);
            lcd.printf("DHT Error: %i\n", dhtError);
        }
        while (gasError1 == 0 || gasError2 == 0)
        {
            wait(2);
            dhtError = dht.readData();
            lcd.cls();
            lcd.locate(0, 0);
            lcd.printf("Gas Error1: %i\n", gasError1);
            lcd.printf("Gas Error2: %i\n", gasError2);
        }
        /*
        These while loops are positioned at the beginning of the loop as
        they used to check if there are any problems with the sensors.
        If there are any problems, the error values are displayed on the
        LCD for debugging.
        */

        if (humidityData >= humidityTarget && humidifierMode == true)
        {
            humidifier.setStatus(false);
            humidifierStatus = humidifier.getStatus();
        }
        else if (humidityData <= humidityTarget && humidifierMode == true)
        {
            humidifier.setStatus(true);
            humidifierStatus = humidifier.getStatus();
        }
        /*
        A simple algorithm used to check if the humidity meets the set
        target. If the humidity is above the target humidity, the
        humidifier is turned off. Otherwise, the humidifier is turned on.
        The "humidiferStatus" is used later on to update the data showed on
        the LCD.
        */

        if (temperatureData <= temperatureTarget && fanMode == true)
        {
            fan.setIntensity(0);
            fanIntensity = fan.getIntensity();
            fan.setStatus(false);
            fanStatus = fan.getStatus();
        }
        else if (temperatureData >= temperatureTarget && fanMode == true)
        {
            fan.setIntensity(min(1.0 * fanCalibration * (temperatureData - temperatureTarget), 1.0));
            /*
            Algorithm used to increase/decrease the fan's intensity based
            on the difference in temperarure and target. The
            "fanCalibration" variable is used to allow the user to adjust
            the rate of intensity.
            */
            fanIntensity = fan.getIntensity();
            fan.setStatus(true);
            fanStatus = fan.getStatus();
        }
        /*
        Used to check if the temperature meets the set
        target. If the temperature is below the target temperature, the
        fan is turned off. Otherwise, the fan is turned on.
        The "fanStatus" is used later on to update the data showed on
        the LCD.
        */

        moistureData = min(2.22 * moistureSensor.read(), 1.0);
        waterValveMode = waterValve.getAutomation(), waterValveStatus = waterValve.getStatus(), moistureTarget = waterValve.getTarget();
        /*
        The "min(2.0*moistureSensor.read()" part of the code is used to
        adjust the reading from the moisture sensor.
        */

        if (moistureData >= moistureTarget && waterValveMode == true)
        {
            waterValve.setStatus(false);
            waterValveStatus = waterValve.getStatus();
        }
        else if (moistureData <= moistureTarget && waterValveMode == true)
        {
            waterValve.setStatus(true);
            waterValveStatus = waterValve.getStatus();
        }
        /*
        Used to check if the moisture levels meets the set
        target. If the moisture level is above the target moisture level,
        the water valve is closed. Otherwise, the water valve is
        opened. The "waterValveStatus" is used later on to update the data
        showed on the LCD.
        */

        gasSensor.IAQmeasure();
        //Commands the gas sensor to take a single measurement of CO2.
        gasData = gasSensor.eCO2;
        gasValveMode = gasValve.getAutomation(), gasValveStatus = gasValve.getStatus(), gasTarget = gasValve.getTarget();
        /*
        The "eCO2" attribute is public, so it can be accessed it directly
        without the need of a set method.
        */

        if (gasData >= gasTarget && gasValveMode == true)
        {
            gasValve.setStatus(false);
            gasValveStatus = gasValve.getStatus();
        }
        else if (gasData <= gasTarget && gasValveMode == true)
        {
            gasValve.setStatus(true);
            gasValveStatus = gasValve.getStatus();
        }
        /*
        Used to check if the CO2 levels meets the set
        target. If the C02 level is above the target CO2 level,
        the gas valve is closed. Otherwise, the gas valve is
        opened. The "gasValveStatus" is used later on to update the data
        showed on the LCD.
        */

        if (lightCounter <= 9)
        {
            lightCounter++;
        }
        else
        {
            lightCounter = 0;
        }
        /*
        Used in conjunction with the "lightValues" array. The "lightCounter"
        is used to point to the position of the array. It increments by 1
        per cycle, and resets to 0 once it reaches the final position in the
        array.
        */

        lightData = 1 - ldr.read();
        //Used to adjust the reading from the LDR.
        lightLevels[lightCounter] = lightData;
        //Saves the currect light level to the current position in the array.
        lightAverage = (lightLevels[0] + lightLevels[1] + lightLevels[2] + lightLevels[3] + lightLevels[4] + lightLevels[5] + lightLevels[6] + lightLevels[7] + lightLevels[8] + lightLevels[9]) / 10;
        //Takes an average of all the saved light levels in the array
        ledMode = led.getAutomation(), ledStatus = led.getStatus(), lightTarget = led.getTarget(), ledCalibration = led.getCalibration(), ledIntensity = led.getIntensity();

        if (lightAverage >= lightTarget && ledMode == true)
        {
            led.setIntensity(0);
            ledIntensity = led.getIntensity();
            led.setStatus(false);
            ledStatus = led.getStatus();
        }
        else if (lightAverage <= lightTarget && ledMode == true)
        {
            led.setIntensity(min(1.0 * ledCalibration * (lightTarget - lightAverage), 1.0));
            ledIntensity = led.getIntensity();
            led.setStatus(true);
            ledStatus = led.getStatus();
        }
        /*
        Used to check if the average light level meets the set
        target. If the average light level is below the target light level,
        the LED is turned off. Otherwise, the LED is turned on.
        The "ledStatus" is used later on to update the data showed on
        the LCD.
        */
        
        //Start of the menu code.
        START:
            
            wait(2);
            lcd.cls();
            lcd.locate(0, 0);

            if (joyRight == 1)
            {
                menuCounter = menuCounter++;
                wait(0.2);
            }
            else if (joyLeft == 1)
            {
                menuCounter = menuCounter--;
                wait(0.2);
            }

            if (menuCounter <= -1)
            {
                menuCounter = 4;
            }
            else if (menuCounter >= 5)
            {
                menuCounter = 0;
            }

            switch (menuCounter)
            {
                case 0:
                    lcd.printf("Humidity: %3.0f%% ", humidityData);
                    lcd.printf("Target: %3.0f%%", humidityTarget);
                    lcd.locate(0, 10);
                    lcd.printf("%s Status: %i", humidifierName, humidifierStatus);
                    break;
                case 1:
                    lcd.printf("Temp: %3.0fC ", temperatureData);
                    lcd.printf("Target: %3.0fC\n", temperatureTarget);
                    lcd.printf("%s Status: %i\n", fanName, fanStatus);
                    lcd.printf("Int: %3.0f%%", 100 * fanIntensity);
                    break;
                case 2:
                    lcd.printf("Moisture Lvl: %3.0f%%\n", 100 * moistureData);
                    lcd.printf("Target: %3.0f%%\n", 100 * moistureTarget);
                    lcd.printf("%s Status: %i\n", waterValveName, waterValveStatus);
                    break;
                case 3:
                    lcd.printf("CO2 Lvl: %ippm\n", gasData);
                    lcd.printf("Target: %3.0fppm\n", gasTarget);
                    lcd.printf("%s Status: %i\n", gasValveName, gasValveStatus);
                    break;
                case 4:
                    lcd.printf("Light Lvl: %3.0f%%\n", 100 * lightData);
                    lcd.printf("Target: %3.0f%% ", 100 * lightTarget);
                    lcd.printf("Average: %3.0f%%\n", 100 * lightAverage);
                    lcd.printf("%s Status: %i ", ledName, ledStatus);
                    lcd.printf("Int: %3.0f%%", 100 * ledIntensity);
                    break;
            }
            //Code used to display and shift through the menu.

            if (joyClick == 1)
            {
                switch (menuCounter)
                {
                    case 0:
                        lcd.cls();
                        lcd.locate(0, 0);
                        lcd.printf("\nHumidity Selected");
                        wait(1);
                        goto HUMIDITY_MENU;
                    case 1:
                        lcd.cls();
                        lcd.locate(0, 0);
                        lcd.printf("\nTemperature Selected");
                        wait(1);
                        goto TEMP_MENU;
                    case 2:
                        lcd.cls();
                        lcd.locate(0, 0);
                        lcd.printf("\nMoisture Selected");
                        wait(1);
                        goto MOISTURE_MENU;
                    case 3:
                        lcd.cls();
                        lcd.locate(0, 0);
                        lcd.printf("\nGas Selected");
                        wait(1);
                        goto GAS_MENU;
                    case 4:
                        lcd.cls();
                        lcd.locate(0, 0);
                        lcd.printf("\nLight Selected");
                        wait(1);
                        goto LIGHT_MENU;
                }
            }
            //Code used when user goes into the settings screen.
    }
    //End of while loop.
    
    /*
    The following sections of code are used for customisable settings. These 
    include controlling and changing the following attributes and functions:
    Target, Automatimation Mode, Status, Intensity and Calibration.
    */

    //Start of menu code for humidity control.
    HUMIDITY_MENU:
        lcd.cls();
        lcd.locate(0, 0);
        lcd.printf("Up: Set Target Humidity\nMid: Set Status  Left: Back");

        newHumidifierStatus = humidifier.getStatus();

        if (joyUp == 1) 
        {
            while (joyClick != 1) 
            {
                lcd.cls();
                lcd.locate(0, 0);
                lcd.printf("Select a target humidity:\n%3.0f%% -> %3.0f%%\nMid: Back", humidityTarget, newHumidityTarget);
                wait(0.5);

                if (joyUp == 1 && (newHumidityTarget + 10 <= maxHumidityTarget)) 
                {
                    newHumidityTarget += 10;
                }
                else if (joyDown == 1 && (newHumidityTarget - 10 >= minHumidityTarget)) 
                {
                    newHumidityTarget -= 10;
                }
                else if (joyRight == 1 && (newHumidityTarget + 1 <= maxHumidityTarget)) 
                {
                    newHumidityTarget++;
                }
                else if (joyLeft == 1 && (newHumidityTarget - 1 >= minHumidityTarget)) 
                {
                    newHumidityTarget--;
                }
            }
            humidifier.setTarget(newHumidityTarget); 
        }
        
        else if (joyClick == 1) 
        {
            wait(1.5);
            while (joyClick != 1) 
            { 
                lcd.cls();
                lcd.locate(0, 0);
                lcd.printf("Toggle Auto: %d -> %d\nToggle Status: %d -> %d\nMid: Back", humidifierMode, newHumidifierMode, humidifierStatus, newHumidifierStatus);
                wait(0.5);

                if (joyUp == 1) 
                {
                    newHumidifierMode = true;
                }
                else if (joyDown == 1) 
                {
                    newHumidifierMode = false;
                }
                else if (joyRight == 1) 
                {
                    newHumidifierStatus = 1;
                }
                else if (joyLeft == 1) 
                {
                    newHumidifierStatus = 0;
                }
            }
            humidifier.setAutomation(newHumidifierMode);
            humidifier.setStatus(newHumidifierStatus);
        }
        else if (joyLeft == 1) 
        {
            goto START;
        }
        wait(1);
        goto HUMIDITY_MENU;
    //End of menu code for humidity control.

    //Start of menu code for temperature control.
    TEMP_MENU:
        lcd.cls();
        lcd.locate(0, 0);
        lcd.printf("Up: Set Target Temp\nMid: Set Status  Left: Back\nDown: Set Calibration");

        newFanStatus = fan.getStatus();

        if (joyUp == 1) 
        {
            while (joyClick != 1) 
            {
                lcd.cls();
                lcd.locate(0, 0);
                lcd.printf("Select a target temp:\n%3.0fC -> %3.0fC\nMid: Back", temperatureTarget, newTemperatureTarget);
                wait(0.5);

                if (joyUp == 1 && (newTemperatureTarget + 10 <= maxTemperatureTarget)) 
                {
                    newTemperatureTarget += 10;
                }
                else if (joyDown == 1 && (newTemperatureTarget - 10 >= minTemperatureTarget)) 
                {
                    newTemperatureTarget -= 10;
                }
                else if (joyRight == 1 && (newTemperatureTarget + 1 <= maxTemperatureTarget)) 
                {
                    newTemperatureTarget++;
                }
                else if (joyLeft == 1 && (newTemperatureTarget - 1 >= minTemperatureTarget)) 
                {
                    newTemperatureTarget--;
                }
            }
            fan.setTarget(newTemperatureTarget);
        }
        else if (joyClick == 1) 
        {
            wait(1.5);
            while (joyClick != 1) 
            {
                lcd.cls();
                lcd.locate(0, 0);
                lcd.printf("Toggle Auto: %d -> %d\nToggle Status: %d -> %d\nMid: Back", fanMode, newFanMode, fanStatus, newFanStatus);
                wait(0.5);

                if (joyUp == 1) 
                {
                    newFanMode = true;
                }
                else if (joyDown == 1) 
                {
                    newFanMode = false;
                }
                else if (joyRight == 1) 
                {
                    newFanStatus = 1;
                }
                else if (joyLeft == 1) 
                {
                    newFanStatus = 0;
                }
            }
            fan.setAutomation(newFanMode);
            fan.setStatus(newFanStatus);
        }
        else if (joyDown == 1) 
        {
            while (joyClick != 1) 
            {
                lcd.cls();
                lcd.locate(0, 0);
                lcd.printf("Select Calibration:\n%1.2f -> %1.2f\nMid: Back", fanCalibration, newFanCalibration);
                wait(0.5);

                if (joyUp == 1 && (newFanCalibration + 0.1 <= maxFanCalibration)) 
                {
                    newFanCalibration += 0.1;
                }
                else if (joyDown == 1 && (newFanCalibration - 0.1 >= minFanCalibration)) 
                {
                    newFanCalibration -= 0.1;
                }
                else if (joyRight == 1 && (newFanCalibration + 0.01 <= maxFanCalibration)) 
                {
                    newFanCalibration += 0.01;
                }
                else if (joyLeft == 1 && (newFanCalibration - 0.01 >= minFanCalibration)) 
                {
                    newFanCalibration -= 0.01;
                }
            }
            fan.setCalibration(newFanCalibration);
        }

        else if (joyLeft == 1) 
        {
            goto START;
        }

        wait(1);
        goto TEMP_MENU;
    //End of menu code for temperature control.

    //Start of menu code for soil irrigation control.
    MOISTURE_MENU:
        lcd.cls();
        lcd.locate(0, 0);
        lcd.printf("Up: Set Target Water Lvl\nMid: Set Status  Left: Back");

        newWaterValveStatus = waterValve.getStatus();

        if (joyUp == 1) 
        {
            while (joyClick != 1) 
            {
                lcd.cls();
                lcd.locate(0, 0);
                lcd.printf("Select a target water lvl:\n%3.0f%% -> %3.0f%%\nMid: Back", 100 * moistureTarget, 100 * newMoistureTarget);

                wait(0.5);

                if (joyUp == 1 && (newMoistureTarget + 0.1 <= maxMoistureTarget)) 
                {
                    newMoistureTarget += 0.1;
                }
                else if (joyDown == 1 && (newMoistureTarget - 0.1 >= minMoistureTarget)) 
                {
                    newMoistureTarget -= 0.1;
                }
                else if (joyRight == 1 && (newMoistureTarget + 1 <= maxMoistureTarget)) 
                {
                    newMoistureTarget += 0.01;
                }
                else if (joyLeft == 1 && (newMoistureTarget - 1 >= minMoistureTarget)) 
                {
                    newMoistureTarget -= 0.01;
                }
            }
            waterValve.setTarget(newMoistureTarget);
        }

        else if (joyClick == 1) 
        {
            wait(1.5);
            while (joyClick != 1) 
            {
                lcd.cls();
                lcd.locate(0, 0);
                lcd.printf("Toggle Auto: %d -> %d\nToggle Status: %d -> %d\nMid: Back", waterValveMode, newWaterValveMode, waterValveStatus, newWaterValveStatus);

                wait(0.5);

                if (joyUp == 1) 
                {
                    newWaterValveMode = true;
                }
                else if (joyDown == 1) 
                {
                    newWaterValveMode = false;
                }
                else if (joyRight == 1) 
                {
                    newWaterValveStatus = 1;
                }
                else if (joyLeft == 1) 
                {
                    newWaterValveStatus = 0;
                }
            }
            waterValve.setAutomation(newWaterValveMode);
            waterValve.setStatus(newWaterValveStatus);
        }

        else if (joyLeft == 1) 
        {
            goto START;
        }

        wait(1);
        goto MOISTURE_MENU;
    //End of menu code for soil irrigation control.

    //Start of menu code for CO2 saturation control.
    GAS_MENU:
        lcd.cls();
        lcd.locate(0, 0);
        lcd.printf("Up: Set Target Gas Lvl\nMid: Set Status  Left: Back");

        newGasValveStatus = gasValve.getStatus();

        if (joyUp == 1) 
        {
            while (joyClick != 1) 
            {
                lcd.cls();
                lcd.locate(0, 0);
                lcd.printf("Select a target gas lvl:\n%3.0f -> %3.0f\nMid: Back", gasTarget, newGasTarget);
                wait(0.5);

                if (joyUp == 1 && (newGasTarget + 10 <= maxGasTarget)) 
                {
                    newGasTarget += 10;
                }
                else if (joyDown == 1 && (newGasTarget - 10 >= minGasTarget)) 
                {
                    newGasTarget -= 10;
                }
                else if (joyRight == 1 && (newGasTarget + 1 <= maxGasTarget)) 
                {
                    newGasTarget++;
                }
                else if (joyLeft == 1 && (newGasTarget - 1 >= minGasTarget)) 
                {
                    newGasTarget--;
                }
            }
            gasValve.setTarget(newGasTarget);
        }

        else if (joyClick == 1) 
        {
            wait(1.5);
            while (joyClick != 1) 
            {
                lcd.cls();
                lcd.locate(0, 0);
                lcd.printf("Toggle Auto: %d -> %d\nToggle Status: %d -> %d\nMid: Back", gasValveMode, newGasValveMode, gasValveStatus, newGasValveStatus);
                wait(0.5);

                if (joyUp == 1) 
                {
                    newGasValveMode = true;
                }
                else if (joyDown == 1) 
                {
                    newGasValveMode = false;
                }
                else if (joyRight == 1) 
                {
                    newGasValveStatus = 1;
                }
                else if (joyLeft == 1) 
                {
                    newGasValveStatus = 0;
                }
            }
            gasValve.setAutomation(newGasValveMode);
            gasValve.setStatus(newGasValveStatus);
        }

        else if (joyLeft == 1)
        {
            goto START;
        }

        wait(1);
        goto GAS_MENU;
    //End of menu code for CO2 saturation control.

    //Start of menu code for light control.
    LIGHT_MENU:
        lcd.cls();
        lcd.locate(0, 0);
        lcd.printf("Up: Set Target Light Lvl\nMid: Set Status  Left: Back\nDown: Set Calibration");

        newLedStatus = led.getStatus();

        if (joyUp == 1) 
        {
            while (joyClick != 1) 
            {
                lcd.cls();
                lcd.locate(0, 0);
                lcd.printf("Select a target light lvl:\n%3.0f%% -> %3.0f%%\nMid: Back", 100 * lightTarget, 100 * newLightTarget);
                wait(0.5);

                if (joyUp == 1 && (newLightTarget + 0.1 <= maxLightTarget)) 
                {
                    newLightTarget += 0.1;
                }
                else if (joyDown == 1 && (newLightTarget - 0.1 >= minLightTarget)) 
                {
                    newLightTarget -= 0.1;
                }
                else if (joyRight == 1 && (newLightTarget + 0.01 <= maxLightTarget)) 
                {
                    newLightTarget += 0.01;
                }
                else if (joyLeft == 1 && (newLightTarget - 0.01 >= minLightTarget)) 
                {
                    newLightTarget -= 0.01;
                }
            }
            led.setTarget(newLightTarget);
        }

        else if (joyClick == 1) 
        {
            wait(1.5);
            while (joyClick != 1) 
            {
                lcd.cls();
                lcd.locate(0, 0);
                lcd.printf("Toggle Auto: %d -> %d\nToggle Status: %d -> %d\nMid: Back", ledMode, newLedMode, ledStatus, newLedStatus);
                wait(0.5);

                if (joyUp == 1) 
                {
                    newLedMode = true;
                }
                else if (joyDown == 1) 
                {
                    newLedMode = false;
                }
                else if (joyRight == 1) 
                {
                    newLedStatus = 1;
                }
                else if (joyLeft == 1) 
                {
                    newLedStatus = 0;                
                }
            }
            led.setAutomation(newLedMode);
            led.setStatus(newLedStatus);
        }

        else if (joyDown == 1) 
        {
            while (joyClick != 1) 
            {
                lcd.cls();
                lcd.locate(0, 0);
                lcd.printf("Select Calibration:\n%3.2f -> %3.2f\nMid: Back", ledCalibration, newLedCalibration);

                wait(0.5);

                if (joyUp == 1 && (newLedCalibration + 0.1 <= maxLedCalibration)) 
                {
                    newLedCalibration += 0.1;
                }
                else if (joyDown == 1 && (newLedCalibration - 0.1 >= minLedCalibration)) 
                {
                    newLedCalibration -= 0.1;
                }
                else if (joyRight == 1 && (newLedCalibration + 0.01 <= maxLedCalibration)) 
                {
                    newLedCalibration += 0.01;
                }
                else if (joyLeft == 1 && (newLedCalibration - 0.01 >= minLedCalibration)) 
                {
                    newLedCalibration -= 0.01;
                }
            }
            led.setCalibration(newLedCalibration);
        }

        else if (joyLeft == 1) 
        {
            goto START;
        }

        wait(1);
        goto LIGHT_MENU;
    //End of menu code for light control.
}