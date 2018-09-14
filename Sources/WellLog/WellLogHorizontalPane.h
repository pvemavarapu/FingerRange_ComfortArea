//|___________________________________________________________________
//!
//! \file WellLogHorizontalPane.h
//!
//! \brief Well-log horizontal pane class.
//!
//! Author: Phillip & Nathan (modified by Mark).
//|___________________________________________________________________

#pragma once

//|___________________
//|
//| Local Includes
//|___________________

#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>

//|___________________________________________________________________
//|
//! Well-log horizontal pane class.
//|___________________________________________________________________

class WellLogHorizontalPane : public osg::Group
{
public:
	WellLogHorizontalPane(float size);
	~WellLogHorizontalPane(void);
	void setDepthMeters(float height);
  
  //! Sets pane color.
  void setColor(const float r, const float g, const float b, const float a);

private:
	void update();

	osg::ref_ptr<osg::MatrixTransform> _transform;
	osg::ref_ptr<osg::Box> _plane;

  osg::ref_ptr<osg::ShapeDrawable> _planeDrawable;

	float _height;
};
