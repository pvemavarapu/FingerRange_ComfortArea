//|___________________________________________________________________
//!
//! \file WellIcon.h
//!
//! \brief Overhead map "well icon" class.
//!
//! Author: Mark.
//|___________________________________________________________________

#ifndef WELLICON_H
#define WELLICON_H

//|___________________
//|
//| Local Includes
//|___________________

#include <string>

#include <osg/MatrixTransform>
#include <osgText/Text>
#include <osg/ShapeDrawable>

//|___________________________________________________________________
//|
//! Overhead map "well icon" class.
//|___________________________________________________________________

class WellIcon : public osg::MatrixTransform
{
public:
  //! Constructor.
  WellIcon(const std::string &well_name);

  //! Destructor.
  virtual ~WellIcon() {}

  //! Sets icon position.
  void setPosition(const float x, const float y);

  //! Gets icon position.
  void getPosition(float &x, float &y);

  //! Shows/hides icon.
  void show(const bool show_status); 

  //! Highlights/unhighlights icon.
  void hightlight(const bool highlight_status);

  //! Is icon displayed?
  bool isShow(); 

private:
  //! Creates text node.
  osg::Node* createTextNode(const std::string &well_name);  

  //! Creates marker node.
  osg::Node* createMarkerNode(); 

  osg::ref_ptr<osgText::Text> _text_geom;         //!< Text geometry.

  osg::ref_ptr<osg::ShapeDrawable> _marker_shape; //!< Marker shape.
};

#endif
