////|___________________________________________________________________
////!
////! \file WellCollection.cpp
////!
////! \brief Well collection class.
////!
////! Author: Phillip (modified by Mark).
////|___________________________________________________________________
//
////|___________________
////|
////| Includes
////|___________________
//
//#include "WellCollection.h"
//#include "WellCollectionReader.h"
//
////|____________________________________________________________________
////|
////| Function: CWellCollection
////|
////! \param None.
////! \return None.
////!
////! Constructor
////|____________________________________________________________________
//
//CWellCollection::CWellCollection(void) 
//{}
//
////|____________________________________________________________________
////|
////| Function: ~CWellCollection
////|
////! \param None.
////! \return None.
////!
////! Destructor
////|____________________________________________________________________
//
//CWellCollection::~CWellCollection(void) 
//{}
//
////|____________________________________________________________________
////|
////| Function: addWell
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//bool CWellCollection::addWell(WellLogData *well_log) 
//{
//	well_logs.push_back( well_log );
//	//cout << "Added well " << well_log->getHeader()->API << ", " << well_log->getNumberOfPicks() << " picks." << endl;
//	return true;
//}
//
////|____________________________________________________________________
////|
////| Function: importFromCSV
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//bool CWellCollection::importFromCSV(string file_name) 
//{
//	CWellCollectionReader *reader = CWellCollectionReader::getInstance();
//
//	if (!reader->importFromCSV(file_name, this)) {
//		cout << "ERROR: could not import file '" + file_name + "'." << endl;
//		return false;
//	} else {
//		cout << "File " + file_name + " successfully imported." << endl;
//		cout << "Number of wells in collection: " << well_logs.size() << endl;
//		return true;
//	}
//}
//
////|____________________________________________________________________
////|
////| Function: getWellLogsByAPI
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//vector<WellLogData*> CWellCollection::getWellLogsByAPI(string api){
//	vector<WellLogData*> result;
//
//	WellLogData *log;
//	CWellLogHeader *header;
//
//	for (unsigned int i = 0; i < well_logs.size(); i++) {
//		log = (WellLogData *) well_logs[i];
//		header = log->getHeader();
//
//		string::size_type loc = header->API.find(api, 0);
//		if(loc != string::npos) result.push_back(log);
//	}
//
//	return result;
//}
//
////|____________________________________________________________________
////|
////| Function: getWellLogsByName
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//vector<WellLogData*> CWellCollection::getWellLogsByName(string name) 
//{
//	vector<WellLogData*> result;
//
//	WellLogData *log;
//	CWellLogHeader *header;
//
//	for (unsigned int i = 0; i < well_logs.size(); i++) {
//		log = (WellLogData *) well_logs[i];
//		header = log->getHeader();
//
//		string::size_type loc = header->WellName.find(name, 0);
//		if(loc != string::npos) result.push_back(log);
//	}
//
//	return result;
//}
//
////|____________________________________________________________________
////|
////| Function: getWellLogsByPosition
////|
////! \param None.
////! \return None.
////!
////! Returns all wells in the collection that are within a certain distance
////! from the specified geodetic position
////!
////! The distance calculation is only an approximation as the distance in m
////! that is represented by a change of one degree latitude / longitude is
////! considered constant. It will probably work only for fairly small radii.
////|____________________________________________________________________
//vector<WellLogData*> CWellCollection::getWellLogsByPosition(double latitude, double longitude, double radius_in_meters){
//	vector<WellLogData*> result;
//
//	WellLogData *log;
//	CWellLogHeader *header;
//
//	for (unsigned int i = 0; i < well_logs.size(); i++) {
//		log = (WellLogData *) well_logs[i];
//		header = log->getHeader();
//
//		double pos_lat = header->Latitude;
//		double pos_lon = header->Longitude;
//
//		double meter_per_deg_lat = CToolBox::getLengthOfDegreeOfLatitude(latitude);
//		double meter_per_deg_lon = CToolBox::getLengthOfDegreeOfLongitude(latitude);
//
//		// compute distance between well position and specified lat / long coordinate;
//		double distance = sqrt( pow(meter_per_deg_lat * (pos_lat - latitude), 2) + pow(meter_per_deg_lon * (pos_lon - longitude), 2) );
//		if (distance < radius_in_meters) result.push_back(log);
//	}
//
//	return result;
//}