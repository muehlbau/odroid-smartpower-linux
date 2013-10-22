#ifndef H_SMARTMETER
#define H_SMARTMETER

/*
 * Copyright (c) 2013 Tobias Muehlbauer. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <thread>
#include <chrono>
#include "smartmeter.hpp"

using namespace std;

#define MAX_STR 65

SmartMeter::SmartMeter(uint intervall) :
		measurement(new Measurement()), device(NULL), meter(NULL), measure(
				false), intervall(intervall) {

}

bool SmartMeter::initDevice() {
	device = hid_open(0x04d8, 0x003f, NULL);
	if (device) {
		hid_set_nonblocking(device, true);
		return true;
	} else {
		cerr << "no device" << endl;
		return false;
	}
}

void SmartMeter::startMeasurement() {
	measurement->ampereSum=0;
	measurement->voltSum=0;
	measurement->wattSum=0;
	measurement->counter=0;
	if (measure == true) {
		cerr
				<< "An old measurement still runs. End it first before starting a new one.";
		return;
	}
	measure = true;
	meter = new thread(&SmartMeter::performMeasurement, this);
}

SmartMeter::Measurement SmartMeter::endMeasurement() {
	measure = false;
	meter->join();
	return *measurement;
}

void SmartMeter::performMeasurement() {
	unsigned char buf[MAX_STR];
	buf[0] = 0x00;
	memset((void*) &buf[2], 0x00, sizeof(buf) - 2);
	buf[1] = 0x37;
	while (measure) {
		if (hid_write(device, buf, sizeof(buf)) == -1) {
			cerr << "error" << endl;
			measurement->voltSum += 0;
			measurement->ampereSum += 0;
			measurement->wattSum += 0;
			measurement->counter = 0;
			exit(-1);
		}
		if (hid_read(device, buf, sizeof(buf)) == -1) {
			cerr << "error" << endl;
			measurement->voltSum += 0;
			measurement->ampereSum += 0;
			measurement->wattSum += 0;
			measurement->counter = 0;
			exit(-1);
		}
		if (buf[0] == 0x37) {
			char volt[7] = { '\0' };
			strncpy(volt, (char*) &buf[2], 5);
			char ampere[7] = { '\0' };
			strncpy(ampere, (char*) &buf[10], 5);
			char watt[7] = { '\0' };
			strncpy(watt, (char*) &buf[18], 5);

			measurement->voltSum += atof(volt);
			measurement->ampereSum += atof(ampere);
			measurement->wattSum += atof(watt);
			measurement->counter++;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(intervall));
	}
}



#endif
