//|___________________________________________________________________
//!
//! \file ObjectDisplay.h
//!
//|___________________________________________________________________

#pragma once

#ifndef OBJECT_DISPLAY
#define OBJECT_DISPLAY

//|___________________
//|
//| Local Includes
//|___________________

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/Generate.h>

#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/AutoTransform>
#include <osg/Geode>
#include <osg/Stencil>
#include <osg/BlendFunc>
#include <osg/BlendColor>
#include <osg/PolygonMode>

#include <osg/ShapeDrawable>

#include <osgFX/Outline>

using namespace gmtl;
using namespace std;

//|___________________________________________________________________
//|
//! Object display class (WIP).
//|___________________________________________________________________

class ObjectDisplay : public osg::Group
{
public:
	ObjectDisplay();
	//static void initClass(void);

	bool initObjectDisplay(float radius, const int id);

	bool createSceneObject(gmtl::Point3f size, gmtl::Vec4f color);

	bool isInitialized(void);

	void select();
	void unselect();

	void showHighlight();
	void hideHighlight();

	void setLocationData(osg::Vec3 location);
	osg::Vec3 getLocationData();

	//! Sets object position.
	void setObjectPosition(const float x, const float y, const float z);

	void setScale(const float x, const float y, const float z, const float tx, const float ty, const float tz);

	//! Gets object position.
	void getObjectPosition(float &x, float &y, float &z);

	//! Highlights/Unhighlights object.
	void highlight(const bool highlight_status);

	void highlight(const bool highlight_status, int highlight_id);

	//! Selects/Unselects object. (overloaded)
	void select(const bool select_status);

	//pvv
	void showActiveObjects(const std::vector< osg::ref_ptr<ObjectDisplay> > *obj_collection);

	//pvv
	void resetObjects(const std::vector< osg::ref_ptr<ObjectDisplay> > *obj_collection);

	//pvv
	void makeTransparent();

	void makeOpaque();

	bool getIfFade();

protected:
	virtual ~ObjectDisplay(void);

private:
	int _id; //log numeric id
	osg::Vec3 _location;

	bool _isInitialized;
	bool _isSelected;
	bool _isCurveDisplayed;
	
	osg::ref_ptr<osg::ShapeDrawable> object_Outline;
	
	// Mainly to position objects
	osg::ref_ptr<osg::MatrixTransform> _object_tf_nd;

	bool _fade_Objects;
};

#endif