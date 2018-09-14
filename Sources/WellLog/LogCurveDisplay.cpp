//|___________________________________________________________________
//!
//! \file LogCurveDisplay.cpp
//!
//! \brief Log-curve display class.
//!
//! Author: Phillip & Nathan (modified by Mark).
//|___________________________________________________________________

//|___________________
//|
//| Includes
//|___________________

// DELETED #include <osg/GmtlMatrixTransform>

#include "LogCurveDisplay.h"

//|____________________________________________________________________
//|
//| Function: LogCurveDisplay
//|
//! \param None.
//! \return None.
//!
//! Constructor. 
//|____________________________________________________________________

LogCurveDisplay::LogCurveDisplay(CLogCurve *curve, double width) : osg::Geometry()
{
	curveWidth = width;
	if (curve != 0) {
		curveInstance = curve;
		createCurve();
	}
}

//|____________________________________________________________________
//|
//| Function: ~LogCurveDisplay
//|
//! \param None.
//! \return None.
//!
//! Destructor. 
//|____________________________________________________________________

LogCurveDisplay::~LogCurveDisplay(void)
{}

//|____________________________________________________________________
//|
//| Function: createCurve
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void LogCurveDisplay::createCurve() 
{		
	double intervalInMeters = curveInstance->getSampleIntervalMeters();
	double depthInMeters = curveInstance->getStartSamplePositionMeters();

	double minValue, meanValue, maxValue;
	CLogCurve::getSampleValueRange((curveInstance->getIdentifier()), minValue, maxValue);
	meanValue = (minValue + maxValue) / 2;
	double scaleFactor = curveWidth / (maxValue - minValue);
	
	//reflecting about origin if sp, since sp is displayed on the left
	if(curveInstance->getIdentifier() == "SP")
		scaleFactor = -scaleFactor;


	osg::ref_ptr<osg::Vec2Array> logVertices = new osg::Vec2Array;
	osg::ref_ptr<osg::Vec4Array> logColors = new osg::Vec4Array;
	osg::Vec4f oneColor;
	//holds order of drawing the vertices
	osg::ref_ptr<osg::DrawElementsUInt> logIndexArray = new osg::DrawElementsUInt(osg::PrimitiveSet::LINE_STRIP);
	
	//holds order of colors
	osg::ref_ptr<osg::TemplateIndexArray <unsigned int, osg::Array::UIntArrayType,4,4> > colorIndexArray;
	colorIndexArray = new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>;
	
	for(unsigned int i = 0; i < curveInstance->samples.size(); i++) {	
		logVertices->push_back(osg::Vec2d((curveInstance->samples[i] - minValue)* scaleFactor, -depthInMeters));
		logIndexArray->push_back(i);
		
		//set coloring
		if (curveInstance->getIdentifier() == "SP") {
			if(curveInstance->samples[i] > 0 ) { //color them yellow, for sands						
				oneColor[0] = 0.2f + 0.8f * (curveInstance->samples[i])/(maxValue);
				oneColor[1] = 0.2f + 0.8f * (curveInstance->samples[i])/(maxValue);
				oneColor[2] = 0.0f;
			} else {
				oneColor[0] = 0.2f + 0.3f * (curveInstance->samples[i] - minValue)/(-minValue);
				oneColor[1] = 0.2f + 0.3f * (curveInstance->samples[i] - minValue)/(-minValue);
				oneColor[2] = 0.2f + 0.3f * (curveInstance->samples[i] - minValue)/(-minValue);
			}

			//oneColor[0] = 0.8f;
			//oneColor[1] = 0.8f;
   //   oneColor[2] = 0.0f;
		} else { //ILD LOG		
			if (curveInstance->samples[i] > meanValue ) {				
				oneColor[0] = 0.0f;
				oneColor[1] = 0.3f + 0.7f * (curveInstance->samples[i] - meanValue)/(maxValue - meanValue);  
				oneColor[2] = 0.0f;
			} else {
				oneColor[0] = 0.4f - 0.4f * (curveInstance->samples[i] - minValue)/(meanValue - minValue);  
				oneColor[1] = 0.4f - 0.1f * (curveInstance->samples[i] - minValue)/(meanValue - minValue);  
				oneColor[2] = 0.4f - 0.1f * (curveInstance->samples[i] - minValue)/(meanValue - minValue);  
			}

			//oneColor[0] = 0.0f;
			//oneColor[1] = 0.8f;
   //   oneColor[2] = 0.8f;
		}
		oneColor[3] = 1.0f;

		logColors->push_back(oneColor);
		colorIndexArray->push_back(i);

		depthInMeters += intervalInMeters;
	}
	
	this->setVertexArray(logVertices.get());
	this->addPrimitiveSet(logIndexArray.get());

	this->setColorArray(logColors.get());
	//this->setColorIndices(colorIndexArray.get());
	this->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
}
