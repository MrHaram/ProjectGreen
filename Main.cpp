#include "mbed.h"
#include "C12832.h"
#include "DHT11.h"
#include "XFAN.h"

C12832 lcd(p5, p7, p6, p8, p11); //Calls LCD Library and assigns pins
DHT11 dht(p12); //Calls Themometer and Humidity dht Library and assigns pins
XFAN fan(p25); //Assigns pin to fan

int main()
{
    //Used to display project name
    string p;
    lcd.locate(0,0);
    lcd.printf("Project Shrek");
    wait(2.0);
    lcd.cls();

    int error = 0, t = 0.0, h = 0.0;
    /*Declares the variables t and h which will be  used to store the
    Temeperature and Humidity*/

    error = dht.readData();//Variable used for debugging
    //If the error variable is 0, the "Reading DHT data..." is printed
    if (error == 0) {
        lcd.cls();
        lcd.locate(0,0);
        lcd.printf("Entering Shrek's Swamp...\n");
    }
    /*Loop starts with updating the error variable. Loop then prints the
    Temperature and Humiditiy to the LCD. This process is repeated every
    two seconds due to how "slow" the DHT sensor is.*/
    while(1) {
        wait(2.0);
        error = dht.readData();
        if (0 == error) {
            t = dht.readTemperature();
            h = dht.readHumidity();
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("DHT Sensor:\n");
            lcd.printf("Temp: %3iC\n", t);
            lcd.printf("Humidity : %3i%%\n", h);
        } else {
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("DHT Error: %d\n", error);
        }
    }
}