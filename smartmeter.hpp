/*
 * smartmeter.hpp
 *
 *  Created on: Oct 17, 2013
 *      Author: seilbeck
 */

#ifndef SMARTMETER_HPP_
#define SMARTMETER_HPP_

#include "hidapi.h"
#include <thread>
#include <vector>

#define MAX_STR 65

using namespace std;

class SmartMeter {

public:
	struct Measurement {
		double watt;
		double volt;
		double ampere;
		double wattHour;
	};

	SmartMeter();

	/**
	 * Initializes and resets the device.
	 * Returns false if no device could be found. True if initialization was successful.
	 * If this is done in short intervals, the hardware has troubles and sometimes delivers wrong (negativ) values.
	 */
	bool initDevice();

	/**
	 * Resets the powermeter. If this is done in short intervals, the hardware has troubles and sometimes delivers wrong (negativ) values.
	 */
	void reset();

	double getWattHour();
	double getVolt();
	double getAmpere();
	double getWatt();
	Measurement getMeasurement();
	/**
	 * interval - In which time intervals in ms is the energy consumption sampled
	 */
	void startSampling(uint intervall);

	/**
	 * Ends a measurement and returns a vector of Measurement structs.
	 *
	 */
	vector<SmartMeter::Measurement> endSampling();

	~SmartMeter();

private:
	vector<Measurement> measurements;
	hid_device* device;
	std::thread* meter;
	bool measure;
	unsigned char buf[MAX_STR];

	void collectSamples(uint intervall);
	void requestData();
	void requestStatus();
	void requestStartStop(bool started);
};

#endif /* SMARTMETER_HPP_ */
