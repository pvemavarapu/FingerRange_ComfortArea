//|___________________________________________________________________
//!
//! \file WellLogPick.cpp
//!
//! \brief Well-log pick class.
//!
//! Author: Phillip & Nathan (modified by Mark).
//|___________________________________________________________________

//|___________________
//|
//| Includes
//|___________________

#include <osg/Matrix>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Billboard>

#include <boost/functional/hash.hpp>

#include "WellLogPick.h"

//|___________________
//|
//| Constants
//|___________________

const float PICKSIZE = 850.0f;

//|____________________________________________________________________
//|
//| Function: WellLogPick
//|
//! \param None.
//! \return None.
//!
//! Constructor. 
//|____________________________________________________________________

WellLogPick::WellLogPick(string name, float depthInMeters) 
{
	_name = name;
	_depthInMeters = depthInMeters;

	_displayNode = new osg::Group;
		
	//initilizing the transform
	_transformNode = new osg::MatrixTransform;
	osg::Matrix trans;
	trans.makeTranslate(osg::Vec3(0.0f,-_depthInMeters,0.0f));
	_transformNode->setMatrix(trans);
	
	_displayNode->addChild(_transformNode.get());

	// Create color based on name of the pick to have uniform colors for
	// the same horizon picks
	boost::hash<std::string> stringHash;

	string s = _name;
	reverse (s.begin(), s.end());


	int hash = stringHash(_name + s);
	float red = (hash % 256)  / 255.0f;
	float green = ((hash >> 2) % 256) / 255.0f;
	float blue = ((hash >> 4) % 256) / 255.0f;

	//color scaling, several colors coming out too dark
	if( (red + green + blue) < 0.1f) {
		float min = std::min(red, green);
		min = std::min(min, blue);
		
		red -= min;
		green -= min;
		blue -= min;

		
		float max = std::max(red, green);
		max = std::max(max, blue);

		if (max != 0.0f){		
		  red /= max;
		  green /= max;
		  blue /= max;
		} else {
			red = green = blue = 0.5f;
		}
	}
	
	//
	//Create line
	//
	osg::ref_ptr<osg::Billboard> lineBill = new osg::Billboard;
	lineBill->setMode(osg::Billboard::AXIAL_ROT);
	lineBill->setAxis(osg::Vec3f(0.0f,1.0f,0.0f));
	lineBill->setNormal(osg::Vec3f(0.0f,0.0f,1.0f));
	osg::ref_ptr<osg::Geometry> lineGeometry = new osg::Geometry;

	osg::ref_ptr<osg::Vec2Array> lineVertices = new osg::Vec2Array;
	osg::ref_ptr<osg::Vec4Array> lineColors = new osg::Vec4Array;
	//holds order of drawing the vertices
	osg::ref_ptr<osg::DrawElementsUInt> lineIndexArray = new osg::DrawElementsUInt(osg::PrimitiveSet::LINE_STRIP);

	//holds order of colors
	osg::ref_ptr<osg::TemplateIndexArray <unsigned int, osg::Array::UIntArrayType,4,4> > colorIndexArray;
	colorIndexArray = new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>;

	lineVertices->push_back(osg::Vec2d(-PICKSIZE/2, 0.0f));
	lineVertices->push_back(osg::Vec2d(PICKSIZE/2, 0.0f));
	lineIndexArray->push_back(0);
	lineIndexArray->push_back(1);

	lineColors->push_back(osg::Vec4f(red,green,blue,1.0f));
	colorIndexArray->push_back(0);

	
	lineGeometry->setVertexArray(lineVertices.get());
	lineGeometry->addPrimitiveSet(lineIndexArray.get());

	lineGeometry->setColorArray(lineColors.get());
	//lineGeometry->setColorIndices(colorIndexArray.get());
	lineGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	lineBill->addDrawable(lineGeometry.get());

	_transformNode->addChild(lineBill.get());

	//_displayNode->addChild(_transformNode.get()); // BUG? ALREADY ADDED


	//calculating feet
	char depth[20];
	sprintf(depth, "%5.1f ft", _depthInMeters * 3.2808399f);

	std::string depthString(depth);

	
	///
	//CREATE TEXT LABEL FOR LINE
	//

	_textNode = new osgText::Text;
	_textNode->setText(_name);
	_textNode->setColor(osg::Vec4(red,green,blue,1.0f));
	_textNode->setFont("");
	_textNode->setCharacterSize(50.0f);
	_textNode->setAlignment(osgText::Text::RIGHT_CENTER);
	_textNode->setPosition(osg::Vec3f(-PICKSIZE/2,0.0f,0.0f));
	lineBill->addDrawable(_textNode.get());

	
	_depthNode = new osgText::Text;
	_depthNode->setText(depthString);
	_depthNode->setColor(osg::Vec4(red,green,blue,1.0f));
	_depthNode->setFont("");
	_depthNode->setCharacterSize(50.0f);
	_depthNode->setAlignment(osgText::Text::LEFT_CENTER);
	_depthNode->setPosition(osg::Vec3f(PICKSIZE/2,0.0f,0.0f));
	lineBill->addDrawable(_depthNode.get());
}

//|____________________________________________________________________
//|
//| Function: ~WellLogPick
//|
//! \param None.
//! \return None.
//!
//! Destructor. 
//|____________________________________________________________________

WellLogPick::~WellLogPick(void) 
{}

//|____________________________________________________________________
//|
//| Function: getName
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

string WellLogPick::getName() 
{
	return _name;
}

//|____________________________________________________________________
//|
//| Function: getDepthMeters
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

float WellLogPick::getDepthMeters() 
{
	return _depthInMeters;
}

//|____________________________________________________________________
//|
//| Function: setDepthFeet
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void WellLogPick::setDepthFeet(float depth) 
{
	_depthInMeters = depth * 0.3048f;
	update();
}

//|____________________________________________________________________
//|
//| Function: setDepthMeters
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void WellLogPick::setDepthMeters(float depth) 
{
	_depthInMeters = depth;
	update();
}

//|____________________________________________________________________
//|
//| Function: setKbMeters
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void WellLogPick::setKbMeters(float kb) 
{
	_kbInMeters = kb;
	update();
}

//|____________________________________________________________________
//|
//| Function: setName
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void WellLogPick::setName(string name) 
{
	_name = name;
	updateTextNode();
}

//|____________________________________________________________________
//|
//| Function: getDisplayNode
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

osg::Group* WellLogPick::getDisplayNode(void)
{
	return _displayNode.get();
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

void WellLogPick::update()
{
	osg::Matrix newTranslate;
	newTranslate.makeTranslate(osg::Vec3f(0.0f,-_depthInMeters,0.0f));

	_transformNode->setMatrix(newTranslate);
	updateTextNode();
	
}

//|____________________________________________________________________
//|
//| Function: updateTextNode
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void WellLogPick::updateTextNode()
{
	char depth[20];
	sprintf(depth, "%5.1f ft", (_depthInMeters + _kbInMeters) * 3.2808399f);
	
	std::string depthString(depth);
	string completeString(_name + "\t\t" + depthString);

	_textNode->setText(_name);
	_depthNode->setText(depthString);
}
