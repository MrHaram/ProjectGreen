#include "mbed.h"
#include "XFAN.h"


PwmOut fan(p25);

int main()
{
    while(1) {
        for(intensity=0; intensity<=100; intensity+=10) {
            fan.write(intensity/100.0f);
            wait(0.5);
        }
    }