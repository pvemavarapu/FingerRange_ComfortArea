//|___________________________________________________________________
//!
//! \file WellLogSlider.h
//!
//! \brief Well-log slider class.
//!
//! Author: Phillip & Nathan (modified by Mark).
//|___________________________________________________________________

#pragma once

//|___________________
//|
//| Local Includes
//|___________________

#include <osg/Group>
#include <osgText/Text>
#include <osg/MatrixTransform>

//|___________________________________________________________________
//|
//! Well-log slider class.
//|___________________________________________________________________

class WellLogSlider : public osg::Group
{
public:
	WellLogSlider(void);
	~WellLogSlider(void);

	void changeDepth(float delta);
	float getDepthInMeters();

private:
	void update(void);
	void updateTextNode(void);

	float _depthInMeters;

	osg::ref_ptr<osg::MatrixTransform> _transform;
	osg::ref_ptr<osgText::Text> _depthText;
};
