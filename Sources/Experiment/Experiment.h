#ifndef EXPERIMENT_H
#define EXPERIMENT_H

//| Local Includes
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

#include <boost/algorithm/string.hpp>

#include <gmtl\gmtl.h>

class Experiment{
public:
	Experiment();

	~Experiment();

	std::vector<std::vector<gmtl::Point3f>>& GetIDValues(const char* fileName, char* sessionName);
	std::vector<std::vector<gmtl::Point3f>>& GetObjectPositions(const char* fileName, char* sessionName);
	std::vector<gmtl::Point3f>& GetObjectSizes(char* fileName, char* sessionName);
	std::vector<std::vector<gmtl::Vec4f>>& GetObjectColors();

	std::vector<int>& GetTrialOrder();

	void PrepareExperimentData(int id);

	std::vector<int> GetSessionOrder(int id);

	std::string getSubjectFolder();

	void writeUserStatsToFile();

	std::string subjectFolder;

	struct subsessionResults{
		gmtl::Point3f startObjectSize, targetObjectSize;
		double distance;
		double indexofDifficulty;

		double firstTouchDistance;
		double startObjectSelectionTime;
		double taskCompletionTime;
		int startObjectMisses, targetObjectMisses, totalMisses;
	};

private:
	std::vector<std::vector<gmtl::Point3f>> practicePositions;
	std::vector<std::vector<gmtl::Point3f>> experimentPositions;
	std::vector<std::vector<gmtl::Point3f>> experimentIDValues;

	std::vector<gmtl::Point3f > practiceSizes;
	std::vector<gmtl::Point3f > experimentSizes;

	std::vector<std::vector<gmtl::Vec4f>> ObjectColors;

	std::vector<int> ExperimentTrialOrder;
};

#endif