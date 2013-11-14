odroid-smartpower-linux
=======================

linux command line tools for hardkernel smart power device

hardkernel.com Smart Power for ODROID development boards: http://www.hardkernel.com/renewal_2011/products/prdt_info.php?g_code=G137361754360

We provide a linux command line tool to log the voltage, ampere, and watts status from a smart power device
to a file.

Prerequisites:

- libusb (Ubuntu/Linaro Ubuntu: sudo apt-get install libusb-1.0-0-dev)
- pkg-config (sudo apt-get install pkg-config)
- GCC 4.7+ 

Installation:

1. Clone the repository
2. Run make in repository

Usage:

    sudo ./smartpower logfile

Why sudo?

By default, libusb is not granted write access to a usb device, which is needed to request status information from the device.
Thus, either smartpower is run as sudo, or libusb permissions in /lib/udev/rules.d/50-udev-default.rules are changed to allow write access.


Library
=======
Easy-to-use C++ API for the smart power device.

Installation:

make lib

Usage:

    #include <iostream>
    #include <chrono>
    #include <thread>
    #include "smartmeter.hpp"
    
    using namespace std;
    
    int main(int argc, char *argv[]) {
    	SmartMeter sm(100);
    	sm.initDevice();
    	sm.startMeasurement();
    
    	this_thread::sleep_for(std::chrono::seconds(5));
    
    	SmartMeter::Measurement m = sm.endMeasurement();
    	cout << m.wattSum / m.counter << " average watt" << endl;
    }

Compilation:
    
    export ISMARTMETER="directory of smartmeter.hpp"
    export LSMARTMETER="directory of libsmartmeter.a"
    g++ -I$ISMARTMETER -L$LSMARTMETER file_to_compile.cpp -lsmartmeter -lpthread -lusb-1.0 -lrt
  
