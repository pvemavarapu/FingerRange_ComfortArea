//|___________________________________________________________________
//!
//! \file ToolBox.h
//!
//! \brief A collection of handy functions.
//!
//! Author: Phillip (modified by Mark).
//|___________________________________________________________________

#ifndef __TOOLBOX_H
#define __TOOLBOX_H

#pragma once

//|___________________
//|
//| Local Includes
//|___________________

//#include <vrj/vrjConfig.h>
//#include <GL/glu.h>

#include <iostream>
#include <string>
#include <sstream>

#include <gmtl/Math.h>

//#define M_PHI          1.61803398874989484820f  /* phi, golden ratio = (1+sqrt(5))/2 */

using namespace std;

//|___________________________________________________________________
//|
//! A collection of handy functions.
//|___________________________________________________________________

class CToolBox {
  public:
    CToolBox(void);
    ~CToolBox(void);

    static string itos(int i);
    //static void clearConsole();
    //static void drawCoordinateSystem();
    //static void drawCube();
    //static void drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1, GLdouble z0, GLdouble z1, GLenum type);
    //static void drawPen(float bevel_d, float bevel_w, float bevel_intra);

    ///
    /// Converts a string to a templated numeric type
    ///
    template <class T> static bool from_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&)) {
	    std::istringstream iss(s); return !(iss >> f >> t).fail(); }

/*
    ///
    /// Deletes leading and trailing " in a string
    ///
    static string removeQuotationMarks(string input) {
	    string result(input);
	    if (result[0] == '\"') result.erase(0, 1);
	    int index = result.size() - 1;
	    if (index >= 0)
		    if (result[ index ] == '\"') result.erase( index, 1 );

	    return result;
    }
*/

    static double getLengthOfDegreeOfLatitude(double latitude);
    static double getLengthOfDegreeOfLongitude(double latitude);
};

#endif

