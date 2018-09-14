//|___________________________________________________________________
//!
//! \file LogCurveDisplay.h
//!
//! \brief Log-curve display class.
//!
//! Author: Phillip & Nathan (modified by Mark).
//|___________________________________________________________________

#pragma once

//|___________________
//|
//| Local Includes
//|___________________

#include <osg\Geometry>

#include "LogCurve.h"

//|___________________________________________________________________
//|
//! Log-curve display class.
//|___________________________________________________________________

class LogCurveDisplay  : public osg::Geometry
{
public:
	 LogCurveDisplay(CLogCurve *curve, double width);
	~LogCurveDisplay(void);

private:	
	void createCurve();

  CLogCurve *curveInstance;
	double curveWidth;
};