//!
//! \file Experiment.cpp
//!
//!
//! Author: Prabhakar.

#define NOMINMAX

#include <stdio.h>
#include <math.h>
#include <windows.h>

#include <direct.h> 
#include <stdlib.h>

#include <boost/bind.hpp>
#include <boost/functional/hash.hpp>

#include <osg/StateSet>
#include <osgUtil/LineSegmentIntersector>
#include <osgText/Text3D>
#include <osgText/Font3D>

//#include <osg/PolygonMode>
//#include <osg/Point>

#include "Terrain/TerrainLoader.h"
#include "WellLog/WellLogHeader.h"
#include "WellLog/WellLogReader.h"
#include "WellLog/LogCurve.h"
#include "Experiment.h"
#include "Application.h"

//File 
#include <fstream> 
#include <limits>

#include <map>  
#include <utility>  

using namespace std;


Application *_application;

int low = 0, med = 0, high = 0;
int prev_well = -1;

//test for the time
int _start_time;

//maximum allowed error rate during the experiment.
const float _max_idle_time = 15.0;

string SUBJECT_ID = "1";

//user stats file
fstream _stats_File;
string _file_Name;

//well depth
float _well_depth;

vector<float> results;
map<float, vector<float>> _user_Results;

//temp vectors to store the targets to prevent repeating.
vector<int> _temp_trial_targets, _temp_task_targets;

//Constructor
Experiment::Experiment(){ 
	srand ( time(0) );

	_taskdesc_tf_nd  = new osg::MatrixTransform();
	_state = _taskdesc_tf_nd->getOrCreateStateSet();
	_state->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

	_taskdesc_nd = new osg::Geode;

	_taskdesc_txt_nd = new osgText::Text;
}

//Desctructor
Experiment::~Experiment(){
}

void Experiment::initExperiment(){
	_display_instructions = true;
	_task_Number = 0;
	_trial_Number = 0;
	
	_practice_runs = 12;

	_well_ToBe_Selected = -1;
	_selection_Technique = 2;		//handy map by default.

	_start_Target_Selected = false;
	_target_Skipped = false;

	_selection_accuracy = 0;	

	_total_Device_Footprint = 0.0f;

	_challenge_Task_Target = 0;

	_expt_tf_nd = new osg::MatrixTransform();

	readExperimentTargets();	
	setUserCoordinateFrame();
	createSubjectID();
}

void Experiment::readExperimentTargets(){
	cout << "READING EXPERIMENT TARGETS.... " << endl;

	char *path = NULL;
	string file_Name = getcwd(NULL, 0); 
	file_Name.append("\\expt_config\\expt_target_objects.txt");
	
	dataFile.open(file_Name.c_str());
	string line;

	if(dataFile.is_open()){
		while(!dataFile.eof()){
			getline(dataFile, line);

			if((line.substr(0, 5)).compare("Trial") == 0){
				istringstream iss(line);
				do{
					string temp;
					iss >> temp;

					if(temp.compare("Trial") == 0 || temp.length() == 0)
						continue;
					else{
						_trial_targets.push_back(atoi(temp.c_str()));
					}
				}while(iss);
				cout << "Number of Objects for Trials: " << _trial_targets.size() << endl;
			}
			
			if((line.substr(0, 5)).compare("Tasks") == 0){
				istringstream iss(line);
				do{
					string temp;

					iss >> temp;

					if(temp.compare("Tasks") == 0 || temp.length() == 0)
						continue;
					else{
						_task_targets.push_back(atoi(temp.c_str()));
					}
				}while(iss);
				cout << "Number of Objects for Tasks: " << _task_targets.size() << endl;
			}
		}		
	}
	dataFile.close();
	_number_of_trials = _trial_targets.size();
	_number_of_tasks = _task_targets.size();
	/*_number_of_trials = 2;
	_number_of_tasks = 2;*/
}

