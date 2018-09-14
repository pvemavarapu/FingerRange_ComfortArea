//|___________________________________________________________________
//!
//! \file ToolBox.cpp
//!
//! \brief A collection of handy functions.
//!
//! Author: Phillip (modified by Mark).
//|___________________________________________________________________

//|___________________
//|
//| Includes
//|___________________

#include "ToolBox.h"

//|____________________________________________________________________
//|
//| Function: CToolBox
//|
//! \param None.
//! \return None.
//!
//! Constructor. 
//|____________________________________________________________________

CToolBox::CToolBox(void) 
{}

//|____________________________________________________________________
//|
//| Function: ~CToolBox
//|
//! \param None.
//! \return None.
//!
//! Destructor. 
//|____________________________________________________________________

CToolBox::~CToolBox(void) 
{}


//|____________________________________________________________________
//|
//| Function: itos
//|
//! \param None.
//! \return None.
//!
//! Converts int to string. 
//|____________________________________________________________________

string CToolBox::itos(int i) {
	stringstream s;
	s << i;
	return s.str();
}

/*
///
/// Clears the console output
///
void CToolBox::clearConsole() {
	#ifdef WIN32
		system("CLS");
	#else
		printf("\033[2J");
	#endif
}

///
/// Draws a coordinate system using unit length vectors
///
void CToolBox::drawCoordinateSystem() {
	glPushAttrib( GL_LIGHTING_BIT );

	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor4f(1.0,0.0,0.0,1.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(1.0,0.0,0.0);
		glColor4f(0.0,1.0,0.0,1.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,1.0,0.0);
		glColor4f(0.0,0.0,1.0,1.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,0.0,1.0);
	glEnd();

	glPopAttrib();
}

///
/// Draws a simple cube used for testing and debugging purposes
///
void CToolBox::drawCube() {
	drawbox(-0.1, 0.1, -0.1, 0.1, -0.1, 0.1, GL_QUADS);
}

///
/// Utility function for drawing a box using OpenGL calls
///
void CToolBox::drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1, GLdouble z0, GLdouble z1, GLenum type) {
   static GLdouble n[6][3] = {
      {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
   };
   static GLint faces[6][4] = {
      { 0, 1, 2, 3}, { 3, 2, 6, 7}, { 7, 6, 5, 4},
      { 4, 5, 1, 0}, { 5, 6, 2, 1}, { 7, 4, 0, 3}
   };
   GLdouble v[8][3], tmp;
   GLint i;

   if (x0 > x1) tmp = x0; x0 = x1; x1 = tmp;
   if (y0 > y1) tmp = y0; y0 = y1; y1 = tmp;
   if (z0 > z1) tmp = z0; z0 = z1; z1 = tmp;
   
   v[0][0] = v[1][0] = v[2][0] = v[3][0] = x0;
   v[4][0] = v[5][0] = v[6][0] = v[7][0] = x1;
   v[0][1] = v[1][1] = v[4][1] = v[5][1] = y0;
   v[2][1] = v[3][1] = v[6][1] = v[7][1] = y1;
   v[0][2] = v[3][2] = v[4][2] = v[7][2] = z0;
   v[1][2] = v[2][2] = v[5][2] = v[6][2] = z1;

   for (i = 0; i < 6; i++) {
      glBegin(type);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][0]][0]);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][1]][0]);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][2]][0]);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][3]][0]);
      glEnd();
   }
}
*/

//|____________________________________________________________________
//|
//| Function: getLengthOfDegreeOfLatitude
//|
//! \param latitude     [in] Latitude. (degree)
//! \return Length in meters of one degree latitude at a latitude "latitude".
//!
//! Calculates the length of one degree of latitude (in meters)
//! depending on a specified latitude value (in degrees)
//|____________________________________________________________________

