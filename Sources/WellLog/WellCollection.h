//|___________________________________________________________________
//!
//! \file WellCollection.h
//!
//! \brief Well collection class.
//!
//! Author: Phillip (modified by Mark).
//|___________________________________________________________________

#ifndef __WELLCOLLECTION_H
#define __WELLCOLLECTION_H

#pragma once

//|___________________
//|
//| Local Includes
//|___________________

#include <vector>
#include <string>
#include <boost/tokenizer.hpp>
#include "WellLogData.h"
#include "Utils/ToolBox.h"

using namespace std;
using namespace boost;

namespace well_meta_type {
	enum { API, SERIAL_NUMBER, OPERATOR, WELL_NAME, WELL_NUMBER, SURF_LAT, SURF_LON, KB, TD, TOWNSHIP, SPUD_DATE, PICK, UNDEFINED };
}

//|___________________________________________________________________
//|
//! Well collection class.
//|___________________________________________________________________

class CWellCollection {
private:
	vector <WellLogData*> well_logs;

public:
	CWellCollection(void);
	~CWellCollection(void);

	bool importFromCSV(string file_name);
	bool addWell(WellLogData *well_log);
	vector<WellLogData*> getWellLogsByAPI(string api);
	vector<WellLogData*> getWellLogsByName(string name);
	vector<WellLogData*> getWellLogsByPosition(double latitude, double longitude, double radius_in_meters);
};

#endif

