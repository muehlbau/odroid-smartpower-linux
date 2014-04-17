all: smartpower programgauge

CC       ?= gcc
CFLAGS   ?= -Wall -g -fpic

CXX      ?= g++
CXXFLAGS ?= -Wall -g -fpic -std=c++11

LDFLAGS  ?= -Wall -g

COBJS     = hid.o 
CPPOBJS   = main.o smartgauge.o
OBJS      = $(COBJS) $(CPPOBJS)
LIBS_USB  = `pkg-config libusb-1.0 --libs` -lrt -lpthread
LIBS      = $(LIBS_USB)

smartpower: $(COBJS) $(CPPOBJS)
	$(CXX) $(LDFLAGS) $^ $(LIBS_USB) -o $@

programgauge: $(COBJS) programgauge.o smartgauge.o
	$(CXX) $(LDFLAGS) $^ $(LIBS_USB) -o $@

$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@

$(CPPOBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) $< -o $@

lib: smartpower
	ar rvs libsmartgauge.a smartgauge.o hid.o

clean:
	rm -f $(OBJS) programgauge.o

.PHONY: clean libs
