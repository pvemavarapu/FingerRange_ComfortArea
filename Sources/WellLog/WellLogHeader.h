//|___________________________________________________________________
//!
//! \file WellLogHeader.h
//!
//! \brief Header information of a well log.
//!
//! The header information of a well log, containing mainly meta information
//! about the well and its location
//!
//! Author: Phillip (modified by Mark).
//|___________________________________________________________________

#ifndef __WELLLOGHEADER_H
#define __WELLLOGHEADER_H
#pragma once

//|___________________
//|
//| Local Includes
//|___________________

#include <string>
#include <iostream>

using namespace std;

//|___________________________________________________________________
//|
//! The header information of a well log, containing mainly meta information
//! about the well and its location
//|___________________________________________________________________

class CWellLogHeader
{
public:
	// General meta data
	string API;
	string SerialNumber;
	string Operator;
	string WellName;
	string WellNumber;
	string Date;

	double Latitude;
	double Longitude;

	string Location;
	string Township;
	string County;
	string State;
	string Country;
	
	string Comments;
	
	// Not used as of now
	string Range;
	string Section;
	string DF;
	string GR;
    
	CWellLogHeader(void);
	~CWellLogHeader(void);

	string getUniqueIdentifier(void);
	void setKellyBushingInFeet(double value);
	double getKellyBushingInMeters(void);
	void setTotalDepthInFeet(double value);
	double getTotalDepthInMeters(void);

private:
	// Relevant for display
	double KbInMeters;	// elevation above sea level (KB) in feet
	double TotalDepthInMeters;
};

#endif

