#include "Experiment\Experiment.h"

Experiment::Experiment(){
}

Experiment::~Experiment(){
}

std::vector<std::vector<gmtl::Point3f>>& Experiment::GetIDValues(const char* fileName, char* sessionName){
	std::ifstream posFile(fileName);
	
	std::string line;

	gmtl::Point3f temp;
	int count = 0;
	std::cout << "Opening Positions File: " << fileName << std::endl;
	if (posFile.is_open()){
		while (getline(posFile, line)){
			std::istringstream iss(line);

			int p1 = 0, p2 = 0, p3 = 0;

			p2 = line.find("|", p1);
			p3 = line.find("|", p2 + 1);

			std::string fp1 = line.substr(0, p2);
			std::string fp2 = line.substr(p2 + 1, p3);
			std::string fp3 = line.substr(p3 + 1, line.length());
			
			std::vector<std::string> strs1, strs2, strs3;
			boost::split(strs1, fp1, boost::is_any_of(","));
			boost::split(strs2, fp2, boost::is_any_of(","));
			boost::split(strs3, fp3, boost::is_any_of(","));

			std::vector<gmtl::Point3f> temp;
			temp.push_back(gmtl::Point3f(atof(strs1[0].c_str()), atof(strs1[1].c_str()), atof(strs1[2].c_str())));
			temp.push_back(gmtl::Point3f(atof(strs2[0].c_str()), atof(strs2[1].c_str()), atof(strs2[2].c_str())));
			temp.push_back(gmtl::Point3f(atof(strs3[0].c_str()), atof(strs3[1].c_str()), atof(strs3[2].c_str())));

			experimentSizes.push_back(gmtl::Point3f(atof(strs3[0].c_str()), atof(strs3[1].c_str()), atof(strs3[2].c_str())));

			if (sessionName == "PRACTICE")
				practicePositions.push_back(temp);
			else if (sessionName == "EXPERIMENT")
				experimentPositions.push_back(temp);
		}
		posFile.close();
	}
	else{
		std::cout << "ERROR -- CANNOT OPEN THE OBJECTS POSITIONS FILE!!!" << std::endl;
	}

	if (sessionName == "PRACTICE")
		return practicePositions;
	else if (sessionName == "EXPERIMENT")
		return experimentPositions;

//	system("PAUSE");
}

std::vector<std::vector<gmtl::Point3f>>& Experiment::GetObjectPositions(const char* fileName, char* sessionName){
	/*std::string name = "Resources/Experiment_Config/";
	name += fileName;*/
	
	std::ifstream posFile(fileName);// 1DTask_Object_Sizes.txt");
	//std::ifstream posFile("Resources/Experiment_Config/1DTask_Positions.txt");

	std::string line;

	gmtl::Point3f temp;
	int count = 0;
	std::cout << "Opening Positions File: " << fileName << std::endl;
	if (posFile.is_open()){
		while (getline(posFile, line)){
			std::istringstream iss(line);

			int p1 = 0, p2 = 0;

			p2 = line.find("|", p1);
			std::string fp1 = line.substr(0, p2);
			std::string fp2 = line.substr(p2 + 1, line.length());

			std::vector<std::string> strs1, strs2;
			boost::split(strs1, fp1, boost::is_any_of(","));
			boost::split(strs2, fp2, boost::is_any_of(","));

			std::vector<gmtl::Point3f> temp;
			temp.push_back(gmtl::Point3f(atof(strs1[0].c_str()), atof(strs1[1].c_str()), atof(strs1[2].c_str())));
			temp.push_back(gmtl::Point3f(atof(strs2[0].c_str()), atof(strs2[1].c_str()), atof(strs2[2].c_str())));

			if (sessionName == "PRACTICE")
				practicePositions.push_back(temp);
			else if (sessionName == "EXPERIMENT")
				experimentPositions.push_back(temp);
		}
		posFile.close();
	}
	else{
		std::cout << "ERROR--CANNOT OPEN THE OBJECTS POSITIONS FILE!!!" << std::endl;
	}

	if (sessionName == "PRACTICE")
		return practicePositions;
	else if (sessionName == "EXPERIMENT")
		return experimentPositions;
}

