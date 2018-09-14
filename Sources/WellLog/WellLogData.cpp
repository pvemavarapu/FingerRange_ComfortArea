////|___________________________________________________________________
////!
////! \file WellLogData.cpp
////!
////! \brief Holds all data for a single well.
////!
////! Unifies the header information of a well log with the data curves
////! and other data specific to the well
////!
////! Author: Phillip (modified by Mark).
////|___________________________________________________________________
//
////|___________________
////|
////| Includes
////|___________________
//
//#include "WellLogData.h"
//#include "WellLogReader.h"
//
////|____________________________________________________________________
////|
////| Function: WellLogData
////|
////! \param None.
////! \return None.
////!
////! Constructor. 
////|____________________________________________________________________
//
//WellLogData::WellLogData(void) 
//{
//	header = new CWellLogHeader();
//}
//
////|____________________________________________________________________
////|
////| Function: ~WellLogData
////|
////! \param None.
////! \return None.
////!
////! Destructor. 
////|____________________________________________________________________
//
//WellLogData::~WellLogData(void) 
//{
//	delete header;
//}
//
////|____________________________________________________________________
////|
////| Function: getHeader
////|
////! \param None.
////! \return None.
////!
////! ... 
////|____________________________________________________________________
//
//CWellLogHeader* WellLogData::getHeader() 
//{
//	return header;
//}
//
////|____________________________________________________________________
////|
////| Function: import
////|
////! \param None.
////! \return None.
////!
////! ... 
////|____________________________________________________________________
//
//bool WellLogData::import() 
//{
//	return import(header->API + ".las");
//}
//
////|____________________________________________________________________
////|
////| Function: import
////|
////! \param None.
////! \return None.
////!
////! ... 
////|____________________________________________________________________
//
//bool WellLogData::import(string file_name) 
//{
//	CWellLogReader* reader = CWellLogReader::getInstance();
//
//	if (!reader->readLASFile(file_name, this)) {
//		cout << "ERROR: could not import file '" + file_name + "'." << endl;
//		return false;
//	} else {
//		cout << "File " + file_name + " successfully imported." << endl << endl;
//		return true;
//	}
//}
//
////|____________________________________________________________________
////|
////| Function: addCurve
////|
////! \param None.
////! \return None.
////!
////! ... 
////|____________________________________________________________________
//
//CLogCurve* WellLogData::addCurve(string _identifier, string _description) 
//{
//	CLogCurve* newCurve = new CLogCurve(_identifier, _description);
//	curves.push_back( newCurve );
//	return newCurve;
//}
//
////|____________________________________________________________________
////|
////| Function: getCurveByID(
////|
////! \param None.
////! \return None.
////!
////! ... 
////|____________________________________________________________________
//
//CLogCurve* WellLogData::getCurveByID(string _identifier) 
//{
//	for (unsigned int i = 0; i < curves.size(); i++)
//		if (curves[i]->getIdentifier() == _identifier) 
//			return curves[i];
//	return 0;
//}
//
////|____________________________________________________________________
////|
////| Function: getNumberOfCurves
////|
////! \param None.
////! \return None.
////!
////! ... 
////|____________________________________________________________________
//
//int WellLogData::getNumberOfCurves() 
//{
//	return (int) curves.size();
//}
//
////|____________________________________________________________________
////|
////| Function: getNumberOfPicks
////|
////! \param None.
////! \return None.
////!
////! ... 
////|____________________________________________________________________
//
//int WellLogData::getNumberOfPicks(void) 
//{
//	return (int) picks.size();
//}
//
////|____________________________________________________________________
////|
////| Function: addPick
////|
////! \param None.
////! \return None.
////!
////! ... 
////|____________________________________________________________________
//
//void WellLogData::addPick(WellLogPick* pick) 
//{
//	picks.push_back(pick);
//}
//
////|____________________________________________________________________
////|
////| Function: getPicks
////|
////! \param None.
////! \return None.
////!
////! ... 
////|____________________________________________________________________
//
//vector<WellLogPick*> WellLogData::getPicks(void)
//{
//	return picks;
//}
