# This repository contains support for NGX development board on mbed platform
* [Blueboard LPC1768 Header](https://ngxkart.com/collections/blueboard/products/blueboard-lpc1768-h)
<!-- * [LPC1768 Xplorer](https://ngxkart.com/collections/xplorer/products/lpc1768-xplorer)
* [mbed-Xpresso Baseboard](https://ngxkart.com/products/mbed-xpresso-baseboard?_pos=2&_sid=8b2764250&_ss=r) -->

# External dependencies
<!-- * install [VSCode]() -->
* Install [Python 2.7.11]() or [above]() and add to PATH
* install [Git]() and add to PATH

#### Project Setup:
* git clone --recursive repo-URL
* cd mbed-ngx-lpc1768-header
* pip install -r requirement.txt
* python mbed-os/tools/make.py -t GCC_ARM -m NGX_BLUEBOARD_LPC1768_H --source . --build ./build
* bin files will be generated in the build folder i.e. mbed-ngx-lpc1768-header.bin
* The bin files can be flashed using [CoFlash]()

<!-- ###### Replace TARGET with below name while building:

* Blueboard LPC1768 Header : NGX_BLUEBOARD_LPC1768_H 
* LPC1768 Xplorer : NGX_LPC1768_Xplorer -->
