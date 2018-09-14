//|___________________________________________________________________
//!
//! \file OverheadMapSel.cpp
//!
//! \brief "Selection" overhead map class.
//!
//! Author: Mark.
//|___________________________________________________________________

//| Includes

#include <stdio.h>
#include <math.h>

#include <osg/Geometry>
#include <osg/StateSet>
#include <osg/LineWidth>

#include "OverheadMapSel.h"


//| Constants

const float DEFAULT_X        = 0.25f;                   // Default overhead map position
const float DEFAULT_Y        = -0.30f;
const float DEFAULT_Z        = 0.5f;
const float DEFAULT_S        = 0.4f;                   // Default overhead map scale

const float NORM_HALF_WIDTH  = 2;                      // Normalized overhead map half width
const float NORM_HALF_HEIGHT = 2;                      // Normalized overhead map half height

const float HALF_WIDTH       = 2.5f;                   // Half width&height of OverheadMap
const float HALF_HEIGHT      = 2.5f;
const float ORIGIN_OFFSET    = 4.0f;                   // Offset from ray start

const float MARKER_SIZE      = 0.025f;                 // Marker size

const float PAN_BORDER_WIDTH = 0.2f;                   // Width of overheadmap borders triggering panning when marker is inside
const float PAN_DELTA_AMOUNT = 0.03f;                  // Pan amount

const float MARKER_NOHIGHLIGHT_X = -100;
const float MARKER_NOHIGHLIGHT_Y = -100;

//| Function: OverheadMapSel
OverheadMapSel::OverheadMapSel(){ 
	// Inits data
	_highlight_id = -1;
	_pan_matrix.setState(gmtl::Matrix44f::TRANS);
	_scale = 1;

	// Root 
	_omap_tf_nd = new osg::MatrixTransform();
	setPositionScale(DEFAULT_X, DEFAULT_Y, DEFAULT_Z, DEFAULT_S);
	//osg::StateSet *state = _omap_tf_nd->getOrCreateStateSet();
	//state->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

	// Frame
	osg::Geode *frame_nd = createFrameNode();
	_omap_tf_nd->addChild( frame_nd );  

	// Marker
	createMarkerNodes();
	_omap_tf_nd->addChild( _marker_tf_nd.get() );  
	setMarkerPosition(MARKER_NOHIGHLIGHT_X, MARKER_NOHIGHLIGHT_Y);
}

//| Function: OverheadMapSel
osg::Group* OverheadMapSel::getRoot(){ 
	return _omap_tf_nd.get();
}

//| Function: OcreateFrameNode
osg::Geode* OverheadMapSel::createFrameNode(){
	osg::ref_ptr<osg::Geode> frame_nd = new osg::Geode;

	// Frame component
	{
		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		osg::ref_ptr<osg::Vec3Array> v   = new osg::Vec3Array;
		osg::ref_ptr<osg::Vec4Array> c   = new osg::Vec4Array;

		geom->setVertexArray( v.get() );
		v->push_back( osg::Vec3(-NORM_HALF_WIDTH, -NORM_HALF_HEIGHT, 0.0f) );
		v->push_back( osg::Vec3( NORM_HALF_WIDTH, -NORM_HALF_HEIGHT, 0.0f) );
		v->push_back( osg::Vec3( NORM_HALF_WIDTH,  NORM_HALF_HEIGHT, 0.0f) );
		v->push_back( osg::Vec3(-NORM_HALF_WIDTH,  NORM_HALF_HEIGHT, 0.0f) );

		geom->setColorArray( c.get() );
		geom->setColorBinding( osg::Geometry::BIND_OVERALL );
		c->push_back( osg::Vec4(1.f, 1.f, 1.f, 1.f) );

		geom->addPrimitiveSet( new osg::DrawArrays(GL_LINE_LOOP, 0, 4) );
		frame_nd->addDrawable( geom.get() );
	}

	// Ray component
	{
		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		osg::ref_ptr<osg::Vec3Array> v   = new osg::Vec3Array;
		osg::ref_ptr<osg::Vec4Array> c   = new osg::Vec4Array;

		geom->setVertexArray( v.get() );
		v->push_back( osg::Vec3(0.0f, -NORM_HALF_HEIGHT, 0.0f) );
		v->push_back( osg::Vec3(0.0f,  NORM_HALF_HEIGHT, 0.0f) );

		geom->setColorArray( c.get() );
		geom->setColorBinding( osg::Geometry::BIND_OVERALL );
		c->push_back( osg::Vec4(1.f, 1.f, 1.f, 1.f) );

		geom->addPrimitiveSet( new osg::DrawArrays(GL_LINES, 0, 2) );    
		frame_nd->addDrawable( geom.get() );
	}

	return frame_nd.release();
}

