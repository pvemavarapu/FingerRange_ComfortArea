////|___________________________________________________________________
////!
////! \file WellLogReader.cpp
////!
////! \brief Reader class for LAS files.
////!
////! Reader class to import data from LAS 2.0 files into a CWellLogData object.
////! The class is implemented as a Singleton.
////!
////! Author: Phillip (modified by Mark).
////|___________________________________________________________________
//
////|___________________
////|
////| Includes
////|___________________
//
//#include "WellLogReader.h"
//
////|___________________
////|
////| Static Initializers???
////|___________________
//
//CWellLogReader *CWellLogReader::instance = 0;
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
//CWellLogReader* CWellLogReader::getInstance() 
//{
//	if (instance == 0)
//		instance = new CWellLogReader();
//	return instance;
//}
//
////|____________________________________________________________________
////|
////| Function: CWellLogReader
////|
////! \param None.
////! \return None.
////!
////! Constructor
////|____________________________________________________________________
//
//CWellLogReader::CWellLogReader(void) 
//{
//	line_counter = 0;
//	file_path = "";
//	sample_interval = 0.5f;
//	start_sample_position = 0;
//}
//
////|____________________________________________________________________
////|
////| Function: ~CWellLogReader
////|
////! \param None.
////! \return None.
////!
////! Destructor
////|____________________________________________________________________
//
//CWellLogReader::~CWellLogReader(void) 
//{}
//
////|____________________________________________________________________
////|
////| Function: addSearchFolder
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//void CWellLogReader::addSearchFolder(string path) 
//{
//	searchFolders.push_back(path);
//}
//
////|____________________________________________________________________
////|
////| Function: clearSearchFolders
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//void CWellLogReader::clearSearchFolders()
//{
//	searchFolders.clear();
//}
//
////|____________________________________________________________________
////|
////| Function: readLASFile
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//bool CWellLogReader::readLASFile(string file_name, WellLogData *log_data) 
//{
//
//	line_counter = 0;
//	// set static file name to enable access for error() procedure
//	file_path = file_name;
//	
//	FILE *file = fopen(file_name.c_str(), "rb");
//	
//	unsigned int i = 0;
//	// try to find file in search folders
//	while ((file == NULL) && (i < searchFolders.size())) 
//	{
//		string path = searchFolders[i] + file_name;
//		//cout << "   Trying alternative folder: " + path << endl;
//		file = fopen( path.c_str(), "rb" );
//		i++;
//	}
//
//	if (file != NULL) 
//	{
//		// line input buffers
//		char line[1200];
//		int i;
//        				
//		// some sections of the LAS file are mandatory
//		bool section_found[6];
//		for (i = 0; i < 6; i++) section_found[i] = false;
//
//		LAS_SECTION current_section = NONE;
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
//				// speed up reading of lines if we are reading the log data samples
//				if (current_section == LOG_DATA) {
//					if (readLASLogData(line, log_data)) {
//						if (line_counter % 100 == 0) cout << ".";
//						continue;
//					}
//				} 
//
//				boost::regex expression;
//				boost::cmatch matches;
//
//				// tilde recognized: beginning of new section
//				expression.assign("^[ ]*#");
//				if (boost::regex_search( line, matches, expression )) {
//					continue;
//				} 
//			
//				char newSection;
//
//				expression.assign("^[ ]*~([A-Za-z]{1})");
//				// if line does not start with the comment flag -> evaluate
//				if (boost::regex_search( line, matches, expression )) {
//					current_section = NONE;
//					string section(matches[1].first, matches[1].second);
//					newSection = section[0];
//				} else {
//					newSection = ' ';
//				}
//
//				switch (current_section) {
//					case VERSION_INFORMATION:
//            if (!readLASVersionInformation(line, log_data)) { fclose(file); return false; } // BUG? FORGET TO CLOSE FILE, ADDED fclose() BY MARK
//						break;
//					case WELL_INFORMATION:
//						if (!readLASWellInformation(line, log_data)) { fclose(file); return false; } // BUG? FORGET TO CLOSE FILE, ADDED fclose() BY MARK
//						break;
//					case CURVE_INFORMATION:
//						if (!readLASCurveInformation(line, log_data)) { fclose(file); return false; } // BUG? FORGET TO CLOSE FILE, ADDED fclose() BY MARK
//						break;
//					case PARAMETERS:
//						if (!readLASParameters(line, log_data)) { fclose(file); return false; } // BUG? FORGET TO CLOSE FILE, ADDED fclose() BY MARK
//						break;
//					case OTHER:
//						break;
//					case LOG_DATA:
//						if (!readLASLogData(line, log_data)) { fclose(file); return false; } // BUG? FORGET TO CLOSE FILE, ADDED fclose() BY MARK
//						if (line_counter % 100 == 0) cout << ".";
//						break;
//					case NONE:
//						switch (newSection) {
//							case 'V':
//								current_section = VERSION_INFORMATION;
//								section_found[VERSION_INFORMATION] = true;
//								//cout << file_name + ": reading version information..." << endl;
//								break;
//							case 'W':
//								current_section = WELL_INFORMATION;
//								section_found[WELL_INFORMATION] = true;
//								//cout << file_name + ": reading well information..." << endl;
//								break;
//							case 'C':
//								current_section = CURVE_INFORMATION;
//								section_found[CURVE_INFORMATION] = true;
//								//cout << file_name + ": reading curve information..." << endl;
//								break;
//							case 'P':
//								current_section = PARAMETERS;
//								section_found[PARAMETERS] = true;
//								//cout << file_name + ": reading parameters..." << endl;
//								break;
//							case 'O':
//								current_section = OTHER;
//								section_found[OTHER] = true;
//								//cout << file_name + ": reading additional information..." << endl;
//								break;
//							case 'A':
//								current_section = LOG_DATA;
//								section_found[LOG_DATA] = true;
//								//cout << file_name + ": reading log data..." << endl;
//								
//								// was information about curves found before?
//								if (section_found[CURVE_INFORMATION]) {
//									// read log data header which contains column information about samples
//									if (!readLASLogDataHeader(line, log_data)) { fclose(file); return false; } // BUG? FORGET TO CLOSE FILE, ADDED fclose() BY MARK
//								} else {
//									error("Log data found, but curves were not defined in curve information section.");
//								}
//								break;
//							default:
//                fclose(file); // BUG? FORGET TO CLOSE FILE, ADDED fclose() BY MARK
//								return false;
//								//break;
//						}
//						break;
//					default:
//            fclose(file); // BUG? FORGET TO CLOSE FILE, ADDED fclose() BY MARK
//						return false;
//						//break;
//				}
//			} else {
//				error("Error while reading file");
//				fclose(file);
//				return false;
//			}
//		}
//
//		cout << endl << curves[0]->getNumberOfSamples() << " data samples imported." << endl;
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
////| Function: readLASVersionInformation
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//bool CWellLogReader::readLASVersionInformation(const char *line, WellLogData *log_data) 
//{
//	boost::regex expression;
//	boost::cmatch matches;
//
//	expression.assign("VERS[ ]*\\.[ ]*([0-9]+).([0-9]+)[ ]*:");
//	if (boost::regex_search( line, matches, expression )) {
//		string versionHigh(matches[1].first, matches[1].second);
//		string versionLow(matches[2].first, matches[2].second);
//
//		cout << "LAS version " << versionHigh + "." + versionLow << endl;
//		
//		if (versionHigh.compare("2") != 0) {
//			warning("Incorrect version number. Imported log data might be corrupt.");
//		}
//		return true;
//	} else {
//		expression.assign("WRAP[ ]*\\.[ ]*(YES|NO)[ ]*:");
//		if (boost::regex_search( line, matches, expression )) {
//			string wrappingUsed(matches[1].first, matches[1].second);
//			
//			// \todo Wrapped line data not yet supported
//			if (wrappingUsed.compare("YES") == 0) {
//        error("Wrapped data format found. This function is not supported, aborting import.");
//				return false;
//			}
//			return true;
//		} else {
//			error("Version information corrupt.");
//			return false;
//		}
//	}
//}
//
////|____________________________________________________________________
////|
////| Function: readLASWellInformation
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//bool CWellLogReader::readLASWellInformation(const char *line, WellLogData *log_data) 
//{
//	boost::regex expression;
//	boost::cmatch matches;
//
//	expression.assign("[ ]*([A-Za-z0-9]{3,})[ ]*\\.[ ]*(?:(?:([A-Za-z]{0,4})[ ]*([0-9\\.\\+-]+))|(?:[ ]*([A-Za-z]*)[ ]*)):");
//	if (boost::regex_search( line, matches, expression )) {
//		string full(matches[0].first, matches[0].second);
//		string container(matches[1].first, matches[1].second);
//		string value1(matches[2].first, matches[2].second);
//		string value2(matches[3].first, matches[3].second);
//
//		CWellLogHeader *header = log_data->getHeader();
//		if (header) {
//			// API ID of the log
//			if (container == "API") {
//				if (!value1.empty()) {
//					header->API = value1;
//					return true;
//				} else {
//					warning("Could not find API in file information. Using file name as API instead.");
//					
//					expression.assign("([0-9]{6,})[^/^\\\\^\\.]*\\.[^/^\\\\^\\.]+$");
//					if (boost::regex_search( file_path.c_str(), matches, expression )) {
//						string api(matches[1].first, matches[1].second);
//						header->API = api;
//					} else {
//						warning("Could not determine API from file name.");
//					}
//					return true;
//				}
//			} 
//			// sample interval for curves
//			if (container == "STEP") {
//				if (!CToolBox::from_string<float>(sample_interval, value2, std::dec))
//					warning("Step size for log samples could not be read. Defaulting to 0.5ft.");
//			}
//			// start depth for log data
//			if (container == "STRT") {
//				if (!CToolBox::from_string<float>(start_sample_position, value2, std::dec)) 
//					warning("Start value for log data could not be read. Defaulting to 0.");
//			}
//
//			
//		} else {
//			error("Internal error: Log Header information not available.");
//		}			
//	}
//	return true;
//}
//
////|____________________________________________________________________
////|
////| Function: readLASCurveInformation
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//bool CWellLogReader::readLASCurveInformation(const char *line, WellLogData *log_data) 
//{
//	boost::regex expression;
//	boost::cmatch matches;
//
//	// try to find curve information
//	expression.assign("^[ ]*([A-Za-z]+)[ ]*\\.[ ]*([A-Za-z]+)[ ]*:[ ]*([^\\n]+)[ ]*\\n");
//	if (boost::regex_search( line, matches, expression )) {
//		string curveID(matches[1].first, matches[1].second);
//		string curveUnit(matches[2].first, matches[2].second);
//		string curveDescription(matches[3].first, matches[3].second);
//
//		// in some files, the curve is misnamed 'DEPT' instead of DEPTH in the curve info
//		if (curveID.compare("DEPT") == 0) curveID = "DEPTH";
//
//		// add a new curve to the data set and init with read values
//		log_data->addCurve(curveID, curveDescription);
//		return true;
//	} else {
//		error("Invalid curve information found.");
//		return true;  // BUG? SHOULD BE false?
//	}
//}
//
////|____________________________________________________________________
////|
////| Function: readLASParameters
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//bool CWellLogReader::readLASParameters(const char *line, WellLogData *log_data) 
//{
//	return true;
//}
//
////|____________________________________________________________________
////|
////| Function: readLASLogDataHeader
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//bool CWellLogReader::readLASLogDataHeader(const char *line, WellLogData *log_data) 
//{
//	boost::regex expression;
//	boost::cmatch matches;
//
//	int count = log_data->getNumberOfCurves();
//
//	string expression_string("~A[ ]*");
//
//	if (count > 0) {
//		for (int i = 0; i < count; i++) {
//			expression_string += "([A-Za-z]+)[ ]*";
//		}
//	        
//		curves.clear();
//
//		// try to find column information
//		expression.assign(expression_string);
//		if (boost::regex_search( line, matches, expression )) {
//			for (int i = 1; i <= count; i++) {
//				string id(matches[i].first, matches[i].second);
//
//				CLogCurve *column = log_data->getCurveByID(id);
//
//				if (column != NULL) {
//          curves.push_back( column );
//					column->setSampleIntervalFeet(sample_interval);
//					column->setStartSamplePositionFeet(start_sample_position);
//
//					//cout << "Log data samples found for " + id + ": " + column->getDescription() << endl;
//				} else {
//					error("Curve data for " + id + " found, but curve information was not defined.");
//					return false;
//				}
//			}
//			return true;
//		} else {
//			error("Log data columns don't match curve description.");
//			return false;
//		}
//	} else {
//		error("Log data found, but no curves were defined.");
//		return false;
//	}
//}
//
////|____________________________________________________________________
////|
////| Function: readLASLogData
////|
////! \param None.
////! \return None.
////!
////! ...
////|____________________________________________________________________
//
//bool CWellLogReader::readLASLogData(const char *line, WellLogData *log_data) 
//{
//	double f_magnitude;
//	string format_string = "%lf";
//	for (unsigned int i = 0; i < curves.size(); i++) 
//	{
//		if (sscanf(line, format_string.c_str(), &f_magnitude) == 1) 
//		{
//			curves[i]->appendSample(f_magnitude);
//		} 
//		else 
//		{
//			error("Invalid data sample found.");
//			return false;
//		}
//
//		format_string = "%*lf " + format_string;
//	}
//	return true;
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
//void CWellLogReader::error(string msg) 
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
//void CWellLogReader::warning(string msg) {
//	cout << "WARNING [" + file_path + ", " + CToolBox::itos(line_counter) + "]: " + msg << endl;
//}
//
