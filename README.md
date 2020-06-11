# This repository contains support for NGX development board on mbed platform
* [Blueboard LPC1768 Header](https://ngxkart.com/collections/blueboard/products/blueboard-lpc1768-h)
* [LPC1768 Xplorer](https://ngxkart.com/collections/xplorer/products/lpc1768-xplorer)
* [mbed-Xpresso Baseboard](https://ngxkart.com/products/mbed-xpresso-baseboard?_pos=2&_sid=8b2764250&_ss=r)


# Enter below command to build the specified target:

python mbed-os/tools/make.py -t GCC_ARM -m TARGET --source . --build ./build

#### Replace TARGET with below name while building:

* Blueboard LPC1768 Header : NGX_BLUEBOARD_LPC1768_H
* LPC1768 Xplorer : NGX_LPC1768_Xplorer
