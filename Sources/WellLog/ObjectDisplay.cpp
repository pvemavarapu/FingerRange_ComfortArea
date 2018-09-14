//|___________________________________________________________________
//!
//! \file ObjectDisplay.cpp
//|___________________________________________________________________

//| Includes
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <sstream>

#include <gmtl/Vec.h>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Billboard>
#include <osg/Matrix>
#include <osg/LineSegment>
#include <osg/Depth>
#include <osg/Stencil>
#include <osg/LineWidth>
#include <osg/Material>
#include <osg/BlendFunc> 

#include "SelectionBox.h"

#include "SceneObjects\ObjectDisplay.h"
//#include "LogcurveDisplay.h"

//| Global Variables

//| Function: ObjectDisplay
ObjectDisplay::ObjectDisplay(){
	_isInitialized = false;
	_isSelected = false;

	_fade_Objects = false;
}

bool ObjectDisplay::createSceneObject(gmtl::Point3f size, gmtl::Vec4f color){
	//
	// Create UPPER SCENE GRAPH
	//
	_object_tf_nd = new osg::MatrixTransform();
	this->addChild(_object_tf_nd.get());

	std::cout << "0: " << size[0] << ", 1: " << size[1] << std::endl;
	//
	// Create The Rectangle
	//
	osg::ref_ptr<osg::Geode> wellMarker = new osg::Geode;
	osg::ref_ptr<osg::ShapeDrawable> wellShape = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), size[0], size[1], size[2]));

	osg::StateSet* state = wellMarker->getOrCreateStateSet();
	osg::ref_ptr<osg::Material> nodematerial = new osg::Material;

	state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	nodematerial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(color[0], color[1], color[2], 1.0f));
	nodematerial->setAlpha(osg::Material::FRONT_AND_BACK, 1.0f);
	osg::BlendFunc* bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
	state->setAttributeAndModes(bf);
	state->setAttribute(nodematerial.get());

	wellMarker->addDrawable(wellShape.get());

	osg::Matrixf osg_m;
	gmtl::Matrix44f tmat;

	gmtl::setTrans(tmat, gmtl::Vec3f(-1.0, 0.0, 0.0));
	osg_m.set(tmat.getData());
	_object_tf_nd->setMatrix(osg_m);
	_object_tf_nd->addChild(wellMarker.get());

	//osg_m = _object_tf_nd->getMatrix();
	//_object_tf_nd->setName("M");

	return true;
}

//| Function: ~ObjectDisplay
ObjectDisplay::~ObjectDisplay(void){
}

//| Function: showActiveObjects
void ObjectDisplay::showActiveObjects(const std::vector<osg::ref_ptr<ObjectDisplay>> *obj_collection){
	for(int i = 0; i < obj_collection->size(); i++){
		if((*obj_collection)[i]){
			(*obj_collection)[i]->makeOpaque();
		}
	}
}

void ObjectDisplay::resetObjects(const std::vector<osg::ref_ptr<ObjectDisplay>> *obj_collection){
	for(int i = 0; i < obj_collection->size(); i++)
		if((*obj_collection)[i])	
			(*obj_collection)[i]->makeTransparent();
}

void ObjectDisplay::makeTransparent(){
	_fade_Objects = true;
	osg::ref_ptr<osg::Node> _well_Marker = _object_tf_nd->getChild(0);
	osg::StateSet* state = _well_Marker->getOrCreateStateSet();
	
	osg::ref_ptr<osg::Material> nodematerial = new osg::Material;
	
	float f_color = 0.08f, a_color = 0.78f;
	
	if(a_color > f_color){
		a_color -= 0.1f;
		nodematerial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		nodematerial->setAlpha(osg::Material::FRONT_AND_BACK, 1.0f);

		state->setMode(GL_BLEND, osg::StateAttribute::ON);
		state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN); 
		state->setAttribute(nodematerial.get());

		return;
	}
	_fade_Objects = false;
}

void ObjectDisplay::makeOpaque(){
	osg::ref_ptr<osg::Node> _well_Marker = _object_tf_nd->getChild(0);
	osg::StateSet* state = _well_Marker->getOrCreateStateSet();
	osg::ref_ptr<osg::Material> nodematerial = new osg::Material;

	nodematerial->setShininess(osg::Material::FRONT_AND_BACK, 100.0);
	nodematerial->setSpecular(osg::Material::FRONT_AND_BACK,  osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	nodematerial->setDiffuse(osg::Material::FRONT_AND_BACK,osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//nodematerial->setDiffuse(osg::Material::FRONT_AND_BACK,osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	nodematerial->setAlpha(osg::Material::FRONT_AND_BACK, 1.0f);

	//state->setAttributeAndModes(bf);
	state->setAttribute(nodematerial.get());
}

bool ObjectDisplay::getIfFade(){
	return _fade_Objects;
}

//| Function: select
void ObjectDisplay::select(){
	_isSelected = true;
}

//| Function: unselect
void ObjectDisplay::unselect() {
	_isSelected = false;
}

//| Function: isInitialized
bool ObjectDisplay::isInitialized(){
	return _isInitialized;
}

//| Function: setLocationData
void ObjectDisplay::setLocationData(osg::Vec3 location){
	_location.set(location);
}

//| Function: setObjectPosition
void ObjectDisplay::setObjectPosition(const float x, const float y, const float z){	
	osg::Matrixf osg_m;
	gmtl::Matrix44f tmat;

	gmtl::setTrans(tmat, gmtl::Vec3f(x, y, z));
	osg_m.set( tmat.getData() );
	_object_tf_nd->setMatrix(osg_m);
}

void ObjectDisplay::setScale(const float x, const float y, const float z, const float tx, const float ty, const float tz){
	osg::Matrix matR, matS, matT, mat;

	matS.makeScale(osg::Vec3f(x, y, z));
	matR.identity();
	matT.makeTranslate(osg::Vec3(tx, ty, tz));

	mat = matR * matS * matT;

	_object_tf_nd->setMatrix(mat);
}

//| Function: getObjectPosition
void ObjectDisplay::getObjectPosition(float &x, float &y, float &z){
	osg::Matrix osg_m = _object_tf_nd->getMatrix();
	osg::Vec3d pos = osg_m.getTrans();
	x = pos[0];
	y = pos[1];
	z = pos[2];
}

//| Function: highlight
void ObjectDisplay::highlight(const bool highlight_status){
	//osg::ref_ptr<osg::Node> _well_Marker = _object_tf_nd->getChild(0);
	//osg::StateSet* state = _well_Marker->getOrCreateStateSet();
	//osg::ref_ptr<osg::Material> nodematerial = new osg::Material;

	//nodematerial->setShininess(osg::Material::FRONT_AND_BACK, 100.0);
	//nodematerial->setSpecular(osg::Material::FRONT_AND_BACK,  osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//nodematerial->setDiffuse(osg::Material::FRONT_AND_BACK,osg::Vec4(0.60f, 0.80f, 0.19f, 1.0f));
	//nodematerial->setAlpha(osg::Material::FRONT_AND_BACK, 1.0f);

	//if (highlight_status) {
	//	//nodematerial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.60f, 0.80f, 0.19f, 1.0f));
	//	 nodematerial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	//} else {
	//	nodematerial->setDiffuse(osg::Material::FRONT_AND_BACK,osg::Vec4(0.13f, 0.545f, 0.13f, 1.0f));
	//}
	//state->setAttribute(nodematerial.get());
	std::cout << "HIGHLIGHT" << std::endl;
	if (highlight_status)
		object_Outline->setNodeMask(0xffffffff);
	else
		object_Outline->setNodeMask(0x0);
}
