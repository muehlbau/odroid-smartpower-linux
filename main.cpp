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
#include <chrono>
#include <thread>
#include "smartmeter.hpp"


using namespace std;

#define MAX_STR 65

void test() {
	SmartMeter sm(100);
	cout << "init device" << endl;
	sm.initDevice();
	cout << "start measurement" << endl;
	sm.startMeasurement();

	cout << "go to sleep" << endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	cout << "end measurement" << endl;
	SmartMeter::Measurement m = sm.endMeasurement();
	cout << m.counter << " counter" << endl;
	cout << m.wattSum / m.counter << " average watt" << endl;

	sm.startMeasurement();
	cout << "go to sleep" << endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	cout << "end measurement" << endl;
	m = sm.endMeasurement();
}

int main(int argc, char *argv[]) {
	test();
//  if (argc!=2) {
//    cerr << "Usage: sudo smartpower logfile" << endl;
//    exit(-1);
//  }

//  ofstream file;
//  file.open(argv[1]);
//	thread test(&SmartMeter::nocheintest, new SmartMeter());
//	test.join();
}
