//|___________________________________________________________________
//!
//! \file WellLogSlider.cpp
//!
//! \brief Well-log slider class.
//!
//! Author: Phillip & Nathan (modified by Mark).
//|___________________________________________________________________

//|___________________
//|
//| Includes
//|___________________

#include <osg/Billboard>
#include <osg/Geometry>

#include "WellLogSlider.h"

//|____________________________________________________________________
//|
//| Function: WellLogSlider
//|
//! \param None.
//! \return None.
//!
//! Constructor. 
//|____________________________________________________________________

WellLogSlider::WellLogSlider(void) : osg::Group()
{	
	_depthInMeters = 0.0f;

	_transform = new osg::MatrixTransform;
	this->addChild(_transform.get());


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

	lineVertices->push_back(osg::Vec2d(-425.0f, 0.0f));
	lineVertices->push_back(osg::Vec2d(425.0f, 0.0f));
	lineIndexArray->push_back(0);
	lineIndexArray->push_back(1);

	lineColors->push_back(osg::Vec4f(1.0f,1.0f,1.0f,0.5f));
	colorIndexArray->push_back(0);

	
	lineGeometry->setVertexArray(lineVertices.get());
	lineGeometry->addPrimitiveSet(lineIndexArray.get());

	lineGeometry->setColorArray(lineColors.get());
	//lineGeometry->setColorIndices(colorIndexArray.get());
	lineGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	lineBill->addDrawable(lineGeometry.get());

	_transform->addChild(lineBill.get());
	
	_depthText = new osgText::Text;
	_depthText->setFont("");
	_depthText->setCharacterSize(50.0f);
	_depthText->setPosition(osg::Vec3f(450.0f,0.0f,0.0f));
	
	lineBill->addDrawable(_depthText.get());
	
	
	update();	
}

//|____________________________________________________________________
//|
//| Function: ~WellLogSlider
//|
//! \param None.
//! \return None.
//!
//! Destructor. 
//|____________________________________________________________________

WellLogSlider::~WellLogSlider(void)
{}

//|____________________________________________________________________
//|
//| Function: update
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void WellLogSlider::update()
{
	osg::Matrix translate;
	translate.makeTranslate(osg::Vec3f(0.0f, -_depthInMeters, 0.0f));
	_transform->setMatrix( translate );
	updateTextNode();
}

//|____________________________________________________________________
//|
//| Function: changeDepth
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void WellLogSlider::changeDepth(float delta)
{
	_depthInMeters += delta;

	if (_depthInMeters < 0.0f)
		_depthInMeters = 0.0f;

	update();
}

//|____________________________________________________________________
//|
//| Function: getDepthInMeters
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

float WellLogSlider::getDepthInMeters()
{
	return _depthInMeters;
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

void WellLogSlider::updateTextNode()
{
	if (_depthText == 0)
		return;

	char depth[30];
	sprintf(depth, "%5.1f ft", _depthInMeters * 3.2808399f);
	
	//_depthText->string.setValue(depth);
	_depthText->setText(depth);
}