int Experiment::getTrialNumber(){
	if(_experiment_session.compare("TRIAL SESSION") == 0)
		return _trial_targets.size();
	else if(_experiment_session.compare("TASK SESSION") == 0)
		return _task_targets.size();
	else if(_experiment_session.compare("CHALLENGE TASK") == 0)
		return 1;
	else 
		return 1;
}

void Experiment::createSubjectID(){
	_subject_Id = "_user_";
	_subject_Id.append(SUBJECT_ID);
	
	cout << "SUBJECT ID: " << _subject_Id << endl;
}

//Interaction Techniques compared in the experiment
//	1 - Ray casting,
//	2 - Map visuals on iPod touch: on,
//Three seperate data file - One for the practice sessions data, one for the experiment session and the other for the challenge session.	
void Experiment::createUserStatsFile(){
	char *path = NULL;
	_file_Name = getcwd(NULL, 0); // 

	_file_Name.append("\\experiment_Data\\");
	_file_Name.append(_selection_Technique);

	if(_experiment_session.compare("TRIAL SESSION") == 0)
		_file_Name.append("\\Trial");
	else if(_experiment_session.compare("TASK SESSION") == 0)
		_file_Name.append("\\Task");
	else if(_experiment_session.compare("CHALLENGE TASK") == 0)
		_file_Name.append("\\Challenge");

	_file_Name.append(_subject_Id);
	_file_Name.append(".txt");

	cout << "FILENAME: " << _file_Name << endl;
}

void Experiment::setExperimentSession(string session_name){
	_experiment_session = session_name;
	
	if(session_name.compare("INSTRUCTIONS") != 0)
		createUserStatsFile();
	
	cout << "SESSION: " << _experiment_session << endl;
}

void Experiment::experimentTaskTechnique(string experimentTechnique){
	_selection_Technique = experimentTechnique;
	cout << "Experiment Selection Technique Set -> " << _selection_Technique << endl;
}

void Experiment::setWellDisplayNodes(vector< osg::ref_ptr<WellDisplay> > well_display_nodes){
	_well_display_nodes = well_display_nodes;

	//get the total number of wells on the terrain.
	_number_of_wells = _well_display_nodes.size();
}

osg::Geode* Experiment::describeExperiment(){
	_desc_nd = new osg::Geode();
	_desc_txt_nd = new osgText::Text();
	_title_txt_nd = new osgText::Text();

	_title_txt_nd->setFont("fonts/arial.ttf");
	_title_txt_nd->setText("EXPERIMENT\n\n\n");
	_title_txt_nd->setColor(osg::Vec4f(1.0f,1.0f,1.0f,1.0f));
	_title_txt_nd->setCharacterSize(0.095f);
	_title_txt_nd->setAlignment(osgText::Text::CENTER_BOTTOM_BASE_LINE);
	
	if(_selection_Technique.compare("RAY CASTING") == 0)
		_desc_txt_nd->setText("This experiment compares two interaction techniques.\n\nYou will have a few practice trials in the beginning.\n\nClick the trigger button(button underneath the wand) to dismiss this message and start the experiment.");
	else
		_desc_txt_nd->setText("This experiment compares two interaction techniques.\n\nYou will have a few practice trials in the beginning.\n\nTap anywhere on the iPod to dismiss this message and start the experiment.");

	_desc_txt_nd->setColor(osg::Vec4f(1.0f,1.0f,1.0f,1.0f));
	_desc_txt_nd->setFont("fonts/arial.ttf");
	_desc_txt_nd->setCharacterSize(0.055f);
	_desc_txt_nd->setAlignment(osgText::Text::CENTER_TOP);
	_desc_txt_nd->setAxisAlignment(osgText::TextBase::SCREEN);

	_desc_nd->addDrawable( _title_txt_nd.get() );
	_desc_nd->addDrawable( _desc_txt_nd.get() );

	return (_desc_nd.release());
}

