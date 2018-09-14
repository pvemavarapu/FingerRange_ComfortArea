//|___________________________________________________________________
//!
//! \file WellLogHorizontalPane.cpp
//!
//! \brief Well-log horizontal pane class.
//!
//! Author: Phillip & Nathan (modified by Mark).
//|___________________________________________________________________

//|___________________
//|
//| Includes
//|___________________

#include <osg/Billboard>

#include "WellLogHorizontalPane.h"

//|____________________________________________________________________
//|
//| Function: WellLogHorizontalPane
//|
//! \param None.
//! \return None.
//!
//! Constructor. 
//|____________________________________________________________________

WellLogHorizontalPane::WellLogHorizontalPane(float size) : osg::Group()
{
	_transform = new osg::MatrixTransform;
	this->addChild(_transform.get());


	_plane = new osg::Box(osg::Vec3d(0.0f,0.0f,-0.1f),size * 2.0f,1.0f, 0.01f);
	_planeDrawable = new osg::ShapeDrawable(_plane);
  setColor(1.0f, 0.0f, 0.0f, 0.5f);
	osg::ref_ptr<osg::Billboard> planeBill = new osg::Billboard;
	planeBill->setMode(osg::Billboard::AXIAL_ROT);
	planeBill->setAxis(osg::Vec3f(0.0f,1.0f,0.0f));
	planeBill->setNormal(osg::Vec3f(0.0f,0.0f,1.0f));
	planeBill->addDrawable(_planeDrawable.get());

	_transform->addChild(planeBill.get());

	osg::ref_ptr<osg::StateSet> state = _transform->getOrCreateStateSet();
	state->setMode(GL_BLEND, osg::StateAttribute::ON);
  //state->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF );
  state->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

  // BUG? _height NOT INITIALIZED
	update();
}

//|____________________________________________________________________
//|
//| Function: ~WellLogHorizontalPane
//|
//! \param None.
//! \return None.
//!
//! Destructor. 
//|____________________________________________________________________

WellLogHorizontalPane::~WellLogHorizontalPane(void)
{}

//|____________________________________________________________________
//|
//| Function: setDepthMeters
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void WellLogHorizontalPane::setDepthMeters(float height)
{
	if (_plane.valid()) {
		_height = height;
		update();
	}
}

//|____________________________________________________________________
//|
//| Function: update
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void WellLogHorizontalPane::update()
{
	osg::Matrix translate;
	translate.makeTranslate(osg::Vec3f(0.0f, - 0.5f * _height, 0.0f));
	
	osg::Vec3f paneLengths = _plane->getHalfLengths();
	paneLengths.set(osg::Vec3f(paneLengths[0],_height/2,paneLengths[2]));
	_plane->setHalfLengths(paneLengths);
	
	
	_transform->setMatrix(translate);
}

//|____________________________________________________________________
//|
//| Function: setColor
//|
//! \param r      [in] Red component.
//! \param g      [in] Green component.
//! \param b      [in] Blue component.
//! \param a      [in] Alpha component.
//! \return None.
//!
//! Sets pane color.
//|____________________________________________________________________

void WellLogHorizontalPane::setColor(const float r, const float g, const float b, const float a)
{
  _planeDrawable->setColor( osg::Vec4f(r, g, b, a) );
}
