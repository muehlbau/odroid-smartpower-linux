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
#include <vector>
#include "smartgauge.hpp"
#include "hidapi.h"

using namespace std;

#define MAX_STR 65

#define REQUEST_DATA        0x37
#define REQUEST_STARTSTOP   0x80
#define REQUEST_STATUS      0x81
#define REQUEST_ONOFF       0x82
#define REQUEST_VERSION     0x83

int main(int argc, char *argv[])
{
  if (argc!=3 && argc!=4) {
    cerr << "Usage: sudo smartpower logfile program [threads]" << endl;
    exit(-1);
  }

  int threads = 1;
  if(argc==4){
    threads = atoi(argv[3]);
    if(threads<1){
      cerr << "Number of threads must be 1 or higher and not "<< threads << endl;
      exit(1);
    }
  }

  ofstream file;
  file.open(argv[1]);
  SmartGauge smartGauge;
  if(!smartGauge.initDevice()){
    exit(1);
  }

  thread t[threads];

  for (int i = 0; i < threads; ++i) {
    t[i] = thread(&system, argv[2]);
  }
  for (int i = 0; i < threads; ++i) {
    t[i].join();
  }
  
  double wh=smartGauge.getWattHour();

     cout << wh << endl;

     file << wh << endl;


  file.close();
}
