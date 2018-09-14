//|___________________________________________________________________
//!
//! \file Selectable.h
//!
//! \brief Selectable class.
//!
//! Author: Kaushik (modified by Mark).
//|___________________________________________________________________

#ifndef _SELECTABLE_H
#define _SELECTABLE_H

//|___________________
//|
//| Local Includes
//|___________________

#include <osg/Node>

//|___________________________________________________________________
//|
//! This abstract class interface is used in SelectionManager to perform
//! custom operations on selected / highlighted nodes. In order to implement
//! that type of behavior, have your class inherit from osg::Node and Selectable.
//! If you tell the SelectionManager that an instance of your class is selectable,
//! it will try to cast it to a Selectable class and call the respective virtual
//! methods on it.
//|___________________________________________________________________

class Selectable
{
public:
	Selectable(void) {};

	// If select() returns false, selection of this node should not be possible
	virtual bool select(void) = 0;

	// If unselect() returns false, the selection of this node should not be altered
	virtual bool unselect(void) = 0;

	virtual void highlight(void) {};
};

#endif