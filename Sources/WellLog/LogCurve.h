//|___________________________________________________________________
//!
//! \file LogCurve.h
//!
//! \brief Single data curve holding float values and meta data.
//!
//! This class holds a single data curve (e.g. resistivity) including its meta data.
//!
//! Author: Phillip (modified by Mark).
//|___________________________________________________________________

#ifndef __LOGCURVE_H
#define __LOGCURVE_H
#pragma once

//|___________________
//|
//| Local Includes
//|___________________

#include <string>
#include <vector>
#include <iostream>
#include <map>

using namespace std;

//|___________________________________________________________________
//|
//! This class holds a single data curve (e.g. resistivity) including its meta data.
//|___________________________________________________________________

class CLogCurve 
{
public:
	CLogCurve(string _identifier, string _description);
	~CLogCurve(void);

	void setIdentifier(string _identifier);
	string getIdentifier(void);
	void setDescription(string _description);
	string getDescription(void);

	void setSampleIntervalMeters(double _sample_interval);
	void setSampleIntervalFeet(double _sample_interval);
	double getSampleIntervalMeters();

	void setStartSamplePositionMeters(double _position);
	void setStartSamplePositionFeet(double _position);
	double getStartSamplePositionMeters();

	double getMaxSampleValue();
	double getMinSampleValue();

	void appendSample(double _sample);
	double getCurrentSample();
	double getNextSample();
	int getNumberOfSamples();

	// Holds the max and min occuring values for all curves with 
	// common identifier string
	static double getMaxOverallValue(string id);
	static double getMinOverallValue(string id);

	// Define the meaningful range of sample values to allow for
	// clipping incoming samples to a reasonable range
	static void setSampleValueRange(string id, double low, double high);
	static void getSampleValueRange(string id, double &low, double &high);

private:
	string identifier;
	string description;

	// Depth increment per sample (in meters)
	double sample_interval;

	// Depth position of first sample (in meters)
	double start_sample_position;

	double max_sample_value;
	double min_sample_value;
		
	vector<double> samples;
	vector<double>::iterator current_sample;

	friend class LogCurveDisplay;

	static map<string, double> maxCurveValue;
	static map<string, double> minCurveValue;
	
	static map<string, double> minRangeValue;
	static map<string, double> maxRangeValue;
};

#endif

