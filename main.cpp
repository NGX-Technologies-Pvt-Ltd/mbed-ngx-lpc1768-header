#include "mbed.h"
#include "_24LCXXX.h"
#include "usb_phy_api.h"
#include "USBSerial.h"
#include "EthernetInterface.h"
#include "TextLCD.h"

static int testEEPROM(void);
static int SocketDemo(void);

DigitalOut testLed(LED1);
DigitalOut usbLed(LED2);

I2C i2c(EEPROM_SDA, EEPROM_SCL);

static BufferedSerial buffered_serial_obj(
    UART0TX, UART0RX, 115200
);

_24LCXXX eeprom(&i2c, 0x50);

// TextLCD lcd(J13_1, J13_2, J13_3, J13_4, J13_5, J13_6);// rs, e, d4-d7
USBSerial usbVCom(false, 0x1f00, 0x2012, 1);
 
int main(void) {
    usbVCom.connect();
    // lcd.printf("NGX LCD Demo\n");
    printf("NGX mbed LED blinky demo\r\n");

    printf("Test EEPROM Started\r\n");
    if (1 != testEEPROM()) {
        printf("Test EEPROM failed\r\n");
    } else {
        printf("Test EEPROM passed\r\n");
    }
    printf("NGX mbed Ethernet Example Demo Started\r\n");
    // SocketDemo();
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
    return 1;
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

#if 0
// Network interface
EthInterface net;

// Socket demo
static int SocketDemo(void) {
    // Bring up the ethernet interface
    printf("Ethernet socket example\n");
    net.connect();

    // Show the network address
    const char *ip = net.get_ip_address();
    printf("IP address is: %s\n", ip ? ip : "No IP");

    // Open a socket on the network interface, and create a TCP connection to mbed.org
    TCPSocket socket;
    socket.open(&net);
    socket.connect("www.arm.com", 80);

    // Send a simple http request
    char sbuffer[] = "GET / HTTP/1.1\r\nHost: www.arm.com\r\n\r\n";
    int scount = socket.send(sbuffer, sizeof sbuffer);
    printf("sent %d [%.*s]\n", scount, strstr(sbuffer, "\r\n")-sbuffer, sbuffer);

    // Recieve a simple http response and print out the response line
    char rbuffer[64];
    int rcount = socket.recv(rbuffer, sizeof rbuffer);
    printf("recv %d [%.*s]\n", rcount, strstr(rbuffer, "\r\n")-rbuffer, rbuffer);

    // Close the socket to return its memory and bring down the network interface
    socket.close();

    // Bring down the ethernet interface
    net.disconnect();
    printf("Done\n");
    return 1;
}

#else

NetworkInterface *net;

// Socket demo
int SocketDemo() {
    int remaining;
    int rcount;
    char *p;
    char buffer[256];
    nsapi_size_or_error_t result;

    // Bring up the ethernet interface
    printf("Mbed OS Socket example\n");

#ifdef MBED_MAJOR_VERSION
    printf("Mbed OS version: %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif

    net = NetworkInterface::get_default_instance();

    if (!net) {
        printf("Error! No network inteface found.\n");
        return 0;
    }

    result = net->connect();
    if (result != 0) {
        printf("Error! net->connect() returned: %d\n", result);
        return result;
    }

    // Show the network address
    SocketAddress a;
    net->get_ip_address(&a);
    printf("IP address: %s\n", a.get_ip_address() ? a.get_ip_address() : "None");
    net->get_netmask(&a);
    printf("Netmask: %s\n", a.get_ip_address() ? a.get_ip_address() : "None");
    net->get_gateway(&a);
    printf("Gateway: %s\n", a.get_ip_address() ? a.get_ip_address() : "None");

    // Open a socket on the network interface, and create a TCP connection to ifconfig.io
    TCPSocket socket;
    // Send a simple http request
    char hostname[] = "ifconfig.io";
    char sbuffer[] = "GET / HTTP/1.1\r\nHost: ifconfig.io\r\nConnection: close\r\n\r\n";
    nsapi_size_t size = strlen(sbuffer);

    result = socket.open(net);
    if (result != 0) {
        printf("Error! socket.open() returned: %d\n", result);
        goto DISCONNECT;
    }

    // Get the host address
    printf("\nResolve hostname %s\n", hostname);
    result = net->gethostbyname(hostname, &a);
    if (result != 0) {
        printf("Error! gethostbyname(%s) returned: %d\n", hostname, result);
        net->disconnect();
        exit(-1);
    }
    printf("%s address is %s\n", hostname, (a.get_ip_address() ? a.get_ip_address() : "None") );

    a.set_port(80);
    result = socket.connect(a);
    if (result != 0) {
        printf("Error! socket.connect() returned: %d\n", result);
        net->disconnect();
        exit(-2);
    }

    // Loop until whole request sent
    while (size) {
        result = socket.send(sbuffer+result, size);
        if (result < 0) {
            printf("Error! socket.send() returned: %d\n", result);
            goto DISCONNECT;
        }
        size -= result;
        printf("sent %d [%.*s]\n", result, strstr(sbuffer, "\r\n")-sbuffer, sbuffer);
    }

    // Receieve an HTTP response and print out the response line
    remaining = 256;
    rcount = 0;
    p = buffer;
    while (remaining > 0 && 0 < (result = socket.recv(p, remaining))) {
        p += result;
        rcount += result;
        remaining -= result;
    }
    if (result < 0) {
        printf("Error! socket.recv() returned: %d\n", result);
        goto DISCONNECT;
    }
	// the HTTP response code
    printf("recv %d [%.*s]\n", rcount, strstr(buffer, "\r\n")-buffer, buffer);

DISCONNECT:

    // Close the socket to return its memory and bring down the network interface
    socket.close();

    // Bring down the ethernet interface
    net->disconnect();
    printf("Done\n");
    return 1;
}

#endif

FileHandle *mbed::mbed_override_console(int fd)
{
    return &buffered_serial_obj;
}