osg::Geode* Experiment::showInstructionsText(){
	_desc_nd = new osg::Geode();
	_desc_txt_nd = new osgText::Text();
	_title_txt_nd = new osgText::Text();

	_title_txt_nd->setText("INSTRUCTIONS\n\n\n");
	_title_txt_nd->setColor(osg::Vec4f(1.0f,1.0f,1.0f,1.0f));
	_title_txt_nd->setFont("fonts/arial.ttf");
	_title_txt_nd->setCharacterSize(0.095f);
	_title_txt_nd->setAlignment(osgText::Text::CENTER_BOTTOM_BASE_LINE);

	if(_selection_Technique.compare("RAY CASTING") == 0)
		_desc_txt_nd->setText("Please read these instructions carefully and ask the experimenter if you have any questions.\n\nMove the wand in your hand and point at the object that is highlighted in blue.\n\nThe ray coming out of the wand will show the pointing direction.\n\nOnce you point at the highlighted object it will turn red.\n\nPress the trigger button (button underneath the wand) to select the object.");
	else
		_desc_txt_nd->setText("Please read these instructions carefully and ask the experimenter if you have any questions.\n\nIn this experiment technique you will use the iPod touch to select the objects instead of the standard wand.\n\nYou will have to move your finger on the iPod touch surface to select the object.\n\nThe object closest to your finger will be highlighted on the screen in front of you.\n\nMove your finger on the touch surface to select the object highlighted in blue.\n\nOnce you are close to the blue object, it turns red.\n\nRelease your finger from the touch surface to select the object.");

	_desc_txt_nd->setColor(osg::Vec4f(1.0f,1.0f,1.0f,1.0f));
	_desc_txt_nd->setFont("fonts/arial.ttf");
	_desc_txt_nd->setCharacterSize(0.055f);
	_desc_txt_nd->setAlignment(osgText::Text::CENTER_TOP);
	_desc_txt_nd->setAxisAlignment(osgText::TextBase::SCREEN);

	_desc_nd->addDrawable( _title_txt_nd.get() );
	_desc_nd->addDrawable( _desc_txt_nd.get() );

	return (_desc_nd.release());
}

osg::Geode* Experiment::startUserTrials(){
	results.clear();
	_user_Results.clear();

	_desc_nd = new osg::Geode();
	_desc_txt_nd = new osgText::Text();
	_title_txt_nd = new osgText::Text();

	_title_txt_nd->setText("PRACTICE SESSION\n\n\n");
	_title_txt_nd->setColor(osg::Vec4f(1.0f,1.0f,1.0f,1.0f));
	_title_txt_nd->setFont("fonts/arial.ttf");
	_title_txt_nd->setCharacterSize(0.095f);
	_title_txt_nd->setAlignment(osgText::Text::CENTER_BOTTOM_BASE_LINE);

	if(_selection_Technique.compare("RAY CASTING") == 0)
		_desc_txt_nd->setText("You have to select the object in the blue color.\n\nThis is a practice session. You can take your time to understand the system.\n\nClick the select button to dismiss this message.");
	else if(_selection_Technique.compare("HANDY MAP") == 0)
		_desc_txt_nd->setText("You have to select the object in the blue color.\n\nMove your finger on the iPod(touch) surface to select the object.\n\nThere will be visuals on the iPod for your reference.\n\nThe object closest to your finger position is highlighed on the screen and the iPod.\nThis is a practice session. Try to select the object without looking at the iPod touch.\n\nYou can take your time to understand the system.\n\nTap anywhere on the iPod to dismiss this message.");
	
	_desc_txt_nd->setColor(osg::Vec4f(1.0f,1.0f,1.0f,1.0f));
	_desc_txt_nd->setFont("fonts/arial.ttf");
	_desc_txt_nd->setCharacterSize(0.055f);
	_desc_txt_nd->setAlignment(osgText::Text::CENTER_TOP);

	_desc_nd->addDrawable( _title_txt_nd.get() );
	_desc_nd->addDrawable( _desc_txt_nd.get() );

	return (_desc_nd.release());
}

