//!
//! \file Experiment.h
//!
//! Author: Prabhakar.

//
//include the trails for the user before performing the experiment.
//

#ifndef __EXPERIMENT_H
#define __EXPERIMENT_H

#include <fstream>
#include <time.h>

#include "Mmsystem.h"

#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osgUtil/SceneView>
#include <osg/PositionAttitudeTransform>
#include <osg/Camera>

#include <gmtl/gmtl.h>

#include "WellLog/WellCollection.h"
#include "WellLog/WellDisplay.h"
#include "OverheadMap/OverheadMapSel.h"
#include "iPod/HostStation.h"

class Experiment{
	public:
		//Constructor.
		Experiment();

		//Destructor.
		virtual ~Experiment();

		//Init routine
		void initExperiment();

		//Retunrs the root node for experiment texts.
		osg::Group* getRoot();

		//Selection between ray casting and handy map
		void experimentTaskTechnique(string experimentTechnique);

		//
		void setExperimentSession(string session_name);

		//Display the experiment description to user
		osg::Geode* describeExperiment();

		//Instructions to the user on how to select an object.
		osg::Geode* showInstructionsText();

		//Simple selection task for instructing the user.
		osg::Geode* instructionsTask();

		//
		void displayInstructions(bool _inst);

		//Display the experiment description to user
		osg::Geode* startUserTrials();

		//Display the experiment description to user
		osg::Geode* startUserTasks();

		//Display the challenge task description to user
		osg::Geode* startChallengeTask();

		//Display session break text		
		osg::Geode* getSessionBreakText();

		//Display thank you text		
		osg::Geode* getThankyouText();

		//replaced tastStartTime(), taskEndTime() by the following
		string runExperiment();

		//
		void setUserCoordinateFrame();

		//
		gmtl::Vec4f getUserFrameCoordinates(string positionName);

		//
		osgText::Text* checkUserInitialPosition(float x, float y, float z);

		//
		void startTargetSelection();

		//to select a well for the task.
		void wellToBeSelected();
		int getWellToBeSelected();

		//read the experiment targets from a file.
		void readExperimentTargets();

		//return the trial number
		int getTrialNumber();

		//Generate unique id for each subject.
		void createSubjectID();

		//create individual directory for each user.
		void Experiment::createUserDirectory();

		//create a file for each user to record his/her stats
		void createUserStatsFile();

		//update the user statistics profile.
		void updateUserStats();

		void writeToFile();

		//get the target well number
		void getTargetWellNumber();

		//highlight the well to be selected.
		void highlightTargetWell();

		//check for the correct well selection
		int checkSelectedWell(int _highlight_id, bool first_selection);

		//
		void finishWellSelection(int _highlight_id, char* str);

		//alert for correct selection
		void correctSelection();

		//alert if incorrect well selected
		void incorrectAlert();

		//get the number of incorrect selections
		int getSelectionAccuracy();

		//set if the well is skipped
		void setTargetSkipped(bool skipped);

		//set the obstructions for hand and head
		void setWellProperties(float well_depth, int _hand_obs, int _head_obs);

		//
		void setDeviceFootPrint(float device_footprint, float device_angular_footprint);

		//time taken to perform the task
		void taskPerformanceTime(std::vector<int>& vec);
		
		//
		void setWellDisplayNodes(vector< osg::ref_ptr<WellDisplay> > well_display_nodes);

		//reset the trials and task number for every experiment mode.
		void resetExperimentCounters();
		
		void startTimer();
	
	protected:
		osg::ref_ptr<osg::MatrixTransform> _expt_tf_nd;                 //!< Transform node. (Root)

		vector< osg::ref_ptr<WellDisplay> > _well_display_nodes;          //!< Vector of "WellDisplay" nodes in scene graph.

		string _experiment_session;
		int _practice_runs, _number_of_trials, _number_of_tasks, _task_Number, _trial_Number;

		int _start_Target, _well_ToBe_Selected, _challenge_Task_Target;
		int _well_Selected;

		int _number_of_wells;

		string _selection_Technique;			//"RAY CASTING", "HANDY MAP ON", "HANDY MAP OFF", "SECONDARY VIRTUAL RAY", "OVERHEAD MAP", "FINGER MARKER"
		int _subject_Number;				
		double _task_Start_Time, _task_End_Time, _task_First_Selection_Time, _task_Performance_Time;
		int _selection_accuracy;
		int _hand_obstructions, _head_obstructions;
		float _well_Depth;
		float _task_device_footprint, _total_Device_Footprint, _task_device_angular_footprint, _total_Device_Angular_Footprint;

		bool _start_Target_Selected, _display_instructions, _target_Skipped;

		//User frame coordinates
		gmtl::Vec4f _user_Center_Coordinates, _user_Hand_Coordinates, _user_Head_Coordinates;

		string _subject_Id;
		string _directory_Name;
	
		//text node for displaying the task
		osg::ref_ptr<osg::MatrixTransform> _taskdesc_tf_nd;

		//Geode and text that describes the experiment.
		osg::ref_ptr<osg::Geode>           _taskdesc_nd;
		osg::ref_ptr<osgText::Text>        _taskdesc_txt_nd;

		//Geode and text that describes the user trials.
		osg::ref_ptr<osg::Geode>           _desc_nd;
		osg::ref_ptr<osgText::Text>        _title_txt_nd;
		osg::ref_ptr<osgText::Text>        _desc_txt_nd;

		osg::StateSet* _state;

		//Overhead map
		OverheadMapSel *_omap_system;                                     //!< Overhead map system.

		//stats file
		fstream dataFile;

		//vectors holding the experiment targets
	    vector<int> _trial_targets;
		vector<int> _task_targets;
};	
#endif