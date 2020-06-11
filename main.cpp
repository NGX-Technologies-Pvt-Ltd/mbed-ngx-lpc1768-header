#include "mbed.h"

DigitalOut testLed(LED1);
DigitalOut usbLed(LED2);

int main()
{
    while(1) {
        testLed = 0;
        usbLed = 0;
        wait_us(50000);
        testLed = 1;
        usbLed = 1;
        wait_us(50000);
    }
}