osg::Geode* Experiment::startUserTasks(){
	results.clear();
	_user_Results.clear();

	_desc_nd = new osg::Geode();
	_desc_txt_nd = new osgText::Text();
	_title_txt_nd = new osgText::Text();

	_title_txt_nd->setText("TASK\n\n\n");
	_title_txt_nd->setColor(osg::Vec4f(1.0f,1.0f,1.0f,1.0f));
	_title_txt_nd->setFont("fonts/arial.ttf");
	_title_txt_nd->setCharacterSize(0.095f);
	_title_txt_nd->setAlignment(osgText::Text::CENTER_BOTTOM_BASE_LINE);
	
	if(_selection_Technique.compare("RAY CASTING") == 0)
		_desc_txt_nd->setText("Practice completed and now you will be performing real trials.\n\nYou have to select the object in the blue color.\n\nThis is NOT a practice and you should be as quick and accurate as possible.\n\nClick the select button to dismiss this message.");
	else 
		_desc_txt_nd->setText("Practice completed and now you will be performing real trials.\n\nYou have to select the object in the blue color.\n\nThis is NOT a practice and you should be as quick and accurate as possible.\n\nTry to select the objects without looking at the iPod touch.\n\nTap anywhere on the iPod to dismiss this message.");

	_desc_txt_nd->setColor(osg::Vec4f(1.0f,1.0f,1.0f,1.0f));
	_desc_txt_nd->setFont("fonts/arial.ttf");
	_desc_txt_nd->setCharacterSize(0.065f);
	_desc_txt_nd->setAlignment(osgText::Text::CENTER_TOP);

	_desc_nd->addDrawable( _title_txt_nd.get() );
	_desc_nd->addDrawable( _desc_txt_nd.get() );

	return (_desc_nd.release());
}

osg::Geode* Experiment::startChallengeTask(){
	results.clear();
	_user_Results.clear();

	_desc_nd = new osg::Geode();
	_desc_txt_nd = new osgText::Text();
	_title_txt_nd = new osgText::Text();

	_title_txt_nd->setText("CHALLENGE TASK\n\n\n");
	_title_txt_nd->setColor(osg::Vec4f(1.0f,1.0f,1.0f,1.0f));
	_title_txt_nd->setFont("fonts/arial.ttf");
	_title_txt_nd->setCharacterSize(0.095f);
	_title_txt_nd->setAlignment(osgText::Text::CENTER_BOTTOM_BASE_LINE);
	
	if(_selection_Technique.compare("RAY CASTING") == 0)
		_desc_txt_nd->setText("This session is similar to the previous trial/task session.\n\nYou have to select the object in highlighted in blue.\n\nThere will be only one task and this is NOT a practice.\n\nYou should be as quick and accurate as possible.\n\nClick the select button to dismiss this message and start the task.");
	else 
		_desc_txt_nd->setText("This session is similar to the previous trial/task session.\n\nYou have to select the object in highlighted in blue.\n\nThere will be only one task and this is NOT a practice.\n\nYou should be as quick and accurate as possible.\n\nTap anywhere on the iPod to dismiss this message.");

	_desc_txt_nd->setColor(osg::Vec4f(1.0f,1.0f,1.0f,1.0f));
	_desc_txt_nd->setFont("fonts/arial.ttf");
	_desc_txt_nd->setCharacterSize(0.065f);
	_desc_txt_nd->setAlignment(osgText::Text::CENTER_TOP);

	_desc_nd->addDrawable( _title_txt_nd.get() );
	_desc_nd->addDrawable( _desc_txt_nd.get() );

	return (_desc_nd.release());
}

