#include "mbed.h"
#include "_24LCXXX.h"

int testEEPROM(void);

DigitalOut testLed(LED1);
DigitalOut usbLed(LED2);

I2C i2c(EEPROM_SDA, EEPROM_SCL);

Serial pc(UART0TX, UART0RX);

_24LCXXX eeprom(&i2c, 0x50);

int main() {
    pc.baud(9600);
    pc.printf("NGX mbed LED blinky demo\r\n");

    pc.printf("Test EEPROM Started\r\n");
    if (1 != testEEPROM()) {
        pc.printf("Test EEPROM failed\r\n");
    } else {
        pc.printf("Test EEPROM passed\r\n");
    }

    while(1) {
        testLed = 1;
        usbLed = 1;
        wait_us(50000);
        testLed = 0;
        usbLed = 0;
        wait_us(50000);
    }
}

int testEEPROM(void) {
    char testData[] = {0xA5, 0xAA, 0x55, 0x26};
    int dataRead;
    char dataByteRead;
    int retval = 1;

    do {
        for (int i = 0; i < sizeof(testData) / sizeof(testData[0]); i++) {
            eeprom.byte_write(i, testData[i]);
        }
        
        for (int i = 0; i < sizeof(testData) / sizeof(testData[0]); i++) {
            eeprom.nbyte_read(i, &dataByteRead, sizeof(dataByteRead));
            if (dataByteRead != testData[i]) {
                retval = 0;
                break;
            }
        }
        if (0 == retval) {
            break;
        }
        
        dataRead = 0xAA55AA55;
        eeprom.nbyte_write( 10, &dataRead, sizeof(int) );
        eeprom.nbyte_read( 10, &dataRead, sizeof(int) );

        if (0xAA55AA55 != dataRead) {
            retval = 0;
            break;
        }
    } while (0);
    return retval;
}