////|___________________________________________________________________
////!
////! \file WellCollectionReader.cpp
////!
////! \brief Well collection reader class.
////!
////! Author: Phillip (modified by Mark).
////|___________________________________________________________________
//
////|___________________
////|
////| Includes
////|___________________
//
//#include "WellCollectionReader.h"
//
////|___________________
////|
////| Static Initializers
////|___________________
//
//int CWellCollectionReader::line_counter = 0;
//string CWellCollectionReader::file_path = "";
//CWellCollectionReader *CWellCollectionReader::instance = 0;
//
////|____________________________________________________________________
////|
////| Function: getInstance
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//CWellCollectionReader* CWellCollectionReader::getInstance() 
//{
//	if (instance == 0) {
//		instance = new CWellCollectionReader();
//	}
//	return instance;
//}
//
////|____________________________________________________________________
////|
////| Function: CWellCollectionReader
////|
////! \param None.
////! \return None.
////!
////! Constructor
////|____________________________________________________________________
//
//CWellCollectionReader::CWellCollectionReader(void) 
//{
//	line_counter = 0;
//}
//
////|____________________________________________________________________
////|
////| Function: ~CWellCollectionReader
////|
////! \param None.
////! \return None.
////!
////! Destructor
////|____________________________________________________________________
//
//CWellCollectionReader::~CWellCollectionReader(void) 
//{}
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
//bool CWellCollectionReader::importFromCSV(string file_name, CWellCollection * collection) 
//{
//
//	line_counter = 0;
//	// set static file name to enable access for error() procedure
//	file_path = file_name;
//
//	FILE *file = fopen(file_name.c_str(), "rb");
//
//	if (file != NULL) {
//		// line input buffers
//		char line[1200];
//		    
//		fgets(line, 1200, file);
//		line_counter++;
//
//    if (ferror(file)) { fclose(file); return false; } // BUG? FORGET TO CLOSE FILE, ADDED fclose() BY MARK
//
//		int mapping[100];
//		string pick_name[100];
//
//		string line_string(line);
//
//		// get mapping of data values from first line
//		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
//		boost::char_separator<char> seperator(",", "", boost::keep_empty_tokens);
//		tokenizer tokens(line_string, seperator);
//		tokenizer::iterator tok_iter = tokens.begin();
//
//		int i = 0;
//		for (tok_iter;	tok_iter != tokens.end(); ++tok_iter) {
//			string token = (string) *tok_iter;
//			token = removeQuotationMarks(token);
//			
//			if (token == "API") mapping[i] = well_meta_type::API;
//			else if (token == "SERIAL_NUMBER") mapping[i] = well_meta_type::SERIAL_NUMBER;
//			else if (token == "OPERATOR") mapping[i] = well_meta_type::OPERATOR;
//			else if (token == "WELL NAME") mapping[i] = well_meta_type::WELL_NAME;
//			else if (token == "WELL NUMBER") mapping[i] = well_meta_type::WELL_NUMBER;
//			else if (token == "SURF LAT") mapping[i] = well_meta_type::SURF_LAT;
//			else if (token == "SURF LON") mapping[i] = well_meta_type::SURF_LON;
//			else if (token == "KB") mapping[i] = well_meta_type::KB;
//			else if (token == "TD") mapping[i] = well_meta_type::TD;
//			else if (token == "TOWNSHIP") mapping[i] = well_meta_type::TOWNSHIP;
//			else if (token == "SPUD DATE") mapping[i] = well_meta_type::SPUD_DATE;
//			else if (token == "RANGE") mapping[i] = well_meta_type::UNDEFINED;
//			else if (token == "SECTION") mapping[i] = well_meta_type::UNDEFINED;
//			else if (token == "DF") mapping[i] = well_meta_type::UNDEFINED;
//			else if (token == "GR") mapping[i] = well_meta_type::UNDEFINED;
//			else {
//				// all columns right of column 15
//				if (i > 14) {
//					mapping[i] = well_meta_type::PICK;
//					pick_name[i] = token;
//				} else {
//					warning("Unknown field found in column " + CToolBox::itos(i) + ": '" + token + "'. Ignored.");
//					mapping[i] = well_meta_type::UNDEFINED;
//				}
//			}
//
//			i++;
//		}
//
//		WellLogData *log;
//		CWellLogHeader *header;
//
//		WellLogPick *pick;
//		string depth_string;
//
//		// read the whole file line per line
//		while (!feof(file)) {
//			fgets(line, 1200, file);
//			line_counter++;
//
//			if (!ferror(file)) {
//				// skip line if it is empty
//				if (line[0] == '\n') continue;
//				
//				string line_string(line);
//
//				// create new well log instance and get the header object
//				// that is going to hold meta information about the well
//				log = new WellLogData();
//				header = log->getHeader();
//
//				typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
//				boost::char_separator<char> seperator(",", "", boost::keep_empty_tokens);
//				tokenizer tokens(line_string, seperator);
//				tokenizer::iterator tok_iter = tokens.begin();
//				
//				int i = 0;
//
//				for (tok_iter;	tok_iter != tokens.end(); ++tok_iter) {
//					switch(mapping[i]) {
//						case well_meta_type::API:
//							header->API = removeQuotationMarks(*tok_iter);
//							break;
//						case well_meta_type::SERIAL_NUMBER:
//							header->SerialNumber = removeQuotationMarks(*tok_iter);
//							break;
//						case well_meta_type::OPERATOR:
//							header->Operator = removeQuotationMarks(*tok_iter);
//							break;
//						case well_meta_type::WELL_NAME:
//							header->WellName = removeQuotationMarks(*tok_iter);
//							break;
//						case well_meta_type::KB:
//							double kbInFeet;
//							if (!CToolBox::from_string<double>(kbInFeet, *tok_iter, std::dec))
//								warning("Could not convert KB to float value: " + *tok_iter);
//							else 
//								header->setKellyBushingInFeet(kbInFeet);
//							break;
//						case well_meta_type::TD:
//							double depthInFeet;
//							if (!CToolBox::from_string<double>(depthInFeet, *tok_iter, std::dec))
//								warning("Could not convert TD to float value: " + *tok_iter);
//							else 
//								header->setTotalDepthInFeet(depthInFeet);
//							break;
//					case well_meta_type::PICK:
//							depth_string = *tok_iter;
//							if (!depth_string.empty()) {
//								double depth;
//								if (!CToolBox::from_string<double>(depth, *tok_iter, std::dec)) {
//									warning("Could not convert pick depth to float value. Pick ignored.");
//								} else {
//									pick = new WellLogPick( pick_name[i], 0.0f);
//									pick->setDepthFeet(depth);
//									log->addPick(pick);							
//								}
//							}
//							break;
//
//						case well_meta_type::SURF_LAT:
//							double latitude;
//							if (!CToolBox::from_string<double>(latitude, *tok_iter, std::dec))
//								warning("Could not convert SURF LAT to float value: " + *tok_iter);
//							else 
//								header->Latitude = latitude;
//							break;
//						case well_meta_type::SURF_LON:
//							double longitude;
//							if (!CToolBox::from_string<double>(longitude, *tok_iter, std::dec))
//								warning("Could not convert SURF LON to float value: " + *tok_iter);
//							else 
//								header->Longitude = -longitude; // HACK!!! Values in CSV file have neg sign
//							break;
//						default:
//							break;
//					}
//					i++;
//				}
//
//				// add imported well log to the collection
//				collection->addWell(log);
//			}
//		}
//		fclose(file);
//		return true;
//	} else {
//		error("File could not be opened.");
//		return false;
//	}	
//}
//
////|____________________________________________________________________
////|
////| Function: error
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//void CWellCollectionReader::error(string msg) 
//{
//	cout << "ERROR [" + file_path + ", " + CToolBox::itos(line_counter) + "]: " + msg << endl;
//}
//
////|____________________________________________________________________
////|
////| Function: warning
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//void CWellCollectionReader::warning(string msg) 
//{
//	cout << "WARNING [" + file_path + ", " + CToolBox::itos(line_counter) + "]: " + msg << endl;
//}
//
////|____________________________________________________________________
////|
////| Function: removeQuotationMarks(
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//string CWellCollectionReader::removeQuotationMarks(string input)
//{
//		string result(input);
//		if (result[0] == '\"') result.erase(0, 1);
//		int index = result.size() - 1;
//		if (index >= 0)
//			if (result[ index ] == '\"') result.erase( index, 1 );
//
//		return result;	
//}
