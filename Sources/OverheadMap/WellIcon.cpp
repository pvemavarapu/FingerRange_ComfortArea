//|___________________________________________________________________
//!
//! \file WellIcon.cpp
//!
//! \brief Overhead map "well icon" class.
//!
//! Author: Mark.
//|___________________________________________________________________

//|___________________
//|
//| Includes
//|___________________

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Shape>
#include <osgText/Font>
#include <iostream>

#include "WellIcon.h"

//|___________________
//|
//| Constants
//|___________________

const float TEXT_SIZE   = 0.15f;          // Text size
const float MARKER_SIZE = 0.025f;         // Marker size
const bool  SHOW_STATUS = false;          // Initial show status

//|____________________________________________________________________
//|
//| Function: WellIcon
//|
//! \param well_name      [in] Well's name.
//! \return None.
//!
//! Constructor. 
//|____________________________________________________________________

WellIcon::WellIcon(const std::string &well_name)
{ 
  // Text
  osg::Node *text_root = createTextNode(well_name);
  this->addChild( text_root );

  // Marker
  osg::Node *marker_root = createMarkerNode();
  this->addChild( marker_root );

  // Sets status
  show(SHOW_STATUS);
}

//|____________________________________________________________________
//|
//| Function: createTextNode
//|
//! \param well_name      [in] Well's name.
//! \return Root of text nodes.
//!
//! Creates text nodes with string set to well_name. 
//|____________________________________________________________________

osg::Node* WellIcon::createTextNode(const std::string &well_name){
  _text_geom = new osgText::Text;
  _text_geom->setText(well_name);
  _text_geom->setColor( osg::Vec4f(0.0f, 1.0f, 0.0f, 1.0f) );
  _text_geom->setFont("");
  _text_geom->setCharacterSize(TEXT_SIZE);
  _text_geom->setAlignment(osgText::Text::CENTER_BOTTOM);

  osg::ref_ptr<osg::Geode> text_nd = new osg::Geode;
  text_nd->addDrawable( _text_geom.get() );

  osg::ref_ptr<osg::MatrixTransform> text_tf_nd = new osg::MatrixTransform;
  osg::Matrix osg_m;
  osg_m.makeTranslate( osg::Vec3(0.0f, MARKER_SIZE, 0.0f) );
  text_tf_nd->setMatrix(osg_m);
  text_tf_nd->addChild( text_nd.get() );

  return text_tf_nd.release();
}

//|____________________________________________________________________
//|
//| Function: createMarkerNode
//|
//! \param None.
//! \return Root of marker nodes.
//!
//! Creates marker nodes. 
//|____________________________________________________________________

osg::Node* WellIcon::createMarkerNode()
{  
  _marker_shape = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), MARKER_SIZE));
  _marker_shape->setColor( osg::Vec4f(0.8f, 0.1f, 0.1f, 1.0f) );

  osg::ref_ptr<osg::Geode> marker_nd = new osg::Geode;
  marker_nd->addDrawable( _marker_shape.get() );

  return marker_nd.release();
}

//|____________________________________________________________________
//|
//| Function: setPosition
//|
//! \param x      [in] Icon's x position.
//! \param y      [in] Icon's y position.
//! \return None.
//!
//! Sets icon x-y position. 
//|____________________________________________________________________

void WellIcon::setPosition(const float x, const float y)
{
  osg::Matrix osg_m;
  osg_m.makeTranslate( osg::Vec3(x, y, 0.0f) );
  this->setMatrix(osg_m);
}

//|____________________________________________________________________
//|
//| Function: getPosition
//|
//! \param x      [out] Icon's x position.
//! \param y      [out] Icon's y position.
//! \return None.
//!
//! Gets icon x-y position. 
//|____________________________________________________________________

void WellIcon::getPosition(float &x, float &y)
{
  osg::Matrix osg_m = this->getMatrix();
  osg::Vec3d pos = osg_m.getTrans();
  x = pos[0];
  y = pos[1];
}

//|____________________________________________________________________
//|
//| Function: show
//|
//! \param show_status      [in] Status ("true" = show).
//! \return None.
//!
//! Shows/hides icon. 
//|____________________________________________________________________

void WellIcon::show(const bool show_status)
{
  if (show_status) {
    this->setNodeMask(0xffffffff);
  } else {
    this->setNodeMask(0x0);
  }
}

//|____________________________________________________________________
//|
//| Function: isShow
//|
//! \param None.
//! \return "true" if displayed.
//!
//! Is icon displayed? Determined by the current NodeMask value.
//|____________________________________________________________________

bool WellIcon::isShow()
{
  if (this->getNodeMask() == 0xffffffff) {
    return true;
  } else {
    return false;
  }
}

//|____________________________________________________________________
//|
//| Function: hightlight
//|
//! \param highlight_status     [in] Status ("true" = highlight).
//! \return None.
//!
//! Highlights/unhighlights icon.
//|____________________________________________________________________

void WellIcon::hightlight(const bool highlight_status){
  if (highlight_status) {
    _text_geom->setColor( osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f) );
    _marker_shape->setColor( osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f) );
	//_marker_shape->setColor( osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f) );
  } else {
    _text_geom->setColor( osg::Vec4f(0.0f, 1.0f, 0.0f, 1.0f) );
    _marker_shape->setColor( osg::Vec4f(0.8f, 0.1f, 0.1f, 1.0f) );
  }
} 