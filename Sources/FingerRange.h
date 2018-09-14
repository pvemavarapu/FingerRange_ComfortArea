#pragma once
//|___________________________________________________________________
//!
//! \file FingerRange.h
//!
//! Author: Prabhakar Vemavarapu.
//|___________________________________________________________________

#ifndef FINGER_RANGE_H
#define FINGER_RANGE_H

//| Local Includes

#include <iostream>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <MMSystem.h>
#include <algorithm>

#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osgUtil/SceneView>
#include <osg/PolygonMode>
#include <osg/LineWidth>
#include <osg/StateSet>
#include <osg/PositionAttitudeTransform>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>
#include <osgText/Text>
#include <osgGA/GUIEventHandler>

#include <osgViewer/Viewer>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <gmtl/gmtl.h>

#include "Experiment/Experiment.h"
#include "Experiment/InstructionsText.h"

#include "SceneObjects/ObjectDisplay.h"

class FingerRange : public osgGA::GUIEventHandler
{
public:
	//! Default Constructor.
	FingerRange();

	//! Destructor.
	~FingerRange();

	//! Init routine.
	void initScene();

	//! Gets scene graph root node.
	osg::Group* getScene();

	void update();

	void setViewer(osgViewer::Viewer* viewer);

	void registerTouchScreens(int screen_Number, int sx, int sy);

	void setSceneBounds(float l, float r, float t, float b, float n, float f);
	void setViewMatrix(osg::Matrixf mat);
	void setProjectionMatrix(osg::Matrixf mat);

	//Multi-touch Inputs
	void transInput(int axis, float x, float y, float _user_depth);

	void updateKeyboardCursor(float x, float y);

	void updateUserCursorPosition(int screen, int x, int y);

	void DoubleTapUpdate();

	//Keyboard Inputs
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&);

	// Variables for the touchscreen resolutions
	struct TouchScreenRes {
		int x;
		int y;
	};

	std::vector<FingerRange::TouchScreenRes> getDeviceResolution();

	int getNumberofOutlines();

	void setSubjectFolder(std::string foldername);

	float _on_Object_Time;