std::vector<gmtl::Point3f>& Experiment::GetObjectSizes(char* fileName, char* sessionName){
	std::string name = "Resources/Experiment_Config/";
	name += fileName;
	std::ifstream sizeFile(fileName);

	std::string line;

	if (sizeFile.is_open()){
		while (std::getline(sizeFile, line)){
			std::istringstream iss(line);

			std::vector<std::string> strs1;
			boost::split(strs1, line, boost::is_any_of(","));

			if (sessionName == "PRACTICE")
				practiceSizes.push_back(gmtl::Point3f(atof(strs1[0].c_str()), atof(strs1[1].c_str()), atof(strs1[2].c_str())));
			else if (sessionName == "EXPERIMENT")
				experimentSizes.push_back(gmtl::Point3f(atof(strs1[0].c_str()), atof(strs1[1].c_str()), atof(strs1[2].c_str())));
		}
		sizeFile.close();
	}
	else{
		std::cout << "ERROR--CANNOT OPEN THE OBJECTS SIZE FILE!!!" << std::endl;
	}

	if (sessionName == "PRACTICE")
		return practiceSizes;
	else if (sessionName == "EXPERIMENT")
		return experimentSizes;
}

std::vector<std::vector<gmtl::Vec4f>>& Experiment::GetObjectColors(){
	// Read Object Colors
	for (size_t i = 0; i < ExperimentTrialOrder.size(); i++){
		std::vector<gmtl::Vec4f> temp;

		if (i % 2 == 0){
			temp.push_back(gmtl::Vec4f(0.5f, 0.5f, 0.5f, 1.0f));
			temp.push_back(gmtl::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
		}
		else{
			temp.push_back(gmtl::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
			temp.push_back(gmtl::Vec4f(0.5f, 0.5f, 0.5f, 1.0f));
		}

		ObjectColors.push_back(temp);
	}

	return ObjectColors;
}

std::vector<int>& Experiment::GetTrialOrder(){
	//for (size_t i = 0; i < experimentPositions.size() * experimentSizes.size(); i++)
	for (size_t i = 0; i < experimentPositions.size(); i++)
		ExperimentTrialOrder.push_back(i);

	std::random_shuffle(ExperimentTrialOrder.begin(), ExperimentTrialOrder.end());

	return ExperimentTrialOrder;
}

void Experiment::PrepareExperimentData(int id){
	subjectFolder = "Resources/Experiment_Data/" + std::to_string(id);

	std::cout << "\tCreating Subject Folder.." << subjectFolder << std::endl;

	if (CreateDirectory(subjectFolder.c_str(), NULL))
		std::cout << "\tCreated Subject Directory" << std::endl;
	else{
		std::cout << "\tError Creating Subject Directory" << std::endl;
		return;
	}

	std::string subjectConfig = subjectFolder + "/Trial_Order.txt";

	std::fstream myfile(subjectConfig, std::ios::app);

	if (myfile.is_open()) 
		for (size_t i = 0; i < ExperimentTrialOrder.size(); i++)
			myfile << ExperimentTrialOrder.at(i) << "\n";
	
	myfile.close();
}

std::vector<int> Experiment::GetSessionOrder(int id){
	std::string fileName = "Resources/Experiment_Config/Subjects_Session_Order.txt";
	std::ifstream sizeFile(fileName);

	std::string line;
	int counter = 1;

	std::vector<int> order;

	if (sizeFile.is_open()){
		while (std::getline(sizeFile, line)){
			if (id == counter){
				//std::istringstream iss(line);
				std::cout << "STARTING SESSION: " << line << std::endl;

				if (std::atoi(line.c_str()) == 1){
					order.push_back(1);
					order.push_back(2);
				}
				else{
					order.push_back(2);
					order.push_back(1);
				}
				break;
			}
			counter++;
		}
		sizeFile.close();
	}
	else{
		std::cout << "ERROR--CANNOT OPEN THE SESSION ORDER FILE!!!" << std::endl;
	}
	return order;
}

std::string Experiment::getSubjectFolder() {
	return subjectFolder;
}