//| Function: OcreateMarkerNodes
void OverheadMapSel::createMarkerNodes(){  
	osg::ref_ptr<osg::Geode> marker_nd = new osg::Geode;
	osg::ref_ptr<osg::Geometry> geom   = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> v     = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> c     = new osg::Vec4Array;

	geom->setVertexArray( v.get() );
	v->push_back( osg::Vec3(-MARKER_SIZE, -MARKER_SIZE, 0.0f) );
	v->push_back( osg::Vec3( MARKER_SIZE,  MARKER_SIZE, 0.0f) );
	v->push_back( osg::Vec3(-MARKER_SIZE,  MARKER_SIZE, 0.0f) );
	v->push_back( osg::Vec3( MARKER_SIZE, -MARKER_SIZE, 0.0f) );

	geom->setColorArray( c.get() );
	geom->setColorBinding( osg::Geometry::BIND_OVERALL );
	c->push_back( osg::Vec4(1.f, 1.f, 0.f, 1.f) );

	geom->addPrimitiveSet( new osg::DrawArrays(GL_LINES, 0, 4) );
	marker_nd->addDrawable( geom.get() );
	osg::StateSet *state = marker_nd->getOrCreateStateSet();
	osg::LineWidth* lw   = new osg::LineWidth( 2.f );
	state->setAttribute( lw );

	_marker_tf_nd = new osg::MatrixTransform();
	_marker_tf_nd->addChild( marker_nd.get() );
	//_marker_tf_nd->setNodeMask(0x0);  // Hidden by default
}

//| Function: setPositionScale
void OverheadMapSel::setPositionScale(const float x, const float y, const float z, const float s){
	gmtl::Matrix44f mat;
	osg::Matrixf osg_m;

	// Sets x-y scale
	mat[0][0] = mat[1][1] = s;
	mat[2][2] = 0;              // HACK! FOR DRAWING CIRCLES
	// Sets position
	mat[0][3] = x;
	mat[1][3] = y;
	mat[2][3] = z;

	// Updates transform node
	osg_m.set( mat.getData() );
	_omap_tf_nd->setMatrix(osg_m);
}

//| Function: setWellCollection
void OverheadMapSel::setWellCollection(const std::vector< osg::ref_ptr<WellDisplay> > *w_collection){
	_well_collection = w_collection;
}

//| Function: addWellIcon
void OverheadMapSel::addWellIcon(const std::string &well_name){
	osg::ref_ptr<WellIcon> well_icon = new WellIcon(well_name);
	_omap_tf_nd->addChild( well_icon.get() );
	_well_icon_collection.push_back(well_icon);
}


//| Function: getWellIconNB
int OverheadMapSel::getWellIconNB(){
	return _well_icon_collection.size();
}

//| Function: clearActiveList
void OverheadMapSel::clearActiveList(){
	for(unsigned int i=0; i < _active_list.size(); i++) {
		_well_icon_collection[_active_list[i]]->show(false);
	}
	_active_list.clear();
}

