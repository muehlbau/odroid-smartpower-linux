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
  if (argc!=2) {
    cerr << "Usage: sudo smartpower logfile" << endl;
    exit(-1);
  }

  ofstream file;
  file.open(argv[1]);
  hid_device* device;
  unsigned char buf[MAX_STR];
  buf[0]=0x00;
  memset((void*)&buf[2],0x00,sizeof(buf)-2);

  device=hid_open(0x04d8,0x003f,NULL);
  if (device) {
    hid_set_nonblocking(device,true);
  } else {
    cerr << "no device" << endl;
    exit(-1);
  }

  buf[1] = REQUEST_STATUS;
      if (hid_write(device,buf,sizeof(buf))==-1) {
      cerr << "error" << endl;
      exit(-1);
    }
    if (hid_read(device,buf,sizeof(buf))==-1) {
      cerr << "error" << endl;
      exit(-1);
    }
  bool started = (buf[1] == 0x01);
  if (!started) {
     buf[1] = REQUEST_STARTSTOP;
     if (hid_write(device,buf,sizeof(buf))==-1) {
        cerr << "error" << endl;
        exit(-1);
     }
  }

  buf[1] = REQUEST_STARTSTOP;
  if (hid_write(device,buf,sizeof(buf))==-1) {
     cerr << "error" << endl;
     exit(-1);
  }
  
  buf[1] = REQUEST_DATA;
  long run=0;
  bool first=true;
  while (true) {
    if (hid_write(device,buf,sizeof(buf))==-1) {
      cerr << "error" << endl;
      exit(-1);
    }
    if (hid_read(device,buf,sizeof(buf))==-1) {
      cerr << "error" << endl;
      exit(-1);
    }

    if(buf[0]==REQUEST_DATA) {
      if (!first) {
         char volt[7]={'\0'};
         strncpy(volt,(char*)&buf[2],5);

         char ampere[7]={'\0'};
         strncpy(ampere,(char*)&buf[10],5);
    
         char watt[7]={'\0'};
         strncpy(watt,(char*)&buf[18],5);

         char wh[7]={'\0'};
         strncpy(wh,(char*)&buf[26],5);

         cout << wh << endl;

         file << (run*100) << "," << volt << "," << ampere << "," << watt << "," << wh << endl;
      } else {
         first=false;
      }
    }

    ++run;
    usleep(100000);
  }

  buf[1] = REQUEST_STARTSTOP;
  if (hid_write(device,buf,sizeof(buf))==-1) {
     cerr << "error" << endl;
     exit(-1);
  }

  file.close();
  hid_close(device);
}
