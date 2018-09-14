//|___________________________________________________________________
//!
//! \file WellLogHeader.cpp
//!
//! \brief Header information of a well log.
//!
//! The header information of a well log, containing mainly meta information
//! about the well and its location
//!
//! Author: Phillip (modified by Mark).
//|___________________________________________________________________

//|___________________
//|
//| Includes
//|___________________

#include "WellLogHeader.h"

//|____________________________________________________________________
//|
//| Function: CWellLogHeader
//|
//! \param None.
//! \return None.
//!
//! Constructor. 
//|____________________________________________________________________

CWellLogHeader::CWellLogHeader(void) 
{
	KbInMeters = TotalDepthInMeters = 0.0f;
	Latitude = Longitude = 0.0f;
}

//|____________________________________________________________________
//|
//| Function: ~CWellLogHeader
//|
//! \param None.
//! \return None.
//!
//! Destructor. 
//|____________________________________________________________________

CWellLogHeader::~CWellLogHeader(void) 
{}

//|____________________________________________________________________
//|
//| Function: getUniqueIdentifier
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

string CWellLogHeader::getUniqueIdentifier() 
{
	return "Well_" + API;
}

//|____________________________________________________________________
//|
//| Function: setKellyBushingInFeet
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void CWellLogHeader::setKellyBushingInFeet(double value)
{
	KbInMeters = value * 0.3048f;
}

//|____________________________________________________________________
//|
//| Function: getKellyBushingInMeters
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

double CWellLogHeader::getKellyBushingInMeters(void) // BUG? --> \ --> WHAT IS THIS???
{
	return KbInMeters;
}

//|____________________________________________________________________
//|
//| Function: setTotalDepthInFeet
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void CWellLogHeader::setTotalDepthInFeet(double value)
{
	TotalDepthInMeters = value * 0.3048f;
}

//|____________________________________________________________________
//|
//| Function: getTotalDepthInMeters
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

double CWellLogHeader::getTotalDepthInMeters(void)
{
	return TotalDepthInMeters;
}