//| Function: computeOMapWRTBaseProjected
void OverheadMapSel::computeOMapWRTBaseProjected(const gmtl::Matrix44f &c_wrt_b){
	gmtl::Vec3f y(c_wrt_b[0][1], c_wrt_b[1][1], 0);
	gmtl::normalize(y);

	// y' = Base-Y
	gmtl::Vec3f z(0, 0, 1);

	// Computes x', normalized, wrt {B}
	gmtl::Vec3f x;
	gmtl::cross(x, y, z);
	gmtl::normalize(x);

	// Computes p'', wrt {B}
	gmtl::Point3f p(c_wrt_b[0][3], c_wrt_b[1][3], c_wrt_b[2][3]);
	p = p - ORIGIN_OFFSET * gmtl::Vec3f(c_wrt_b[0][2], c_wrt_b[1][2], c_wrt_b[2][2]);
	
	//|
	//| Updates _o_wrt_b & _b_wrt_o

	// Translation
	_o_wrt_b[0][3] = p[0];
	_o_wrt_b[1][3] = p[1];
	_o_wrt_b[2][3] = 0;

	// X-Axis
	_o_wrt_b[0][0] = x[0];
	_o_wrt_b[1][0] = x[1];
	_o_wrt_b[2][0] = 0;

	// Y-Axis
	_o_wrt_b[0][1] =   -y[0];
	_o_wrt_b[1][1] =   -y[1];
	_o_wrt_b[2][1] =   0;

	// X-Axis
	_o_wrt_b[0][2] = 0;
	_o_wrt_b[1][2] = 0;
	_o_wrt_b[2][2] = 1;

	// Sets state
	_o_wrt_b.setState(gmtl::Matrix44f::AFFINE);

	// Inverts to get _b_wrt_o
	gmtl::invert(_b_wrt_o, _o_wrt_b);
}

//| Function: computeOMapWRTBaseFree
void OverheadMapSel::computeOMapWRTBaseFree(const gmtl::Matrix44f &c_wrt_b){
	// OverheadMap frame has the same rotation as Controller frame (wrt {B})
	_o_wrt_b = c_wrt_b;

	// Computes new origin p'', wrt {B}
	_o_wrt_b[0][3] = c_wrt_b[0][3] - ORIGIN_OFFSET*c_wrt_b[0][2];
	_o_wrt_b[1][3] = c_wrt_b[1][3] - ORIGIN_OFFSET*c_wrt_b[1][2];
	_o_wrt_b[2][3] = c_wrt_b[2][3] - ORIGIN_OFFSET*c_wrt_b[2][2];

	// Sets state
	_o_wrt_b.setState(gmtl::Matrix44f::AFFINE);

	// Inverts to get _b_wrt_o
	gmtl::invert(_b_wrt_o, _o_wrt_b);
}

//| Function: setDatasetWRTBase
void OverheadMapSel::setDatasetWRTBase(const gmtl::Matrix44f &d_wrt_b){
	_t_wrt_b = d_wrt_b;
}

//| Function: updateOverheadMap
void OverheadMapSel::updateOverheadMap(){    
	// Updates the map  
	if ( _active_list.size() == 0 ) {
		gmtl::Point3f p;                    // Position of well-log

		gmtl::Matrix44f pan_matrix_inv;     // Invert of panning matrix
		gmtl::invert(pan_matrix_inv, _pan_matrix);
		_t_wrt_o = pan_matrix_inv * _b_wrt_o * _t_wrt_b;

		for(unsigned int i=0; i<_well_collection->size(); i++) {
			if ((*_well_collection)[i]) {
				// Computes p
				(*_well_collection)[i]->getWellPosition(p[0], p[1], p[2]); // wrt {T}
				p = _t_wrt_o * p; // wrt {O}

				// Checks if falls within OverheadMap region
				if ( (p[0] >= -HALF_WIDTH * _scale) && (p[0] <= HALF_WIDTH * _scale) && (p[1] >= -HALF_HEIGHT * _scale) && (p[1] <= HALF_HEIGHT * _scale) ) {
					addToActiveList(i, p[0], p[1]);
				}
			}
		}
	}

	// Updates highlight
	updateHighlight();
}

//| Function: addToActiveList
void OverheadMapSel::addToActiveList(const int idx, const float x, const float z){
	if ( _well_icon_collection[idx]->isShow() == false ) { // Not yet in active list
		_active_list.push_back( idx );
		_well_icon_collection[idx]->show(true); // Shown if in active list

		float x_norm = NORM_HALF_WIDTH * x/(HALF_WIDTH*_scale);
		float y_norm = -NORM_HALF_HEIGHT * z/(HALF_HEIGHT*_scale);
		_well_icon_collection[idx]->setPosition(x_norm, y_norm);
	}
}