osg::Geode* Experiment::getSessionBreakText(){
	_desc_nd = new osg::Geode();
	_desc_txt_nd = new osgText::Text();

	_desc_txt_nd->setText("You have completed the first session in the experiment.\n\nYou will have a minute break, which can be extended.");

	_desc_txt_nd->setColor(osg::Vec4f(1.0f,1.0f,1.0f,1.0f));
	_desc_txt_nd->setFont("fonts/arial.ttf");
	_desc_txt_nd->setCharacterSize(0.095f);
	_desc_txt_nd->setAlignment(osgText::Text::CENTER_TOP);

	_desc_nd->addDrawable( _desc_txt_nd.get() );

	return (_desc_nd.release());
}

osg::Geode* Experiment::getThankyouText(){
	_desc_nd = new osg::Geode();
	_desc_txt_nd = new osgText::Text();

	_desc_txt_nd->setText("Thank you for participating in the experiment.\n\nHAVE A GREAT DAY!!!!");

	_desc_txt_nd->setColor(osg::Vec4f(1.0f,1.0f,1.0f,1.0f));
	_desc_txt_nd->setFont("fonts/arial.ttf");
	_desc_txt_nd->setCharacterSize(0.095f);
	_desc_txt_nd->setAlignment(osgText::Text::CENTER_TOP);

	_desc_nd->addDrawable( _desc_txt_nd.get() );

	return (_desc_nd.release());
}

void Experiment::wellToBeSelected(){
	if(_experiment_session.compare("TRIAL SESSION") == 0){
		int _index = rand() % _trial_targets.size();

		_well_ToBe_Selected = _trial_targets.at(_index);

		_temp_trial_targets.push_back(_well_ToBe_Selected);
		_trial_targets.erase(_trial_targets.begin() + _index);
	}
	if(_experiment_session.compare("TASK SESSION") == 0){
		int _index = rand() % _task_targets.size();
	
		_well_ToBe_Selected = _task_targets.at(_index);

		_temp_task_targets.push_back(_well_ToBe_Selected);
		_task_targets.erase(_task_targets.begin() + _index);
	}
	
	_well_display_nodes[_well_ToBe_Selected]->select(true);
	
	prev_well = _well_ToBe_Selected;
}

void Experiment::startTargetSelection(){
	_well_ToBe_Selected = 32;

	_start_Target = _well_ToBe_Selected;
	_well_display_nodes[_well_ToBe_Selected]->select(true);
	//cout << "START TARGET HIGHLIGHTED" << endl;
}

int Experiment::getWellToBeSelected(){
	return _well_ToBe_Selected;
}

void Experiment::displayInstructions(bool _inst){
	_display_instructions = _inst;
}

void Experiment::setUserCoordinateFrame(){
	_user_Hand_Coordinates = gmtl::Vec4f(-0.0887943f, 1.15429f, 1.883393f, 1.0f);
	_user_Head_Coordinates = gmtl::Vec4f(-0.137393f, 1.25529f, 1.872454f, 1.0f);
	_user_Center_Coordinates = gmtl::Vec4f(-0.113093f, 1.34656f, 1.877923f, 1.0f);
}

gmtl::Vec4f Experiment::getUserFrameCoordinates(string positionName){
	if(positionName.compare("HAND") == 0)
		return _user_Hand_Coordinates;
	if(positionName.compare("HEAD") == 0)
		return _user_Head_Coordinates;
	if(positionName.compare("CENTER") == 0)
		return _user_Center_Coordinates;
}

