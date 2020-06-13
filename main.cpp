#include "mbed.h"
#include "_24LCXXX.h"
#include "USBSerial.h"

static int testEEPROM(void);

DigitalOut testLed(LED1);
DigitalOut usbLed(LED2);

I2C i2c(EEPROM_SDA, EEPROM_SCL);

Serial pc(UART0TX, UART0RX);

_24LCXXX eeprom(&i2c, 0x50);

USBSerial usbVCom(false, 0x1f00, 0x2012, 0x0001);

int main() {    
    usbVCom.connect();
    pc.baud(9600);
    pc.printf("NGX mbed LED blinky demo\r\n");

    pc.printf("Test EEPROM Started\r\n");
    if (1 != testEEPROM()) {
        pc.printf("Test EEPROM failed\r\n");
    } else {
        pc.printf("Test EEPROM passed\r\n");
    }

    while(1) {
        if (usbVCom.connected()) {
            usbVCom.printf("I am a virtual COM port\r\n");
        }
        testLed = 1;
        usbLed = 1;
        wait_us(50000);
        testLed = 0;
        usbLed = 0;
        wait_us(50000);
    }
}

static int testEEPROM(void) {
    char testData[] = {0xA5, 0xAA, 0x55, 0x26};
    int dataRead;
    char dataByteRead;
    int retval = 1;

    do {
        for (unsigned int i = 0; i < sizeof(testData) / sizeof(testData[0]); i++) {
            eeprom.byte_write(i, testData[i]);
        }
        
        for (unsigned int i = 0; i < sizeof(testData) / sizeof(testData[0]); i++) {
            eeprom.nbyte_read(i, &dataByteRead, sizeof(dataByteRead));
            if (dataByteRead != testData[i]) {
                retval = 0;
                break;
            }
        }
        if (0 == retval) {
            break;
        }
        
        dataRead = (int)0xAA55AA55;
        eeprom.nbyte_write( 10, &dataRead, sizeof(int) );
        eeprom.nbyte_read( 10, &dataRead, sizeof(int) );

        if ((int)0xAA55AA55 != dataRead) {
            retval = 0;
            break;
        }
    } while (0);
    return retval;
}