//| Function: setMarkerPosition
void OverheadMapSel::setMarkerPosition(const float x, const float y){
	osg::Matrix osg_m = osg::Matrix::translate(x, y, 0.0f);

	osg_m.makeTranslate( osg::Vec3(x, y, 0.0f) );
	std::cout << "2.1\n";
	//_marker_tf_nd->setMatrix(osg_m);
	
	float x_norm_inv = (HALF_WIDTH*_scale) * x/NORM_HALF_WIDTH;
	float y_norm_inv =  (HALF_HEIGHT*_scale) * y/NORM_HALF_HEIGHT;

	gmtl::Vec4f testVec;
	testVec = gmtl::Vec4f(x_norm_inv, 1.0f, -y_norm_inv, 1.0f);

	gmtl::Matrix44f _o_wrt_t;
	gmtl::invert(_o_wrt_t, _t_wrt_o);
	
	if(x != -100){
		testVec = _o_wrt_t * testVec;
		setTerrainCoordinates(testVec);
	}
}

//| Function: updateMarkerPositionDelta
void OverheadMapSel::updateMarkerPositionDelta(const float dx, const float dy){
	float x, y;
	int h=0, v=0;

	// Gets current position and computes new position
	getMarkerPosition(x, y);
	x += dx;
	y += dy;

	// Clipping
	if ( x < -NORM_HALF_WIDTH ) {
		x = -NORM_HALF_WIDTH;
		h = -1;
	} else
		if ( x >  NORM_HALF_WIDTH ) {
			x =  NORM_HALF_WIDTH;
			h = 1;
		}
		if ( y < -NORM_HALF_HEIGHT ) {
			y = -NORM_HALF_HEIGHT;
			v = 1;
		} else
			if ( y >  NORM_HALF_HEIGHT ) {
				y =  NORM_HALF_HEIGHT;
				v = -1;
			}

			// Updates scene graph
			setMarkerPosition(x, y);

			// Panning
			updatePanMatrixBump(h, v);
}

//| Function: updateMarkerPositionDirect
void OverheadMapSel::updateMarkerPositionDirect(float x, float y){
		// Clipping
  if ( x < -NORM_HALF_WIDTH ) {
    x = -NORM_HALF_WIDTH; 
  } else
  if ( x >  NORM_HALF_WIDTH ) {
    x =  NORM_HALF_WIDTH;
  }
  if ( y < -NORM_HALF_HEIGHT ) {
    y = -NORM_HALF_HEIGHT;
  } else
  if ( y >  NORM_HALF_HEIGHT ) {
    y =  NORM_HALF_HEIGHT;
  }

  // Updates scene graph
  setMarkerPosition(x, y);

  // Panning
  //updatePanMatrixBorder(x, y);
}

//| Function: resetMarkerPosition
void OverheadMapSel::resetMarkerPosition(){
	//updateMarkerPositionDirect(0, 0);
	setMarkerPosition(MARKER_NOHIGHLIGHT_X, MARKER_NOHIGHLIGHT_Y);
}

//| Function: getMarkerPosition
void OverheadMapSel::getMarkerPosition(float &x, float &y){
	osg::Matrix osg_m = _marker_tf_nd->getMatrix();
	osg::Vec3d pos = osg_m.getTrans();
	x = pos[0];
	y = pos[1];
}

//| Function: getActiveList
void OverheadMapSel::getActiveList(ChoiceSet &cset){
	float x, y;

	for(unsigned int i=0; i < _active_list.size(); i++) {    
		_well_icon_collection[_active_list[i]]->getPosition(x, y);
		cset.push_back( Choice(_active_list[i], (*_well_collection)[_active_list[i]]->getName(), "", "", x, -y) );
	}
}

//| Function: updateHighlight
void OverheadMapSel::updateHighlight(){
	// Unhighlights an old icon
	highlight(false);

	// Searches for a new icon closest to the marker
	float mx, my;
	getMarkerPosition(mx, my);

	if ( (mx == MARKER_NOHIGHLIGHT_X) || (my == MARKER_NOHIGHLIGHT_Y) ) {
		return;
	}

	float min_dist = 1000000.0f;
	float dist;
	float x, y;
	for(unsigned int i=0; i < _active_list.size(); i++) {
		_well_icon_collection[_active_list[i]]->getPosition(x, y);
		dist = sqrt( pow(x-mx, 2.0f) + pow(y-my, 2.0f) );
		if (dist <= min_dist) {
			min_dist      = dist;
			_highlight_id = _active_list[i];
		}
	}

	//std::cout << "HIGHLIGHT ID: " << _highlight_id << std::endl;

	// Highlights the new icon
	highlight(true);
}