private:
	//! Creates a scene graph.
	void createSceneGraph();

	void showExperimentInfo();

	void SetNormalizeFactors();

	void SetNormalizeFactors(int sessionNumber);

	void showInteractivePracticeTask();
	void updatePracticeObjects();
	void updateExperimentObjects();

	void setObjectPosition(gmtl::Point3f size, gmtl::Point3f pos1, gmtl::Point3f pos2);

	void checkTargetProximity(double x, double y);

	void delayTimer(float seconds);

	void resetCounters();

	//User cursor
	osg::Geode* createUserCursor();

	void drawCoordinates();

	void showScreenErrorAlert(int screenNo);

	// OSG nodes
	osg::ref_ptr<osg::Group>           _root_nd;                      //!< Root node.
	osg::ref_ptr<osg::MatrixTransform> _uw_tf_nd;                     //!< User-world transform node.
	osg::ref_ptr<osg::Group>           _object_group_nd;                //!< Parent node of all objects.

	std::vector<osg::ref_ptr<osg::MatrixTransform>>		   _marker_tf_nd;

	//Objects
	vector< osg::ref_ptr<ObjectDisplay> > _object_nodes;          //!< Vector of "ObjectDisplay" nodes in scene graph.

	osg::ref_ptr<ObjectDisplay> _object_display_nd;

	osg::ref_ptr<osg::MatrixTransform> _grid_tf_nd;

	float _user_depth;
	int _selection_axis;

	osg::ref_ptr<osg::MatrixTransform> _position_Marker_tf_nd;
	osg::ref_ptr<osg::Geode> _position_Marker_nd;

	osg::ref_ptr<osg::Geode> axes_geode;
	osg::PositionAttitudeTransform* axisXForm;
	// User position representation
	osg::ref_ptr<osg::Geode> _cursor_Node;
	osg::ref_ptr<osg::PositionAttitudeTransform> cursor_PAT;

	int _connected_Devices_Counter;

	std::map<int, TouchScreenRes> _touchScreen_Resolution;
	std::vector<TouchScreenRes> _touchScreen_Res;

	gmtl::Point3f _normalize_Factors;
	std::vector<gmtl::Point3f> _normalized_Factors;

	// Practice Object Position and Sizes
	std::vector<std::vector<gmtl::Point3f>> _practice_Object_Positions;
	std::vector<gmtl::Point3f> _practice_Object_Sizes;

	// Experiment Trial Object Position and Sizes
	std::vector<std::vector<gmtl::Point3f>> _experiment_ID_Values;
	std::vector<std::vector<gmtl::Point3f>> _experiment_Object_Positions;
	std::vector<gmtl::Point3f> _experiment_Object_Sizes;
	std::vector<std::vector<gmtl::Vec4f>> _object_Colors;
	std::vector<int> _sessions_Order;
	std::vector<int> _subsessions_Order;

	std::vector<std::string> _session_Texts;
	std::vector<std::string> _subsession_Practice_Texts;
	std::vector<std::string> _subsession_Expt_Texts;
	std::vector<std::string> _subsession_On_Screen_Text;

	std::vector<std::string> _subsession_Names;

	std::vector<int> _trial_Order;

	osg::ref_ptr<osgText::Text> _expt_text_nd;

	//User Instructions - texts, data structures holding it, geode, etc.
	enum Instruction_Screen {
		EXPT_DESC, DEVICE_DESC, SESSION_1, SESSION_1_LT_PRACTICE, SESSION_1_RT_PRACTICE, SESSION_1_LT_EXPT, SESSION_1_RT_EXPT,
		SESSION_2, SESSION_2_LI_PRACTICE, SESSION_2_RI_PRACTICE, SESSION_2_LM_PRACTICE, SESSION_2_RM_PRACTICE,
		SESSION_2_LI_EXPT, SESSION_2_RI_EXPT, SESSION_2_LM_EXPT, SESSION_2_RM_EXPT
	};

	std::vector <std::string> _instruction_Screen;

	osg::ref_ptr<osg::Geode>           _desc_nd;
	osg::ref_ptr<osgText::Text>        _desc_txt_nd;

	osg::ref_ptr<osg::Geode>           _trialCounter_Text_nd;
	osg::ref_ptr<osgText::Text>        _trialCounter_Text;

	osg::ref_ptr<osg::Geode>           _on_Screen_Text_nd;
	osg::ref_ptr<osgText::Text>        _on_Screen_Text;

	osg::ref_ptr<osg::Geode>           _session_Text_nd;
	osg::ref_ptr<osgText::Text>        _session_Text;

	osg::ref_ptr<osg::Geode>           _practice_Instruction_Text_nd;
	osg::ref_ptr<osgText::Text>        _practice_Instruction_Text;

	osg::ref_ptr<osgText::Text>		   _task_Text;
	osg::ref_ptr<osg::Geode>           _task_Text_nd;

	/*********************************
	** Variables for the experiment **
	*********************************/
	Experiment *_experiment;

	std::vector< osg::ref_ptr<ObjectDisplay> > _object_display_nodes;
	osg::ref_ptr<osg::MatrixTransform> _gw_tf_nd;
	osg::ref_ptr<osg::MatrixTransform> _scene_tf_nd;

	osg::ref_ptr<osg::Node> _scene_nd;

	vector<gmtl::Point3f> object_XYZ;

	gmtl::Vec3f _user_Cursor_Orientation;

	float x_Factor, y_Factor, z_Factor;
	gmtl::Point3f _converted_Points;

	gmtl::Vec3f _cursor_Orientation;
	gmtl::Vec3f _user_Cursor_Position;

	const int ReceivesShadowTraversalMask = 0x1;
	const int CastsShadowTraversalMask = 0x2;

	int _interaction_Window_Width, _interaction_Window_Height;

	bool _start_Arcball_Rotation;
	float arcball_Radius, arcball_Radius_Square;
	float currX, currY, prevX, prevY;
	gmtl::Vec3f _arcball_Start_Rotation;
	gmtl::Vec3f _arcball_Prev_Rotation;
	gmtl::Vec3f _arcball_Current_Rotation;

	//Scene tranformation Matrices
	osg::Matrix sceneScale, sceneRotate, sceneTranslate;
	float scene_Scale_Factor;

	gmtl::Point3f _normalized_Values_Conversion;

	int _random_Trial;
	int _trial_Counter;

	osg::ref_ptr<osg::MatrixTransform> _expt_tf_nd;
	osg::ref_ptr<osg::MatrixTransform> _on_ScreenText_tf_nd;
	osg::ref_ptr<osg::MatrixTransform> _trialCounter_Text_tf_nd;
	osg::ref_ptr<osg::MatrixTransform> _session_Name_tf_nd;
	osg::ref_ptr<osg::MatrixTransform> _practice_Instruction_tf_nd;
	osg::ref_ptr<osg::MatrixTransform> _task_Text_tf_nd;

	int _SUBJECT_ID;
	std::string _subjectFolder;
	std::string _subjectConfig;

	float keyboardX, keyboardY, keyboardZ;

	//int _session_Counter;
	std::size_t _session_Counter;
	//int _subsessions_Counter;
	std::size_t _subsessions_Counter;

	float _scene_Width;
	float _scene_Height;

	double _scene_X_Min, _scene_X_Max, _scene_Y_Min, _scene_Y_Max, _scene_Z_Min, _scene_Z_Max;
	double _device_X_Min, _device_X_Max, _device_Y_Min, _device_Y_Max, _device_Z_Min, _device_Z_Max;

	double _user_Cursor_X, _user_Cursor_Y;

	osg::Matrixf _view_Mat, _proj_Mat;

	osg::ref_ptr<osg::MatrixTransform> _cursor_tf_nd;

	std::vector<osg::ref_ptr<osgFX::Outline>> _object_Outlines;
	gmtl::Point3f _object_Size;
	gmtl::Point3f _target_1_Position, _target_2_Position;

	gmtl::Point3f _target_1_Min, _target_1_Max, _target_2_Min, _target_2_Max;

	int _practice_Instructions_Display_Count;

	bool _start_Object_Selected = false;
	bool _target_Object_Selected = false;

	bool _on_Start_Object = false;
	bool _on_Target_Object = false;

	clock_t _start_Time;

	double _startObject_Selected_Time, _targetObject_Selected_Time;

	bool _show_Expt_Info;
	bool _start_Session;
	bool _show_Device_Desc;
	bool _show_Session_Info;
	bool _show_Practice_Info;
	bool _show_Task_Info;
	bool _show_ExptSession_Info;
	bool _show_Break_Info;
	bool _practice_Session;
	bool _interactive_Task_Session;
	bool _experiment_Session;

	bool _user_Tapped = false;

	int _practice_Trial_Counter = 1;

	int temp = 0;

	TouchScreenRes screenRes;
	int _screen_Number;

	/*****************************
	** Experiment Subject Stats **
	*****************************/
	bool _first_Touch;
	gmtl::Point3f _first_TouchPoint;

	double _task_StartTime, _task_EndTime, _task_TotalTime;

	double _initial_Touch_Distance;
	double _first_TouchTime, _start_ObjectAcquiredTime;

	int _number_StartObject_Misses, _number_TargetObject_Misses;

	std::string _interactionFinger;

	Experiment::subsessionResults taskResult;

	std::vector<std::pair<int, Experiment::subsessionResults >> results;

	std::pair<int, Experiment::subsessionResults> trialResult;

	std::fstream _resultsFileStream;

	std::map < std::string, std::vector<Experiment::subsessionResults>> _user_Results;

	std::string _resultsFile;
};

#endif
