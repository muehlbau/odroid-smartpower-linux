/*
 * smartmeter.hpp
 *
 *  Created on: Oct 17, 2013
 *      Author: seilbeck
 */

#ifndef SMARTMETER_HPP_
#define SMARTMETER_HPP_

#include "hidapi.h"

class SmartMeter {

public:
	struct Measurement {
		uint counter;
		double wattSum;
		double voltSum;
		double ampereSum;
	};

	/**
	 * interval - In which time intervals in ms is the energy consumption checked
	 */
	SmartMeter(uint interval);

	/**
	 * Initializes device.
	 * Returns false if no device could be found. True if initialization was successful.
	 *
	 */
	bool initDevice();

	void startMeasurement();

	/**
	 * Ends a measurement and returns a Measurement struct.
	 * This struct can be used to calculate the average energy consumption e.g. Measurment.wattSum / Measurement.counter
	 *
	 */
	Measurement endMeasurement();

	~SmartMeter(){
		if(device!=NULL)
			hid_close(device);
		if(measurement!=NULL)
			delete measurement;
		if(meter!=NULL)
			delete meter;
	};

private:
	Measurement* measurement;
	hid_device* device;
	std::thread* meter;
	bool measure;
	uint intervall;

	void performMeasurement();
};

#endif /* SMARTMETER_HPP_ */
