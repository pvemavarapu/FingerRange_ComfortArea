//|___________________________________________________________________
//!
//! \file WellCollectionReader.h
//!
//! \brief Well collection reader class.
//!
//! Author: Phillip (modified by Mark).
//|___________________________________________________________________

#ifndef __WELLCOLLECTIONREADER_H
#define __WELLCOLLECTIONREADER_H
#pragma once

//|___________________
//|
//| Local Includes
//|___________________

#include <string>
#include "WellCollection.h"
#include "WellLogPick.h"

using namespace std;

//|___________________________________________________________________
//|
//! Well collection reader class.
//|___________________________________________________________________

class CWellCollectionReader {
	static int line_counter;
	static string file_path;
private:
	CWellCollectionReader(void);
	static CWellCollectionReader *instance;
	void error(string msg);
	void warning(string msg);
public:
	static CWellCollectionReader* getInstance();
	~CWellCollectionReader(void);
	bool importFromCSV(string file_name, CWellCollection *collection);
	
	/// Deletes leading and trailing " in a string
	static string removeQuotationMarks(string input); 
};

#endif