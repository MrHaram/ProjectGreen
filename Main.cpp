#include "mbed.h"
#include "C12832.h"
#include "DHT11.h"

C12832 lcd(p5, p7, p6, p8, p11); //Calls LCD Library and assigns pins
DHT11 dht(p12); //Calls Themometer and Humidity dht Library and assigns pins

int main()
{
    //Used to display project name
    lcd.locate(0,0);
    lcd.printf("Project Leaf");
    wait(2.0);
    lcd.cls();

    int error = 0;
    float t = 0.0, h = 0.0; 
    /*Declares the variables t and h which will be  used to store the 
    Temeperature and Humidity*/

    error = dht.readData();//Variable used for debugging
    //If the error variable is 0, the "Reading DHT data..." is printed
    if (error == 0) {
        lcd.cls();
        lcd.locate(0,0);
        lcd.printf("Reading DHT data...\n");
    }
    /*Loop starts with updating the error variable. Loop then prints the 
    Temperature and Humiditiy to the LCD. This process is repeated every
    two seconds due to how "slow" the DHT sensor is.*/
    while(1) {
        wait(2.0);
        error = dht.readData();
        if (0 == error) {
            t   = dht.readTemperature();
            h   = dht.readHumidity();
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("Temp: %4.2f\n", t);
            lcd.printf("Humidity: %4.2f\n", h);
        } else {
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("DHT Error: %d\n", error);
        }
    }
}