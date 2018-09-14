//|___________________________________________________________________
//!
//! \file LogCurve.cpp
//!
//! \brief Single data curve holding float values and meta data.
//!
//! This class holds a single data curve (e.g. resistivity) including its meta data.
//!
//! Author: Phillip & Nathan (modified by Mark).
//|___________________________________________________________________

//|___________________
//|
//| Includes
//|___________________

#include "LogCurve.h"

//|___________________
//|
//| Static Initializers???
//|___________________

map<string, double> CLogCurve::maxCurveValue;
map<string, double> CLogCurve::minCurveValue;
map<string, double> CLogCurve::minRangeValue;
map<string, double> CLogCurve::maxRangeValue;

//|____________________________________________________________________
//|
//| Function: CLogCurve
//|
//! \param None.
//! \return None.
//!
//! Constructor. 
//|____________________________________________________________________

CLogCurve::CLogCurve(string _identifier, string _description) 
{
	identifier = _identifier;
	description = _description;

	max_sample_value = -10000.0f;
	min_sample_value =  10000.0f;

	if (maxCurveValue.count(identifier) == 0) 
		maxCurveValue[identifier] = max_sample_value;

	if (minCurveValue.count(identifier) == 0) 
		minCurveValue[identifier] = min_sample_value;

	if (minRangeValue.count(identifier) == 0)
		minRangeValue[identifier] = -10000.0f;

	if (maxRangeValue.count(identifier) == 0)
		maxRangeValue[identifier] =  10000.0f;
}

//|____________________________________________________________________
//|
//| Function: ~CLogCurve
//|
//! \param None.
//! \return None.
//!
//! Destructor. 
//|____________________________________________________________________

CLogCurve::~CLogCurve(void) 
{
	samples.clear();
}

//|____________________________________________________________________
//|
//| Function: appendSample
//|
//! \param None.
//! \return None.
//!
//! Add a single curve sample value and update the min / max values if necessary. 
//|____________________________________________________________________

void CLogCurve::appendSample(double _sample) 
{
	// CRITICAL Nathan: I don't know why this is in here, we are already rotating it 180 elsewhere
	// Invert values for SP curves
	//if (identifier == "SP")
	//	_sample *= -1;

	// Trim sample to valid range
	if (_sample > maxRangeValue[identifier])
		_sample = maxRangeValue[identifier];

	if (_sample < minRangeValue[identifier])
		_sample = minRangeValue[identifier];

	samples.push_back(_sample);
	if (_sample > max_sample_value) 
	{
		max_sample_value = _sample;
		// update static curve max value if necessary
		if (maxCurveValue[identifier] < max_sample_value) 
			maxCurveValue[identifier] = max_sample_value;
	}
	if (_sample < min_sample_value) 
	{
		min_sample_value = _sample;
		// update static curve min value if necessary
		if (minCurveValue[identifier] > min_sample_value) 
			minCurveValue[identifier] = min_sample_value;
	}
	current_sample = samples.begin();
}

//|____________________________________________________________________
//|
//| Function: getCurrentSample
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

double CLogCurve::getCurrentSample() 
{
	return *current_sample;
}

//|____________________________________________________________________
//|
//| Function: getNextSample
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

double CLogCurve::getNextSample() 
{
	if (current_sample != samples.end())
		++current_sample;
	
	return *current_sample;
}

//|____________________________________________________________________
//|
//| Function: setStartSamplePositionMeters
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void CLogCurve::setStartSamplePositionMeters(double _position) 
{
	start_sample_position = _position;
}

//|____________________________________________________________________
//|
//| Function: setStartSamplePositionFeet
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void CLogCurve::setStartSamplePositionFeet(double _position) 
{
	start_sample_position = _position * 0.3048f;
}

//|____________________________________________________________________
//|
//| Function: getStartSamplePositionMeters
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

double CLogCurve::getStartSamplePositionMeters() 
{
	return start_sample_position;
}

//|____________________________________________________________________
//|
//| Function: setSampleIntervalMeters
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void CLogCurve::setSampleIntervalMeters(double _sample_interval) 
{
	sample_interval = _sample_interval;
}

//|____________________________________________________________________
//|
//| Function: etSampleIntervalFeet
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void CLogCurve::setSampleIntervalFeet(double _sample_interval) 
{
	sample_interval = _sample_interval * 0.3048f;
}

//|____________________________________________________________________
//|
//| Function: getSampleIntervalMeters
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

double CLogCurve::getSampleIntervalMeters() 
{
	return sample_interval;
}

//|____________________________________________________________________
//|
//| Function: setIdentifier
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void CLogCurve::setIdentifier(string _identifier) 
{
	identifier = _identifier;
}

//|____________________________________________________________________
//|
//| Function: getIdentifier
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

string CLogCurve::getIdentifier() 
{
	return identifier;
}

//|____________________________________________________________________
//|
//| Function: setDescription
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void CLogCurve::setDescription(string _description) 
{
	description = _description;
}

//|____________________________________________________________________
//|
//| Function: getDescription
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

string CLogCurve::getDescription() 
{
	return description;
}

//|____________________________________________________________________
//|
//| Function: getNumberOfSamples
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

int CLogCurve::getNumberOfSamples() 
{
	return samples.size();
}

//|____________________________________________________________________
//|
//| Function: getMaxSampleValue
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

double CLogCurve::getMaxSampleValue() 
{
	return max_sample_value;
}

//|____________________________________________________________________
//|
//| Function: etMinSampleValue
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

double CLogCurve::getMinSampleValue() 
{
	return min_sample_value;
}
 
//|____________________________________________________________________
//|
//| Function: getMaxOverallValue
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

double CLogCurve::getMaxOverallValue(string id) 
{
	return maxCurveValue[id];
}

//|____________________________________________________________________
//|
//| Function: getMinOverallValue
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

double CLogCurve::getMinOverallValue(string id) 
{
	return minCurveValue[id];
}

//|____________________________________________________________________
//|
//| Function: setSampleValueRange
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void CLogCurve::setSampleValueRange(string id, double low, double high)
{
	minRangeValue[id] = low;
	maxRangeValue[id] = high;
}

//|____________________________________________________________________
//|
//| Function: getSampleValueRange
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void CLogCurve::getSampleValueRange(string id, double &low, double &high)
{
	low = minRangeValue[id];
	high = maxRangeValue[id];
}