double CToolBox::getLengthOfDegreeOfLatitude(double latitude){
	// Convert latitude to radians
	latitude = gmtl::Math::deg2Rad(latitude);

	// Set up constants
	double m1 = 111132.92;    // latitude calculation term 1
	double m2 = -559.82;      // latitude calculation term 2
	double m3 = 1.175;        // latitude calculation term 3
	double m4 = -0.0023;      // latitude calculation term 4

	// Calculate the length of a degree of latitude in meters
	return (m1 + (m2 * gmtl::Math::cos(2 * latitude)) + (m3 * gmtl::Math::cos(4 * latitude)) + (m4 * gmtl::Math::cos(6 * latitude)));
}

//|____________________________________________________________________
//|
//| Function: getLengthOfDegreeOfLongitude
//|
//! \param latitude     [in] Latitude. (degree)
//! \return Length in meters of one degree longitude at a latitude "latitude".
//!
//! Calculates the length of one degree of longitude (in meters)
//! depending on a specified latitude value (in degrees)
//|____________________________________________________________________

double CToolBox::getLengthOfDegreeOfLongitude(double latitude) 
{
  // Convert latitude to radians
  latitude = gmtl::Math::deg2Rad(latitude);

  // Set up constants
  double p1 = 111412.84;    // longitude calculation term 1
  double p2 = -93.5;        // longitude calculation term 2
  double p3 = 0.118;        // longitude calculation term 3

  // Calculate the length of a degree of longitude in meters
  return ((p1 * gmtl::Math::cos(latitude)) + (p2 * gmtl::Math::cos(3 * latitude)) + (p3 * gmtl::Math::cos(5 * latitude)));
}

/*
void CToolBox::drawPen(float bevel_d, float bevel_w, float bevel_intra) {
	GLUquadricObj *coneQuadric = gluNewQuadric();
	gluCylinder(coneQuadric, 0.0f, 1.0f, M_PHI, 20, 5);//draw cone

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, M_PHI);
		gluCylinder(coneQuadric, 1.0f, 1.0f, 6.0f, 20,5);//draw cylinder

		glTranslatef(0.0f, 0.0f, 6.0f);
		gluCylinder(coneQuadric, 1.0f, bevel_d, bevel_w, 20,5);//draw bevel(down)

		glTranslatef(0.0f, 0.0f, bevel_w);
		gluCylinder(coneQuadric, bevel_d, 1.0f, bevel_w, 20,5);//draw bevel(up)

		glTranslatef(0.0f, 0.0f, bevel_w);
		gluCylinder(coneQuadric, 1.0f, 1.0f, bevel_intra, 20,5);//draw bevel(shift)

		glTranslatef(0.0f, 0.0f, bevel_intra);
		gluCylinder(coneQuadric, 1.0f, bevel_d, bevel_w, 20,5);//draw bevel(down)

		glTranslatef(0.0f, 0.0f, bevel_w);
		gluCylinder(coneQuadric, bevel_d, 1.0f, bevel_w, 20,5);//draw bevel(up)

		glTranslatef(0.0f, 0.0f, bevel_w);
		gluCylinder(coneQuadric, 1.0f, 1.0f, bevel_intra, 20,5);//draw bevel(shift)

		glTranslatef(0.0f, 0.0f, bevel_intra);
		gluDisk(coneQuadric, 0.0f, 1.0f, 20, 2);//draw cap

	glPopMatrix();
}
*/

//SoNode * findFirst
//SoSearchAction sa;
//				sa.setType(SoDrawStyle::getClassTypeId());
//				sa.setInterest(SoSearchAction::ALL);
//				sa.setSearchingAll(true);
//				sa.apply(selectedNodes[i]);
//				SoPathList & pl = sa.getPaths();
//				cout << "found " << pl.getLength() << " draw style nodes." << endl;
//				for (int i = 0; i < pl.getLength(); i++) {
//					SoFullPath * p = (SoFullPath*) pl[i];
//					if (p->getTail()->isOfType(SoDrawStyle::getClassTypeId())) {