osgText::Text* Experiment::checkUserInitialPosition(float x, float y, float z){
	_desc_nd = new osg::Geode();
	_desc_txt_nd = new osgText::Text();

	_desc_txt_nd->setColor(osg::Vec4f(1.0f,1.0f,1.0f,1.0f));
	_desc_txt_nd->setFont("fonts/arial.ttf");
	_desc_txt_nd->setCharacterSize(0.045f);
	_desc_txt_nd->setAlignment(osgText::Text::CENTER_TOP);

	_desc_nd->addDrawable( _desc_txt_nd.get() );

	if((x - _user_Hand_Coordinates[0]) > 0.304)
		_desc_txt_nd->setText("MOVE YOUR HAND TO THE LEFT TO GET TO THE START POSITION");
	else if((x - _user_Hand_Coordinates[0]) < -0.304)
		_desc_txt_nd->setText("MOVE YOUR HAND TO THE RIGHT TO GET TO THE START POSITION"); 
	else if((y - _user_Hand_Coordinates[1]) > 0.304)
		_desc_txt_nd->setText("MOVE YOUR HAND DOWN TO GET TO THE START POSITION");
	else if((y - _user_Hand_Coordinates[1]) < -0.304)
		_desc_txt_nd->setText("MOVE YOUR HAND UP GET TO THE START POSITION"); 
	else if((z - _user_Hand_Coordinates[2]) > 0.304)
		_desc_txt_nd->setText("MOVE YOUR HAND FORWARD TOWARDS THE SCREEN TO GET TO THE START POSITION");
	else if((z - _user_Hand_Coordinates[2]) < -0.304)
		_desc_txt_nd->setText("MOVE YOUR HAND BACKWARDS AWAY FROM THE SCREEN TO GET TO THE START POSITION"); 
	else 
		_desc_txt_nd->setText("CONTINUE");

	return (_desc_txt_nd.release());
}

string Experiment::runExperiment(){
	_selection_accuracy = 0;

	if(_display_instructions == true){
		_well_ToBe_Selected = 6;
		_well_display_nodes[_well_ToBe_Selected]->select(true);
		return "INSTRUCTIONS";
	}

	if(_experiment_session.compare("CHALLENGE TASK") == 0){
		_well_ToBe_Selected = _challenge_Task_Target;
		
		float x, y,z;

		_well_display_nodes[_well_ToBe_Selected]->getWellPosition(x, y, z);
		_well_depth = z;

		_well_display_nodes[_well_ToBe_Selected]->select(true);
		return "CHALLENGE_FINISHED";
	}

	startTargetSelection();

	if(_start_Target_Selected != true){
		return "Start_Target_Not_Selected";
	}

	if(_trial_Number < _number_of_trials){
		wellToBeSelected();
	
		_trial_Number++;

		if(_trial_Number < _number_of_trials)
			return "TRAILS_REMAINING";
		else
			return "TRAILS_FINISHED";
	}

	else if(_task_Number < _number_of_tasks){
		wellToBeSelected();
	
		_task_Number++;

		if(_task_Number < _number_of_tasks)
			return "TASKS_REMAINING";
		else{
			cout << "TASKS: " << _task_Number << endl;

			return "TASKS_FINISHED";
		}
	}
}

int Experiment::checkSelectedWell(int _highlight_id, bool first_selection){
	if(first_selection)
		_task_First_Selection_Time = (clock() - _start_time)/(double) CLOCKS_PER_SEC;

	if(_highlight_id == _well_ToBe_Selected){
		if(_well_ToBe_Selected == 6){
			return 0;
		}
		if(_well_ToBe_Selected == _start_Target){
			_start_Target_Selected = true;
			return 0;
		}
		finishWellSelection(_highlight_id, __FUNCTION__);
		return 0;
	}
	else{
		if(_experiment_session.compare("INSTRUCTIONS") != 0 && _well_ToBe_Selected != _start_Target){
			_selection_accuracy++;
		}
		return 1;
	}
}

void Experiment::finishWellSelection(int _highlight_id, char* str){
	cout << "FINISH WELL SELECTION: " << _well_ToBe_Selected << endl;
	int elapsed_Time = clock() - _start_time;

	_task_Performance_Time = elapsed_Time/(double) CLOCKS_PER_SEC;

	updateUserStats();

	_well_display_nodes[_well_ToBe_Selected]->select(false);
	_well_ToBe_Selected = -1;
	_start_Target_Selected = false;
}

void Experiment::correctSelection(){
	PlaySound("C:\\Users\\pvv8654\\handyMapExperiment\\depthMarker\\Resources\\sounds\\DingLing.wav", NULL, SND_ASYNC);	
}

