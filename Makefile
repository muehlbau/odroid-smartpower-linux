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

install: lib
	install -D libsmartgauge.a /usr/local/lib/
	@mkdir -p /usr/local/include/smartgauge
	install -D smartgauge.hpp /usr/local/include/smartgauge/

uninstall:
	rm -f /usr/local/lib/libsmartgauge.a
	rm -rf /usr/local/include/smartgauge

clean:
	rm -f $(OBJS) programgauge.o

cleanall: clean
	rm -f smartpower libsmartgauge.a

.PHONY: clean libs