//| Function: highlight
void OverheadMapSel::highlight(const bool highlight_status){
	if (highlight_status) {
		if ( _highlight_id >= 0 ) {
			_well_icon_collection[_highlight_id]->hightlight(true);
			(*_well_collection)[_highlight_id]->highlight(true); 
		}
	} else {
		if ( _highlight_id >= 0 ) {
			_well_icon_collection[_highlight_id]->hightlight(false);
			(*_well_collection)[_highlight_id]->highlight(false); 
			_highlight_id = -1;
		}
	}
}

//| Function: isShow
bool OverheadMapSel::isShow(){
	if ( _omap_tf_nd->getNodeMask() == 0xffffffff ) {
		return true;
	} else {
		return false;
	}
}

//| Function: show
void OverheadMapSel::show(const bool show_status){
	if (show_status) {
		_omap_tf_nd->setNodeMask(0xffffffff);
		showMarker(true);
	} else {
		_omap_tf_nd->setNodeMask(0x0);
		showMarker(false);
	}
}

//| Function: isShowMarker
bool OverheadMapSel::isShowMarker(){
	if ( _marker_tf_nd->getNodeMask() == 0xffffffff ) {
		return true;
	} else {
		return false;
	}
}

//| Function: showMarker
void OverheadMapSel::showMarker(const bool show_status){
	if (show_status) {
		_marker_tf_nd->setNodeMask(0xffffffff);
		highlight(true);
	} else {
		_marker_tf_nd->setNodeMask(0x0);
		highlight(false);
	}
}

//| Function: getSelection
int OverheadMapSel::getSelection(){
	return _highlight_id;
}

//| Function: updatePanMatrixBump
void OverheadMapSel::updatePanMatrixBump(const int h, const int v){
	// Horizontal pan
	_pan_matrix[0][3] += h*PAN_DELTA_AMOUNT;

	// Vertical pan
	_pan_matrix[2][3] += v*PAN_DELTA_AMOUNT;
}

//| Function: updatePanMatrixBorder
void OverheadMapSel::updatePanMatrixBorder(const float x, const float y){
	// Horizontal pan
	if (x < -NORM_HALF_WIDTH+PAN_BORDER_WIDTH) {
		_pan_matrix[0][3] -= PAN_DELTA_AMOUNT; // Left
	} else
		if (x >  NORM_HALF_WIDTH-PAN_BORDER_WIDTH) {
			_pan_matrix[0][3] += PAN_DELTA_AMOUNT; // Right
		}

		// Vertical pan
		if (y < -NORM_HALF_HEIGHT+PAN_BORDER_WIDTH) {
			_pan_matrix[2][3] += PAN_DELTA_AMOUNT; // Down
		} else
			if (y >  NORM_HALF_HEIGHT-PAN_BORDER_WIDTH) {
				_pan_matrix[2][3] -= PAN_DELTA_AMOUNT; // Up
			}
}

//| Function: resetPanMatrix
void OverheadMapSel::resetPanMatrix(){
	_pan_matrix[0][3] = _pan_matrix[2][3] = 0;
}

//| Function: updateScale
void OverheadMapSel::updateScale(const float ds){
	_scale += ds;
	if (_scale <= 0) {
		_scale = 0.000001f;
	}
}

//| Function: isPanningBorder
bool OverheadMapSel::isPanningBorder(){
	float x, y;

	// Gets marker position
	getMarkerPosition(x, y);

	// Checks if OverheadMap is panning
	if (x < -NORM_HALF_WIDTH+PAN_BORDER_WIDTH) {
		return true;   // Panning left
	} else
		if (x >  NORM_HALF_WIDTH-PAN_BORDER_WIDTH) {
			return true;   // Panning right
		} else
			if (y < -NORM_HALF_HEIGHT+PAN_BORDER_WIDTH) {
				return true;   // Panning down
			} else
				if (y >  NORM_HALF_HEIGHT-PAN_BORDER_WIDTH) {
					return true;   // Panning up
				} else {
					return false;  // No panning
				} 
}

void OverheadMapSel::setTerrainCoordinates(gmtl::Vec4f coordVec){
	terrainCoordinates = coordVec;

	std::cout << "TC: " << terrainCoordinates << std::endl;
}

gmtl::Vec4f OverheadMapSel::getTerrainCoordinates(){
	return terrainCoordinates;
}