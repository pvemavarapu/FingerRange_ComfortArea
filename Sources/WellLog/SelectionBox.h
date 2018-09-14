//|___________________________________________________________________
//!
//! \file SelectionBox.h
//!
//! \brief Selection box class (WIP).
//!
//! Author: Nathan (modified by Mark).
//|___________________________________________________________________

#ifndef _SELECTION_BOX_H
#define _SELECTION_BOX_H

//|___________________
//|
//| Local Includes
//|___________________

#include <osg/Geode>
#include <osg/StateSet>

#include "Selectable.h"

//|___________________________________________________________________
//|
//! Example class to demonstrate how a class derived from
//! Selectable can implement custom behavior for highlighting,
//! selection, and deselection
//|___________________________________________________________________

class SelectionBox : public osg::Geode, public Selectable
{
public:
	SelectionBox(void);

	virtual bool select(void);
	virtual bool unselect(void);
	virtual void highlight(void);

protected:
	osg::ref_ptr<osg::StateSet> _stateSet;
};

#endif