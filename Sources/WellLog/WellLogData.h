//|___________________________________________________________________
//!
//! \file WellLogData.h
//!
//! \brief Holds all data for a single well.
//!
//! Unifies the header information of a well log with the data curves
//! and other data specific to the well
//!
//! Author: Phillip (modified by Mark).
//|___________________________________________________________________

#ifndef __WELLLOGDATA_H
#define __WELLLOGDATA_H

#pragma once

//|___________________
//|
//| Local Includes
//|___________________

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "WellLogHeader.h"
#include "WellLogPick.h"
#include "LogCurve.h"

using namespace std;

//|___________________________________________________________________
//|
//! Unifies the header information of a well log with the data curves
//! and other data specific to the well
//|___________________________________________________________________

class WellLogData
{
public:
	WellLogData(void);
	~WellLogData(void);

	CWellLogHeader* getHeader();
	bool import(string file_name);
	bool import(void);


	CLogCurve* addCurve(string _identifier, string _description);
	CLogCurve* getCurveByID(string _identifier);
	int getNumberOfCurves(void);
	
	void addPick(WellLogPick *pick);
	vector<WellLogPick*> getPicks(void);
	int getNumberOfPicks(void);

private:
	CWellLogHeader *header;	
	
	vector<CLogCurve*> curves;
	vector<WellLogPick*> picks;
};

#endif

