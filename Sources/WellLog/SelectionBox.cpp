//|___________________________________________________________________
//!
//! \file SelectionBox.cpp
//!
//! \brief Selection box class (WIP).
//!
//! Author: Nathan (modified by Mark).
//|___________________________________________________________________

//|___________________
//|
//| Includes
//|___________________

#include <iostream>

#include <osg/ShapeDrawable>
#include <osg/Material>

#include "SelectionBox.h"

using namespace std;

//|____________________________________________________________________
//|
//| Function: SelectionBox
//|
//! \param None.
//! \return None.
//!
//! Constructor
//|____________________________________________________________________

SelectionBox::SelectionBox() : osg::Geode()
{
	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
    hints->setDetailRatio(2.0f);

	osg::ref_ptr<osg::ShapeDrawable> boxShape = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 1.00f));
	osg::ref_ptr<osg::StateSet> state = boxShape->getOrCreateStateSet();
	state->setMode(GL_DEPTH_WRITEMASK,osg::StateAttribute::OFF);

	osg::ref_ptr<osg::ColorMask> colorMask = new osg::ColorMask;
	colorMask->setAlphaMask(false);
	colorMask->setBlueMask(false);
	colorMask->setGreenMask(false);
	colorMask->setRedMask(false);

	state->setAttributeAndModes(colorMask);
	this->addDrawable(boxShape.get());

	_stateSet = this->getOrCreateStateSet();
	_stateSet->setDataVariance(osg::Object::DYNAMIC);
}

//|____________________________________________________________________
//|
//| Function: select
//|
//! \param None.
//! \return None.
//!
//! ...
//|____________________________________________________________________

bool SelectionBox::select()
{	
	cout << "SELECT a box!\n";
	return true;
}

//|____________________________________________________________________
//|
//| Function: unselect
//|
//! \param None.
//! \return None.
//!
//! ...
//|____________________________________________________________________

bool SelectionBox::unselect()
{
	//cout << "UNSELECT a box!\n";
	return true;
}

//|____________________________________________________________________
//|
//| Function: highlight
//|
//! \param None.
//! \return None.
//!
//! ...
//|____________________________________________________________________

void SelectionBox::highlight()
{	
	//cout << "Highlight a box!\n";
}

