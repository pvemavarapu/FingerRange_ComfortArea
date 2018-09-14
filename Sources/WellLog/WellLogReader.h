//|___________________________________________________________________
//!
//! \file WellLogReader.h
//!
//! \brief Reader class for LAS files.
//!
//! Reader class to import data from LAS 2.0 files into a CWellLogData object.
//! The class is implemented as a Singleton.
//!
//! Author: Phillip (modified by Mark).
//|___________________________________________________________________

#ifndef __WELLLOGREADER_H
#define __WELLLOGREADER_H
#pragma once

//|___________________
//|
//| Local Includes
//|___________________

#include <string>
#include <iostream>

#include "Utils/ToolBox.h"
#include "WellLogData.h"
#include "LogCurve.h"
#include "WellLogHeader.h"

// boost regexp included for intrepreting files using regular expressions
// tell Boost to not link the lib automatically because of version inconsistency with 
// Boost libraries in VRJuggler\dep\libs 
//#define BOOST_REGEX_NO_LIB
#include <boost/regex.hpp>

using namespace std;
//using namespace boost;

//|___________________________________________________________________
//|
//! Reader class to import data from LAS 2.0 files into a CWellLogData object.
//! The class is implemented as a Singleton.
//|___________________________________________________________________

class CWellLogReader
{
	int line_counter;
	string file_path;
	float sample_interval;
	float start_sample_position;
	vector<string> searchFolders;

private:
	CWellLogReader(void);
	static CWellLogReader *instance;
	vector<CLogCurve*> curves;
	void error(string msg);
	void warning(string msg);
	
public:
	static CWellLogReader* getInstance();
	~CWellLogReader(void);	

	void clearSearchFolders();
	void addSearchFolder(string path);
	bool readLASFile(string file_name, WellLogData *log_data);
	bool readLASVersionInformation(const char *line, WellLogData *log_data);
	bool readLASWellInformation(const char *line, WellLogData *log_data);
	bool readLASCurveInformation(const char *line, WellLogData *log_data);
	bool readLASParameters(const char *line, WellLogData *log_data);
	bool readLASLogDataHeader(const char *line, WellLogData *log_data);
	bool readLASLogData(const char *line, WellLogData *log_data);
	enum LAS_SECTION {VERSION_INFORMATION, WELL_INFORMATION, CURVE_INFORMATION, PARAMETERS, LOG_DATA, OTHER, NONE};
};

#endif

