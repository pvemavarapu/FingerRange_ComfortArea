//|___________________________________________________________________
//!
//! \file WellLogPick.h
//!
//! \brief Well-log pick class.
//!
//! Author: Phillip & Nathan (modified by Mark).
//|___________________________________________________________________

#ifndef __WELLLOGPICK_H
#define __WELLLOGPICK_H

#pragma once

//|___________________
//|
//| Local Includes
//|___________________

#include <string>

#include <osgText/Text>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Geometry>

#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>

using namespace std;
using namespace gmtl;

//|___________________________________________________________________
//|
//! Well-log pick class.
//|___________________________________________________________________

class WellLogPick 
{
public:
	WellLogPick(string name, float depthInMeters);
	~WellLogPick(void);

	string getName();
	float getDepthMeters();
	void setDepthMeters(float _depth);
	void setDepthFeet(float _depth);
	void setKbMeters(float kb);
	void setName(string _name);

	osg::Group* getDisplayNode(void);

private:
	void update(void);
	void updateTextNode(void);

	string _name;
	float _kbInMeters;
	float _depthInMeters;
	osg::ref_ptr<osg::Group> _displayNode;
	osg::ref_ptr<osg::MatrixTransform> _transformNode;
	osg::ref_ptr<osgText::Text> _textNode;
	osg::ref_ptr<osgText::Text> _depthNode;
	osg::ref_ptr<osg::Geometry> lineGeometry;
};

#endif

