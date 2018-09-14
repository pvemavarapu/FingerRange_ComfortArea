//|___________________________________________________________________
//!
//! \file OverheadMapSel.h
//!
//! \brief "Selection" overhead map class.
//!
//! Author: Mark.
//|___________________________________________________________________

#ifndef OVERHEADMAPSEL_H
#define OVERHEADMAPSEL_H

//|___________________
//|
//| Local Includes
//|___________________

#include <string>
#include <vector>

#include <gmtl/gmtl.h>

#include <osg/MatrixTransform>
#include <osg/Geode>

#include "WellLog/WellDisplay.h"
#include "WellIcon.h"
#include "iPod/NetworkProtocol.h"

//|___________________________________________________________________
//|
//! "Selection" overhead map class.
//|___________________________________________________________________

class OverheadMapSel
{
public:
  //! Constructor.
  OverheadMapSel();

  //! Destructor.
  virtual ~OverheadMapSel() {}

  //! Gets overhead map root node.
  osg::Group* getRoot();

  //! Sets reference to well collection.
  void setWellCollection(const std::vector< osg::ref_ptr<WellDisplay> > *w_collection);

  //! Adds well icon to overhead map.
  void addWellIcon(const std::string &well_name);

  //! Gets number of well icons.
  int getWellIconNB();

  //! Clears active list.
  void clearActiveList();

  //! Computes {OverheadMap} wrt {Base} with frame projection.
  void computeOMapWRTBaseProjected(const gmtl::Matrix44f &c_wrt_b);

  //! Computes {OverheadMap} wrt {Base} without frame projection.
  void computeOMapWRTBaseFree(const gmtl::Matrix44f &c_wrt_b);

  //! Sets {Dataset} wrt {Base}.
  void setDatasetWRTBase(const gmtl::Matrix44f &d_wrt_b);

  //! Updates OverheadMap display.
  void updateOverheadMap();

  //! Sets marker position.
  void setMarkerPosition(const float x, const float y);

  void setTerrainCoordinates(gmtl::Vec4f coordVec);
  gmtl::Vec4f getTerrainCoordinates();

  //! Updates marker position by delta amount.
  void updateMarkerPositionDelta(const float dx, const float dy);

  //! Updates marker position by setting it directly.
  void updateMarkerPositionDirect(float x, float y);

  //! Resets marker position.
  void resetMarkerPosition();

  //! Gets list of active well-logs.
  void getActiveList(ChoiceSet &cset);

  //! Updates highlight.
  void updateHighlight();

  //! Highlights/Unhighlights closest well.
  void highlight(const bool highlight_status);

  //! Is OverheadMap displayed?
  bool isShow(); 

  //! Shows/hides OverheadMap.
  void show(const bool show_status); 

  //! Is marker displayed?
  bool isShowMarker(); 

  //! Shows/hides marker.
  void showMarker(const bool show_status); 

  //! Gets OverheadMap selection.
  int getSelection();

  //! Resets panning matrix to contain no translation. 
  void resetPanMatrix();

  //! Updates OverheadMap scale.
  void updateScale(const float ds);

  //! Is panning using border method?
  bool isPanningBorder(); 

  //! Adds well icon to active list.
  void addToActiveList(const int idx, const float x, const float z);

private:
  //! Creates geode containing overhead map frame.
  osg::Geode* createFrameNode();

  //! Creates marker nodes. 
  void createMarkerNodes();

  //! Sets overhead map position & scale.
  void setPositionScale(const float x, const float y, const float z, const float s);

  //! Gets marker position.
  void getMarkerPosition(float &x, float &y);

  //! Updates panning matrix using bump method.
  void updatePanMatrixBump(const int h, const int v);

  //! Updates panning matrix using border method.
  void updatePanMatrixBorder(const float x, const float y);

  osg::ref_ptr<osg::MatrixTransform> _omap_tf_nd;                 //!< Transform node. (Root)
  osg::ref_ptr<osg::MatrixTransform> _marker_tf_nd;               //!< Marker transform node.

  const std::vector< osg::ref_ptr<WellDisplay> > *_well_collection;    //!< Reference to well collection.

  std::vector< osg::ref_ptr<WellIcon> > _well_icon_collection;    //!< Well icon collection.
  std::vector< unsigned int > _active_list;                       //!< List of active well icon indices.

  gmtl::Matrix44f _o_wrt_b;                                       //!< {O}verheadMap wrt {B}ase.
  gmtl::Matrix44f _b_wrt_o;                                       //!< {B} wrt {O}.
  gmtl::Matrix44f _t_wrt_b;                                       //!< {T}errain wrt {B}.
  gmtl::Matrix44f _t_wrt_o;                                       //!< {T} wrt {O}.

  int _highlight_id;                                              //!< ID of hightlighted well icon.

  gmtl::Matrix44f _pan_matrix;                                    //!< Panning matrix.
  float _scale;                                                   //!< OverheadMap scale.

  gmtl::Vec4f terrainCoordinates;
};

#endif
