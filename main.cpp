#include "mbed.h"
#include "_24LCXXX.h"
#include "USBSerial.h"
#include "EthernetInterface.h"
#include "TextLCD.h"

static int testEEPROM(void);
static int SocketDemo(void);

DigitalOut testLed(LED1);
DigitalOut usbLed(LED2);

I2C i2c(EEPROM_SDA, EEPROM_SCL);

Serial pc(UART0TX, UART0RX);

_24LCXXX eeprom(&i2c, 0x50);

USBSerial usbVCom(false, 0x1f00, 0x2012, 0x0001);

TextLCD lcd(J13_1, J13_2, J13_3, J13_4, J13_5, J13_6);// rs, e, d4-d7

int main() {
    usbVCom.connect();
    lcd.printf("NGX LCD Demo\n");
    pc.baud(9600);
    pc.printf("NGX mbed LED blinky demo\r\n");

    pc.printf("Test EEPROM Started\r\n");
    if (1 != testEEPROM()) {
        pc.printf("Test EEPROM failed\r\n");
    } else {
        pc.printf("Test EEPROM passed\r\n");
    }
    pc.printf("NGX mbed Ethernet Example Demo Started\r\n");
    SocketDemo();
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

// Network interface
EthernetInterface net;

// Socket demo
static int SocketDemo(void) {
    // Bring up the ethernet interface
    pc.printf("Ethernet socket example\n");
    net.connect();

    // Show the network address
    const char *ip = net.get_ip_address();
    pc.printf("IP address is: %s\n", ip ? ip : "No IP");

    // Open a socket on the network interface, and create a TCP connection to mbed.org
    TCPSocket socket;
    socket.open(&net);
    socket.connect("www.arm.com", 80);

    // Send a simple http request
    char sbuffer[] = "GET / HTTP/1.1\r\nHost: www.arm.com\r\n\r\n";
    int scount = socket.send(sbuffer, sizeof sbuffer);
    pc.printf("sent %d [%.*s]\n", scount, strstr(sbuffer, "\r\n")-sbuffer, sbuffer);

    // Recieve a simple http response and print out the response line
    char rbuffer[64];
    int rcount = socket.recv(rbuffer, sizeof rbuffer);
    pc.printf("recv %d [%.*s]\n", rcount, strstr(rbuffer, "\r\n")-rbuffer, rbuffer);

    // Close the socket to return its memory and bring down the network interface
    socket.close();

    // Bring down the ethernet interface
    net.disconnect();
    pc.printf("Done\n");
    return 1;
}
