![Test Image 1](./Images/BlueBoard_LPC1768_H.png)
# NGX Blueboard-LPC1768-H board support for Mbed OS
* [Blueboard LPC1768 Header](https://ngxkart.com/collections/blueboard/products/blueboard-lpc1768-h)

# Features
* mbed enabled
    * Easy to use C/C++ SDK
    * Handy libraries
* 20 pin JTAG/SWD programming connector
* USB device interface
* NXP LPC1768 MCU
    * Powerful ARM Cortex-M3 core
    * Up to 100MHz
    * 512KB Flash, 64KB RAM 
    * Ethernet, USB Host/Device, 4xUART, 3xI2C, 2xSPI

# External dependencies
* Install [Python 2.7.11](https://www.python.org/download/releases/2.7/) or [above](https://www.python.org/downloads/release/python-377/) and add to PATH
* Install [pip](https://www.liquidweb.com/kb/install-pip-windows/)
* Install [gcc-arm-none-eabi](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
* install [Git](https://git-scm.com/download/win) and add to PATH

#### Project Setup:
* open Git bash
* git clone --recursive repo-URL
* cd mbed-ngx-lpc1768-header
* pip install -r mbed-os/requirements.txt
* python mbed-os/tools/make.py -t GCC_ARM -m NGX_BLUEBOARD_LPC1768_H --source . --build ./build
* bin files will be generated in the build folder i.e. mbed-ngx-lpc1768-header.bin
* The bin files can be flashed using [CoFlash](./tools/CoFlash-1.4.8.exe)

### Mbed OS version support
 
| Bluepill          | Mbed OS  (hash)                           | Status              |
| ----------------- | ----------------------------------------- | ------------------- |
| preview           | beta ([#1b2a68ae](https://github.com/ARMmbed/mbed-os/#1b2a68ae204b70a7aaf3818dc8fa3881250b13b0))                     | Compiles and runs ok         |

### Documents
* [Schematic](./docs/schematics/Blueboard_lpc1768H-V2.pdf)

### Testing
* [Blinky Example Code]()
* [UART Serial Example Code]()
* [USB  Serial Example Code]()

### Programming with CoFlash programming utility
Blueboard LPC1768 Header can be programmed using standard 20-pin JTAG or SWD interface connector using any standard JTAG adapter.

![CoFlash1](./Images/coflash1.PNG)

![CoFlash2](./Images/coflash2.PNG)

This is the pinout of the NGX Blueboard LPC1768 H:

<!-- ###### Replace TARGET with below name while building:

* Blueboard LPC1768 Header : NGX_BLUEBOARD_LPC1768_H 
* LPC1768 Xplorer : NGX_LPC1768_Xplorer -->