void Experiment::incorrectAlert(){
	PlaySound("C:\\Users\\pvv8654\\handyMapExperiment\\depthMarker\\Resources\\sounds\\buzzer.wav", NULL, SND_ASYNC);	
}

int Experiment::getSelectionAccuracy(){
	return _selection_accuracy;
}

void Experiment::setTargetSkipped(bool skipped){
	_target_Skipped = skipped;
}

void Experiment::startTimer(){
	_start_time = clock();
}

void Experiment::updateUserStats(){
	//well depth
	//task completion time
	//first selection time
	//incorrect selections
	//target skipped
	//obstructions
		//hand and eye
	//controller footprint
	cout << "UPDATE USER STATS: " << _well_ToBe_Selected << endl;

	results.clear();

	results.push_back(_task_Performance_Time);
	results.push_back(_task_First_Selection_Time);
	results.push_back(_selection_accuracy);
	if(_target_Skipped)
		results.push_back(1);
	else
		results.push_back(0);
	results.push_back(_head_obstructions);
	results.push_back(_hand_obstructions);
	results.push_back(_task_device_footprint);
	results.push_back(_task_device_angular_footprint);

	if(results.size() != 0){
		_user_Results.insert(make_pair(_well_Depth, results));
	}

	_selection_accuracy = 0;
	_target_Skipped = false;
}

void Experiment::writeToFile(){
	_stats_File.open(_file_Name.c_str(), ios::out);

	_stats_File << "FILE: USER_" << SUBJECT_ID << "\nSelection Technique: " << _selection_Technique;
	if(_experiment_session.compare("TRIAL SESSION") == 0)
		_stats_File << "\nNUMBER OF TRIALS: 12";
	else if(_experiment_session.compare("TASK SESSION") == 0)
		_stats_File << "\nNUMBER OF TASKS: 25";
	else if(_experiment_session.compare("CHALLENGE TASK") == 0)
		_stats_File << "\nCHALLENGE TASK";

	_stats_File << "\n\nRESULTS:";
	_stats_File << "\nWell Depth\tTask Completion Time\tFirst Selection Time\tIncorrect Selections\tTarget Skipped(Y/N)\tObstruction - Hand & Eye\tController Foot Print\tController Angular FootPrint";
	
	for(map<float, vector<float>>::iterator i = _user_Results.begin() ; i != _user_Results.end(); ++i){  
		vector<float> temp = i->second;
		_stats_File << "\n" << i->first << "\t\t\t" << temp[0] << "\t\t\t\t\t" << temp[1] << "\t\t\t\t\t\t\t" << temp[2] << "\t\t\t\t\t\t" << temp[3] << "\t\t\t\t\t\t" << temp[4] << "\t  " << temp[5] << "\t\t\t" << temp[6] << "\t\t\t" << temp[7]; 
	}
	_stats_File << "\nTotal Controller Footprint : " << _total_Device_Footprint;
	_stats_File << "\nTotal Controller Angular Footprint : " << _total_Device_Angular_Footprint;
	
	_total_Device_Footprint = 0.0f;
	_total_Device_Angular_Footprint = 0.0f;
	
	_stats_File.close();
}

void Experiment::resetExperimentCounters(){
	cout << "RESETING COUNTERS" << endl;
	_trial_targets = _temp_trial_targets;
	_task_targets = _temp_task_targets;
	_trial_Number = 0;
	_task_Number = 0;
	_selection_accuracy = 0;
}

void Experiment::setWellProperties(float well_depth, int _hand_obs, int _head_obs){
	_well_Depth = well_depth;
	_hand_obstructions = _hand_obs;
	_head_obstructions = _head_obs;
}

void Experiment::setDeviceFootPrint(float device_footprint, float device_angular_footprint){
	_task_device_footprint = device_footprint;
	_total_Device_Footprint += device_footprint;

	_task_device_angular_footprint = device_angular_footprint;
	_total_Device_Angular_Footprint += device_angular_footprint;
}