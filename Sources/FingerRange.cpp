//|___________________________________________________________________
//!
//! \file FingerRange.cpp
//!
//! Author: Prabhakar Vemavarapu
//|___________________________________________________________________

//| Includes
#include "FingerRange.h"

#define Epsilon 0.001f

#define ONED_TASK
#define NUMBER_PRACTICE_TRIALS 5
#define EXPERIMENT_SETUP
//#define CONSTANT_WIDTH

//| Global Variables

extern FingerRange *application;                 // Reference to vrJuggler application instance defined in main.cpp

osgViewer::Viewer* _thisViewer;

/******************************************************
** Add --stereo to the debugging argument for stereo **
/*****************************************************/

FingerRange::FingerRange() {
}

//| Function: ~FingerRange
//|____________________________________________________________________
FingerRange::~FingerRange() {
}

//| Function: initScene
//|____________________________________________________________________
void FingerRange::initScene() {
	//void FingerRange::initScene(osgViewer::Viewer viewer){
	std::srand(unsigned(std::time(0)));

	_experiment = new Experiment();
	//_ss = new ServerSocket();

	_practice_Instructions_Display_Count = 0;

	taskResult = {};
	results.clear();

	// Initializes data	
	scene_Scale_Factor = 1.0f;

	_scene_Width = 960;
	_scene_Height = 540;

	_interaction_Window_Width = 2560;
	_interaction_Window_Height = 1440;

	_connected_Devices_Counter = 0;

	//****************************
	// Commented for Pilot Study
	//****************************
	/*std::cout << "Enter the subject ID: ";
	std::cin >> _SUBJECT_ID;*/

	_SUBJECT_ID = 6;



#ifdef ONED_TASK
	_practice_Object_Positions = _experiment->GetObjectPositions("Resources/Experiment_Config/1D_Practice_Task_Positions.txt", "PRACTICE");
	_practice_Object_Sizes = _experiment->GetObjectSizes("Resources/Experiment_Config/1D_Practice_Object_Sizes.txt", "PRACTICE");

	_experiment_ID_Values = _experiment->GetIDValues("Resources/Experiment_Config/1D_ID_Task_Values.txt", "EXPERIMENT");

	_experiment_Object_Positions.resize(_experiment_ID_Values.size());

	for (int i = 0; i < _experiment_Object_Positions.size(); i++) {
		_experiment_Object_Positions.at(i).push_back(gmtl::Point3f(_experiment_ID_Values.at(i).at(0)[0], _experiment_ID_Values.at(i).at(0)[1], _experiment_ID_Values.at(i).at(0)[2]));
		_experiment_Object_Positions.at(i).push_back(gmtl::Point3f(_experiment_ID_Values.at(i).at(1)[0], _experiment_ID_Values.at(i).at(1)[1], _experiment_ID_Values.at(i).at(1)[2]));
		_experiment_Object_Sizes.push_back(gmtl::Point3f(_experiment_ID_Values.at(i).at(2)[0], _experiment_ID_Values.at(i).at(2)[1], _experiment_ID_Values.at(i).at(2)[2]));
	}

	_trial_Order = _experiment->GetTrialOrder();
	_object_Colors = _experiment->GetObjectColors();

	std::cout << "\nPreparing Subject " << _SUBJECT_ID << " Experiment Data" << std::endl;

	_experiment->PrepareExperimentData(_SUBJECT_ID);
	std::cout << "Done preparing experiment data for subject " << _SUBJECT_ID << std::endl;
	_sessions_Order = _experiment->GetSessionOrder(_SUBJECT_ID);
	_subjectFolder = _experiment->getSubjectFolder();

	_session_Texts.push_back(_session_1_Desc_Text);
	_session_Texts.push_back(_session_2_Desc_Text);
#endif // ONED_TASK	

	if (_sessions_Order.at(0) == 1) {
		_screen_Number = 1;

		_subsessions_Order.push_back(1);
		_subsessions_Order.push_back(2);

		_subsession_Practice_Texts.push_back(_session_1_LT_PRACTICE);
		_subsession_Practice_Texts.push_back(_session_1_RT_PRACTICE);
		_subsession_Expt_Texts.push_back(_session_1_LT_EXPT);
		_subsession_Expt_Texts.push_back(_session_1_RT_EXPT);

		_subsession_On_Screen_Text.push_back(_session_1_LT_On_Screen_Text);
		_subsession_On_Screen_Text.push_back(_session_1_RT_On_Screen_Text);

		_subsession_Names.push_back("LT");
		_subsession_Names.push_back("RT");

	}
	else {
		_screen_Number = 2;

		_subsessions_Order.push_back(1);
		_subsessions_Order.push_back(2);
		_subsessions_Order.push_back(3);
		_subsessions_Order.push_back(4);

		_subsession_Practice_Texts.push_back(_session_2_LI_PRACTICE);
		_subsession_Practice_Texts.push_back(_session_2_LM_PRACTICE);
		_subsession_Practice_Texts.push_back(_session_2_RI_PRACTICE);
		_subsession_Practice_Texts.push_back(_session_2_RM_PRACTICE);

		_subsession_Expt_Texts.push_back(_session_2_LI_EXPT);
		_subsession_Expt_Texts.push_back(_session_2_LM_EXPT);
		_subsession_Expt_Texts.push_back(_session_2_RI_EXPT);
		_subsession_Expt_Texts.push_back(_session_2_RM_EXPT);

		_subsession_On_Screen_Text.push_back(_session_2_LI_On_Screen_Text);
		_subsession_On_Screen_Text.push_back(_session_2_LM_On_Screen_Text);
		_subsession_On_Screen_Text.push_back(_session_2_RI_On_Screen_Text);
		_subsession_On_Screen_Text.push_back(_session_2_RM_On_Screen_Text);

		_subsession_Names.push_back("LI");
		_subsession_Names.push_back("LM");
		_subsession_Names.push_back("RI");
		_subsession_Names.push_back("RM");
	}

	std::random_shuffle(_subsessions_Order.begin(), _subsessions_Order.end());

	_first_Touch = false;

	_show_Expt_Info = true;
	_show_Break_Info = false;

	_subsessions_Counter = 0;
	_trial_Counter = 0;
	_random_Trial = _trial_Order.at(_trial_Counter);
	int temp;

	_practice_Session = true;
	_interactive_Task_Session = true;

	if (_practice_Session) {
#ifdef CONSTANT_WIDTH
		_object_Size = gmtl::Point3f(1.5f, 2.0f, 0.0f);
#else
		_object_Size = gmtl::Point3f(0.25, 2.0, 0.0);// _practice_Object_Sizes.at(rand() % _practice_Object_Sizes.size());
#endif
		temp = rand() % _practice_Object_Positions.size();
		_target_1_Position = _practice_Object_Positions.at(temp)[0];
		_target_2_Position = _practice_Object_Positions.at(temp)[1];
	}
	else {
#ifdef CONSTANT_WIDTH
		_object_Size = gmtl::Point3f(1.0f, 3.0f, 0.0f);
#else
		_object_Size = _experiment_Object_Sizes.at(_random_Trial / _experiment_Object_Positions.size());
#endif

		temp = _random_Trial % _experiment_Object_Positions.size();

		_target_1_Position = _experiment_Object_Positions.at(temp)[0];
		_target_2_Position = _experiment_Object_Positions.at(temp)[1];
	}

	_target_1_Min = _target_1_Position - (_object_Size / 2.0f);
	_target_1_Max = _target_1_Position + (_object_Size / 2.0f);
	_target_2_Min = _target_2_Position - (_object_Size / 2.0f);
	_target_2_Max = _target_2_Position + (_object_Size / 2.0f);

	// Creates scene graph
	createSceneGraph();
	_practice_Session = false;

	_session_Counter = 0;
	_start_Session = false;
	_show_Device_Desc = false;
	_show_Session_Info = false;

	_on_Object_Time = 0.0f;
	showExperimentInfo();
}

void FingerRange::setSubjectFolder(std::string folderName) {
	_subjectFolder = folderName;
}

void FingerRange::showExperimentInfo() {
	if (_show_Expt_Info) {
		_desc_nd = new osg::Geode();
		_desc_txt_nd = new osgText::Text();

		_desc_txt_nd->setText("");
		_desc_txt_nd->setColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
		_desc_txt_nd->setFont("Resources/Fonts/arial.ttf");
		_desc_txt_nd->setCharacterSize(0.55f);
		_desc_txt_nd->setAlignment(osgText::Text::CENTER_CENTER);
		_desc_txt_nd->setAxisAlignment(osgText::TextBase::SCREEN);

		if (_session_Counter == 0)
			_desc_txt_nd->setText(_welcome_Text);

		_desc_nd->addDrawable(_desc_txt_nd.get());

		osg::Matrixf osg_m;
		_expt_tf_nd = new osg::MatrixTransform();

		osg_m.makeTranslate(osg::Vec3(0.0f, 1.4f, 0.0f));
		_expt_tf_nd->setMatrix(osg_m);
		_expt_tf_nd->addChild(_desc_nd);

		_gw_tf_nd->addChild(_expt_tf_nd);

		_on_Screen_Text_nd = new osg::Geode();
		_on_Screen_Text = new osgText::Text();

		_on_Screen_Text->setText("");

		_on_Screen_Text->setColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
		_on_Screen_Text->setFont("Resources/Fonts/arial.ttf");
		_on_Screen_Text->setCharacterSize(0.55f);
		_on_Screen_Text->setAlignment(osgText::Text::CENTER_CENTER);
		_on_Screen_Text->setAxisAlignment(osgText::TextBase::SCREEN);

		_on_Screen_Text_nd->addDrawable(_on_Screen_Text.get());

		_on_ScreenText_tf_nd = new osg::MatrixTransform();

		osg_m.makeTranslate(osg::Vec3(0.0f, 10.0f, 0.0f));
		_on_ScreenText_tf_nd->setMatrix(osg_m);
		_on_ScreenText_tf_nd->addChild(_on_Screen_Text_nd);

		_scene_tf_nd->addChild(_on_ScreenText_tf_nd);

		_trialCounter_Text_nd = new osg::Geode();
		_trialCounter_Text = new osgText::Text();

		_trialCounter_Text->setText("Trial No: ");

		_trialCounter_Text->setColor(osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
		_trialCounter_Text->setFont("Resources/Fonts/arial.ttf");
		_trialCounter_Text->setCharacterSize(0.50f);
		_trialCounter_Text->setAlignment(osgText::Text::CENTER_CENTER);
		_trialCounter_Text->setAxisAlignment(osgText::TextBase::SCREEN);

		_trialCounter_Text_nd->addDrawable(_trialCounter_Text.get());

		_trialCounter_Text_tf_nd = new osg::MatrixTransform();

		osg_m.makeTranslate(osg::Vec3(-14.0f, 7.5f, 0.0f));
		_trialCounter_Text_tf_nd->setMatrix(osg_m);
		_trialCounter_Text_tf_nd->addChild(_trialCounter_Text_nd);

		_trialCounter_Text_tf_nd->setNodeMask(0x0);
		_scene_tf_nd->addChild(_trialCounter_Text_tf_nd);

		_session_Name_tf_nd = new osg::MatrixTransform();

		osg::ref_ptr<osg::Geode> session_Text_Quad_Geode = new osg::Geode;
		osg::ref_ptr<osg::ShapeDrawable> session_Quad_Shape = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 0.0f, -0.15f), 6.875f, 1.125f, 0.0f));
		session_Quad_Shape->setColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 0.1f));

		session_Text_Quad_Geode->addDrawable(session_Quad_Shape.get());
		_session_Name_tf_nd->addChild(session_Text_Quad_Geode.get());

		_session_Text_nd = new osg::Geode();
		_session_Text = new osgText::Text();

		_session_Text->setText("TASK DESCRIPTION");

		_session_Text->setColor(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
		_session_Text->setFont("Resources/Fonts/arialbd.ttf");
		_session_Text->setCharacterSize(0.55f);
		_session_Text->setAlignment(osgText::Text::CENTER_CENTER);
		_session_Text->setAxisAlignment(osgText::TextBase::SCREEN);

		session_Text_Quad_Geode->addDrawable(_session_Text.get());

		osg_m.makeTranslate(osg::Vec3(14.0f, 9.5f, 0.0f));
		_session_Name_tf_nd->setMatrix(osg_m);
		_session_Name_tf_nd->addChild(_session_Text_nd);

		_scene_tf_nd->addChild(_session_Name_tf_nd);

		_practice_Instruction_tf_nd = new osg::MatrixTransform();

		_practice_Instruction_Text_nd = new osg::Geode();
		_practice_Instruction_Text = new osgText::Text();

		_practice_Instruction_Text->setText("");

		_practice_Instruction_Text->setColor(osg::Vec4f(0.5f, 0.5f, 0.5f, 1.0f));
		_practice_Instruction_Text->setFont("Resources/Fonts/arial.ttf");
		_practice_Instruction_Text->setCharacterSize(0.4f);
		_practice_Instruction_Text->setAlignment(osgText::Text::CENTER_CENTER);
		_practice_Instruction_Text->setAxisAlignment(osgText::TextBase::SCREEN);

		_practice_Instruction_Text_nd->addDrawable(_practice_Instruction_Text.get());

		osg_m.makeTranslate(osg::Vec3(0.0f, -10.0f, 0.0f));
		_practice_Instruction_tf_nd->setMatrix(osg_m);
		_practice_Instruction_tf_nd->addChild(_practice_Instruction_Text_nd);

		_scene_tf_nd->addChild(_practice_Instruction_tf_nd.get());
		//_gw_tf_nd->addChild(_practice_Instruction_tf_nd.get());

		_task_Text_nd = new osg::Geode();
		_task_Text = new osgText::Text();

		_task_Text->setText("");

		_task_Text->setColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
		_task_Text->setFont("Resources/Fonts/arial.ttf");
		_task_Text->setCharacterSize(0.35f);
		_task_Text->setAlignment(osgText::Text::CENTER_CENTER);
		_task_Text->setAxisAlignment(osgText::TextBase::SCREEN);

		_task_Text_nd->addDrawable(_task_Text.get());

		_task_Text_tf_nd = new osg::MatrixTransform();

		osg_m.makeTranslate(osg::Vec3(-16.5f, -7.35f, 0.0f));
		_task_Text_tf_nd->setMatrix(osg_m);
		_task_Text_tf_nd->addChild(_task_Text_nd);

		_task_Text_tf_nd->setNodeMask(0x0);
		_scene_tf_nd->addChild(_task_Text_tf_nd);
	}
}

//| Function: createSceneGraph
void FingerRange::createSceneGraph() {
	osg::Matrixf osg_m;                 // Temp storage for transfering to and from a gmtl matrix

										//| Upper nodes
	_root_nd = new osg::Group();
	_uw_tf_nd = new osg::MatrixTransform();
	_gw_tf_nd = new osg::MatrixTransform();

	_scene_tf_nd = new osg::MatrixTransform();
	_grid_tf_nd = new osg::MatrixTransform();

	osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;
	shadowedScene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
	shadowedScene->setCastsShadowTraversalMask(CastsShadowTraversalMask);

	osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
	shadowedScene->setShadowTechnique(sm.get());

	_root_nd->addChild(_gw_tf_nd.get());

	sceneTranslate.makeTranslate(osg::Vec3f(_scene_Width, _scene_Height, 0.0));
	sceneRotate.makeRotate(osg::DegreesToRadians(90.0f), osg::Vec3f(1.0f, 0.0f, 0.0f));
	sceneScale.makeScale(scene_Scale_Factor, scene_Scale_Factor, scene_Scale_Factor);
	_scene_tf_nd->setMatrix(sceneRotate * sceneScale);

	_gw_tf_nd->addChild(_scene_tf_nd.get());

	// Enables lighting in entire scene
	osg::StateSet* state = _root_nd->getOrCreateStateSet();
	state->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	state->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);

	// Creates object group node
	_object_group_nd = new osg::Group();
	_scene_tf_nd->addChild(_object_group_nd.get());

	// Creates objects
	int error_count = 0;
	int count = 0;

	for (unsigned int i = 0; i < 2;) {
		// Creates ObjectDisplay node
		_object_display_nd = new ObjectDisplay();

		if (!_object_display_nd->createSceneObject(_object_Size, _object_Colors.at(0)[i])) {
			printf("FingerRange::createSceneGraph --> ERROR: Unable to initilize ObjectDisplay data.\n");
			error_count++;
			continue;
		}
		else {
			gmtl::Point3f p;

			if (i == 0)
				p = _target_1_Position;
			else
				p = _target_2_Position;

			osg::ref_ptr<osgFX::Outline> outline = new osgFX::Outline;

			outline->setWidth(10.0f);
			outline->setColor(osg::Vec4(0.9529f, 0.9529f, 0.0823f, 1.0f));

			outline->addChild(_object_display_nd.get());

			_object_Outlines.push_back(outline);

			_scene_tf_nd->addChild(outline);

			outline->setNodeMask(0x0);

			_object_display_nd->setObjectPosition(p[0], p[1], p[2]);//
			_object_group_nd->addChild(_object_display_nd.get());

			_object_display_nodes.push_back(_object_display_nd);
			i++;
		}
	}

	std::cout << "INIT SCENE DISPLAY DONE" << std::endl;

	const clock_t begin_time = clock();

	_cursor_Node = createUserCursor();

	_cursor_tf_nd = new osg::MatrixTransform();

	keyboardX = _target_1_Position[0] - 0.5;
	keyboardY = _target_1_Position[1];
	keyboardZ = 0.05f;

	//osg_m.setTrans(keyboardX, keyboardY, keyboardZ);
	//_cursor_tf_nd->setMatrix(osg_m);

	_cursor_tf_nd->addChild(_cursor_Node.get());

	_scene_tf_nd->addChild(_cursor_tf_nd.get());

	if (_show_Expt_Info)
		_scene_tf_nd->setNodeMask(0x0);
}

void FingerRange::setSceneBounds(float l, float r, float t, float b, float n, float f) {
	_scene_X_Min = l;
	_scene_X_Max = r;
	_scene_Y_Min = b;
	_scene_Y_Max = t;
	_scene_Z_Min = n;
	_scene_Z_Max = f;

	std::cout << "SCENE BOUNDS SET" << std::endl;
}

void FingerRange::setViewMatrix(osg::Matrixf mat) {
	_view_Mat = mat;
}

void FingerRange::setProjectionMatrix(osg::Matrixf mat) {
	_proj_Mat = mat;
}

void FingerRange::registerTouchScreens(int screen_Number, int sx, int sy) {
	std::cout << "Registering Touch Screens" << std::endl;

	//	_screen_Number = screen_Number;

	screenRes.x = sx;
	screenRes.y = sy;

	_touchScreen_Res.push_back(screenRes);

	_touchScreen_Resolution.insert(std::make_pair(_connected_Devices_Counter, screenRes));

	//SetNormalizeFactors(_sessions_Order.at(_session_Counter));
	SetNormalizeFactors(screen_Number);

	_connected_Devices_Counter++;

	//	std::cout << "SIZE: " << _touchScreen_Res.size() << std::endl;
}

std::vector<FingerRange::TouchScreenRes> FingerRange::getDeviceResolution() {
	return _touchScreen_Res;
}

void FingerRange::SetNormalizeFactors() {
	_device_X_Max = (float)_touchScreen_Resolution[0].x;
	_device_Y_Max = (float)_touchScreen_Resolution[0].y;
	_device_Z_Max = (float)_touchScreen_Resolution[0].x;

	std::cout << "TX: " << _device_X_Max << ", TY: " << _device_Y_Max << ", TZ: " << _device_Z_Max << std::endl;

	_device_X_Min = 0.0f;
	_device_Y_Min = 0.0f;
	_device_Z_Min = 0.0f;

	std::cout << "SceneXMax: " << _scene_X_Max << ", SceneXMin: " << _scene_X_Min << "\nSceneYMax: " << _scene_Y_Max << ", SceneYMin: " << _scene_Y_Min << "\nSceneZMax: " << _scene_Z_Max << ", SceneZMin: " << _scene_Z_Min << std::endl;

	_normalize_Factors[0] = (_scene_X_Max - _scene_X_Min) / (_device_X_Max - _device_X_Min);
	_normalize_Factors[1] = (_scene_Y_Max - _scene_Y_Min) / (_device_Y_Max - _device_Y_Min);
	_normalize_Factors[2] = (_scene_Z_Max - _scene_Z_Min) / (_device_Z_Max - _device_Z_Min);

	std::cout << "NOMALIZE FACTORS -- [0]: " << _normalize_Factors[0] << ", [1]: " << _normalize_Factors[1] << ", [2]: " << _normalize_Factors[2] << std::endl;
}

void FingerRange::SetNormalizeFactors(int sessionNumber) {
	float maxX, maxY, maxZ;

	if ((sessionNumber - 1) == 0) {
		maxX = (float)(float)_touchScreen_Resolution[0].x;
		maxY = (float)(float)_touchScreen_Resolution[0].y;
		maxZ = (float)(float)_touchScreen_Resolution[0].x;
	}
	else if ((sessionNumber - 1) == 1) {
		maxX = (float)(float)_touchScreen_Resolution[1].x;
		maxY = (float)(float)_touchScreen_Resolution[1].y;
		maxZ = (float)(float)_touchScreen_Resolution[1].x;
	}

	std::cout << "TX: " << maxX << ", TY: " << maxY << ", TZ: " << maxZ << std::endl;

	float minX = 0.0f;
	float minY = 0.0f;
	float minZ = 0.0f;

	_normalize_Factors[0] = (_scene_X_Max - _scene_X_Min) / (maxX - minX);
	_normalize_Factors[1] = (_scene_Y_Max - _scene_Y_Min) / (maxY - minY);
	_normalize_Factors[2] = (_scene_Z_Max - _scene_Z_Min) / (maxZ - minZ);

	std::cout << "A: " << _normalize_Factors[0] << ", B: " << _normalize_Factors[1] << ", C: " << _normalize_Factors[2] << std::endl;
}

void FingerRange::showInteractivePracticeTask() {
	_on_Object_Time = 1.5f;

	osg::Matrixf osg_m;

	_desc_txt_nd->setText(_task_Desc_Text);
	delayTimer(1.0f);

	_target_1_Position = gmtl::Point3f(-7.5, 0.0, 0.0);
	_target_2_Position = gmtl::Point3f(7.5, 0.0, 0.0);

	_object_Size = gmtl::Point3f(0.25, 2.0, 0.0);// _object_Size = _practice_Object_Sizes.at(rand() % _practice_Object_Sizes.size());

	_target_1_Min = _target_1_Position - (_object_Size / 2.0f);
	_target_1_Max = _target_1_Position + (_object_Size / 2.0f);
	_target_2_Min = _target_2_Position - (_object_Size / 2.0f);
	_target_2_Max = _target_2_Position + (_object_Size / 2.0f);

	setObjectPosition(_object_Size, _target_1_Position, _target_2_Position);

	_desc_txt_nd->setText("TASK\n----\nTask comprises of selecting two rectangles - first a grey start object, then a blue target object.");
	delayTimer(1.0f);

	_scene_tf_nd->setNodeMask(0xffffffff);

	_start_Object_Selected = false;

	_practice_Instruction_Text->setText("Touch and move your finger on the\nTOP TOUCH SURFACE\nto see the highlight around this object in yellow color");
	_practice_Instruction_Text->setColor(osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));

	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	osg::Vec4Array* colors = new osg::Vec4Array;

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> outlineGeom = new osg::Geometry;

	osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth();
	linewidth->setWidth(5.0f);

	if (_start_Object_Selected == false) {
		osg_m.makeTranslate(osg::Vec3(_target_1_Position[0] - 3.0f, _target_1_Position[1] + 3.0f, 0.0f));

		v->push_back(osg::Vec3(_target_1_Position[0] - 2.0f, _target_1_Position[1] + 2.0f, 0.0f));
		v->push_back(osg::Vec3(_target_1_Position[0] - 2.3f, _target_1_Position[1] + 1.7f, 0.0f));
		v->push_back(osg::Vec3(_target_1_Position[0] - 1.4f, _target_1_Position[1] + 0.8f, 0.0f));
		v->push_back(osg::Vec3(_target_1_Position[0] - 1.7f, _target_1_Position[1] + 0.5f, 0.0f));
		v->push_back(osg::Vec3(_target_1_Position[0] - 0.8f, _target_1_Position[1] + 0.5f, 0.0f));
		v->push_back(osg::Vec3(_target_1_Position[0] - 0.8f, _target_1_Position[1] + 1.4f, 0.0f));
		v->push_back(osg::Vec3(_target_1_Position[0] - 1.1f, _target_1_Position[1] + 1.1f, 0.0f));
		//v->push_back(osg::Vec3(_target_1_Position[0] - 2.0f, _target_1_Position[1] + 2.0f, 0.0f));

		outlineGeom->setVertexArray(v.get());

		colors->push_back(osg::Vec4(0.2f, 1.0f, 0.6f, 1.0f));
		outlineGeom->setColorArray(colors);
		outlineGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

		outlineGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, v->size()));
		outlineGeom->getOrCreateStateSet()->setAttributeAndModes(linewidth, osg::StateAttribute::ON);
		outlineGeom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

		geode->addDrawable(outlineGeom.get());

		_scene_tf_nd->addChild(geode.get());
	}
	else if (_start_Object_Selected == true && _target_Object_Selected == false) {
		_practice_Instruction_Text->setText("Now, move your finger on the \nTOP TOUCH SURFACE\nto see the highlight around this object in yellow color");
		osg_m.makeTranslate(osg::Vec3(_target_2_Position[0] + 3.0f, _target_2_Position[1] - 3.0f, 0.0f));

		osg::ref_ptr<osg::Geode> geode = new osg::Geode;

		osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth();
		linewidth->setWidth(5.0f);

		osg::ref_ptr<osg::Geometry> outlineGeom = new osg::Geometry;

		v->push_back(osg::Vec3(_target_2_Position[0] + 2.0f, _target_2_Position[1] - 2.0f, 0.0f));
		v->push_back(osg::Vec3(_target_2_Position[0] + 1.7f, _target_2_Position[1] - 1.7f, 0.0f));
		v->push_back(osg::Vec3(_target_2_Position[0] + 2.6f, _target_2_Position[1] - 0.8f, 0.0f));
		v->push_back(osg::Vec3(_target_2_Position[0] + 2.3f, _target_2_Position[1] - 0.5f, 0.0f));
		v->push_back(osg::Vec3(_target_2_Position[0] + 3.2f, _target_2_Position[1] - 0.5f, 0.0f));
		v->push_back(osg::Vec3(_target_2_Position[0] + 3.2f, _target_2_Position[1] - 1.4f, 0.0f));
		v->push_back(osg::Vec3(_target_2_Position[0] + 2.9f, _target_2_Position[1] - 1.1f, 0.0f));
		//v->push_back(osg::Vec3(_target_1_Position[0] - 2.0f, _target_1_Position[1] + 2.0f, 0.0f));

		outlineGeom->setVertexArray(v.get());

		osg::Vec4Array* colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(0.2f, 1.0f, 0.6f, 1.0f));
		outlineGeom->setColorArray(colors);
		outlineGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

		outlineGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, v->size()));
		outlineGeom->getOrCreateStateSet()->setAttributeAndModes(linewidth, osg::StateAttribute::ON);
		outlineGeom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

		geode->addDrawable(outlineGeom.get());
	}

	_practice_Instruction_tf_nd->setMatrix(osg_m);

	_cursor_tf_nd->setNodeMask(0x0);
}

void FingerRange::updatePracticeObjects() {
	int pos = rand() % _practice_Object_Positions.size();

	_target_1_Position = _practice_Object_Positions.at(pos)[0];
	_target_2_Position = _practice_Object_Positions.at(pos)[1];

	_object_Size = gmtl::Point3f(0.25, 2.0, 0.0);// _object_Size = _practice_Object_Sizes.at(rand() % _practice_Object_Sizes.size());

	_target_1_Min = _target_1_Position - (_object_Size / 2.0f);
	_target_1_Max = _target_1_Position + (_object_Size / 2.0f);
	_target_2_Min = _target_2_Position - (_object_Size / 2.0f);
	_target_2_Max = _target_2_Position + (_object_Size / 2.0f);

	setObjectPosition(_object_Size, _practice_Object_Positions.at(pos)[0], _practice_Object_Positions.at(pos)[1]);

	_start_Object_Selected = false;

	if (_practice_Instructions_Display_Count < 3) {
		if (_sessions_Order.at(_session_Counter) == 1)
			_practice_Instruction_Text->setText(_start_Object_Instruction1 + "\nTOP TOUCH SURFACE\n" + _start_Object_Instruction2);
		if (_sessions_Order.at(_session_Counter) == 2)
			_practice_Instruction_Text->setText(_start_Object_Instruction1 + "\nBOTTOM TOUCH SURFACE\n" + _start_Object_Instruction2);

		_practice_Instruction_Text->setColor(osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));

		osg::Matrixf osg_m;
		osg_m.makeTranslate(osg::Vec3(_target_1_Position[0], _target_1_Position[1] - 2.0f, 0.0f));
		_practice_Instruction_tf_nd->setMatrix(osg_m);
	}
	else if (_practice_Instructions_Display_Count == 3)
		_practice_Instruction_tf_nd->setNodeMask(0x0);

	_cursor_tf_nd->setNodeMask(0x0);
}

void FingerRange::updateExperimentObjects() {
	std::cout << "TC: " << _trial_Counter << ", TOS - 1: " << _trial_Order.size() - 1 << std::endl;

	if (_trial_Counter < _trial_Order.size() - 1 || _trial_Counter < 0) {
		_trial_Counter++;

		std::string str = "Trial No: ";
		str.append(std::to_string(_trial_Counter + 1));

		_trialCounter_Text->setText(str);

		_random_Trial = _trial_Order.at(_trial_Counter);
		//_object_Size = _experiment_Object_Sizes.at(_random_Trial / _experiment_Object_Positions.size());

		int temp = _random_Trial % _experiment_Object_Positions.size();

		_target_1_Position = _experiment_Object_Positions.at(temp)[0];
		_target_2_Position = _experiment_Object_Positions.at(temp)[1];

		_object_Size = gmtl::Point3f(0.25, 2.0, 0.0); //_experiment_ID_Values.at(_random_Trial).at(2);

		_target_1_Min = _target_1_Position - (_object_Size / 2.0f);
		_target_1_Max = _target_1_Position + (_object_Size / 2.0f);
		_target_2_Min = _target_2_Position - (_object_Size / 2.0f);
		_target_2_Max = _target_2_Position + (_object_Size / 2.0f);

		setObjectPosition(_object_Size, _experiment_Object_Positions.at(temp)[0], _experiment_Object_Positions.at(temp)[1]);

		_task_Text->setText(std::string("D: ").append(std::to_string(gmtl::length(gmtl::Vec3f(_target_1_Position - _target_2_Position))).substr(0, 5)).append(",  W: ").append(std::to_string(_object_Size[0]).substr(0, 5)));

		_number_StartObject_Misses = 0;
		_number_TargetObject_Misses = 0;
		_initial_Touch_Distance = 0.0f;
		_start_ObjectAcquiredTime = 0.0f;

		taskResult = {};

		taskResult.startObjectSize = _object_Size;
		taskResult.targetObjectSize = _object_Size;

		taskResult.distance = gmtl::length(gmtl::Vec3f(_target_1_Position - _target_2_Position));

		taskResult.indexofDifficulty = log2((float)(taskResult.distance / _object_Size[0]) + 1);
		//		std::cout << "Distance: " << taskResult.distance << ", OBJECT WIDTH: " << _object_Size << ", ID: " << taskResult.indexofDifficulty << std::endl;

		_start_Object_Selected = false;
		_first_Touch = false;

		keyboardX = _target_1_Position[0] - 0.5;
		keyboardY = _target_1_Position[1];
		keyboardZ = 0.05f;

		osg::Matrixf osg_m;
		osg_m.setTrans(keyboardX, keyboardY, keyboardZ);
		_cursor_tf_nd->setMatrix(osg_m);
	}
	else if ((_subsessions_Counter + 1) < _subsessions_Order.size()) {
		/*_subsessions_Counter++;
		if (_subsessions_Counter < _subsessions_Order.size()){*/
		_subsessions_Counter++;

		_scene_tf_nd->setNodeMask(0x0);
		_expt_tf_nd->setNodeMask(0xffffffff);
		_trialCounter_Text_tf_nd->setNodeMask(0x0);
		_task_Text_tf_nd->setNodeMask(0x0);

		_show_Practice_Info = true;
		_experiment_Session = false;

		_practice_Instructions_Display_Count = 0;
		_practice_Trial_Counter = 0;

		std::cout << "RESULTS FILE : " << _resultsFile << std::endl;
		std::cout << "RESULTS SIZE : " << results.size() << std::endl;

		//std::cout << "RESULTS FOR " << _interactionFinger << "ARE: " << std::endl;
		//for(int i = 0; i < results.size(); i++)
		//	std::cout << results.at(i).startObjectSize << ", " << results.at(i).targetObjectSize << ", " << results.at(i).indexofDifficulty << ", " << results.at(i).taskCompletionTime << ", " << results.at(i).totalMisses << std::endl;

		_resultsFileStream.close();

		_interactionFinger = _subsession_Names.at(_subsessions_Order.at(_subsessions_Counter) - 1);
		std::cout << "INTERACTION FINGER: " << _interactionFinger << std::endl;

		_resultsFile = _subjectFolder + "/" + _interactionFinger + ".txt";

		_resultsFileStream.open(_resultsFile, ios::app);

		_resultsFileStream << "TRIAL NO" << "\t" << "Start Object Size" << "\t" << "Target Object Size" << "\t" << "Distance" << "\t" << "Index of Difficulty" << "\t" << "Task Completion Time" << "\t" << "Total Misses" << "\t" << "First Touch Distance" << "\t" << "Start Object Selection Time" << "\t" << "Start Object Misses" << "\t" << "Target Object Misses" << std::endl;

		//_desc_txt_nd->setText(_session_1_RT_PRACTICE);
		_desc_txt_nd->setText(_subsession_Practice_Texts.at(_subsessions_Order.at(_subsessions_Counter) - 1));
		//		}
	}
	else if (_session_Counter < _sessions_Order.size()) {
		std::cout << "RESULTS FILE : " << _resultsFile << std::endl;
		std::cout << "RESULTS SIZE : " << results.size() << std::endl;

		_scene_tf_nd->setNodeMask(0x0);
		_expt_tf_nd->setNodeMask(0xffffffff);
		_trialCounter_Text_tf_nd->setNodeMask(0x0);
		_task_Text_tf_nd->setNodeMask(0x0);

		_resultsFileStream.close();

		_practice_Instructions_Display_Count = 0;
		_practice_Trial_Counter = 0;

		_session_Counter++;

		_show_Break_Info = true;

		_scene_tf_nd->setNodeMask(0x0);
		_expt_tf_nd->setNodeMask(0xffffffff);

		_desc_txt_nd->setText(_session_Break);// _session_1_Desc_Text);

		resetCounters();
	}
	else {
		std::cout << "RESULTS FILE : " << _resultsFile << std::endl;
		std::cout << "RESULTS SIZE : " << results.size() << std::endl;

		_resultsFileStream.close();

		_scene_tf_nd->setNodeMask(0x0);
		_expt_tf_nd->setNodeMask(0xffffffff);

		_desc_txt_nd->setText(_expt_End);
	}
	_cursor_tf_nd->setNodeMask(0x0);
}

void FingerRange::setObjectPosition(gmtl::Point3f size, gmtl::Point3f pos1, gmtl::Point3f pos2) {
	_object_display_nodes.at(0)->setScale(size[0], size[1], size[2], pos1[0], pos1[1], pos1[2]);
	_object_display_nodes.at(1)->setScale(size[0], size[1], size[2], pos2[0], pos2[1], pos2[2]);

	for (std::size_t i = 0; i < _object_Outlines.size(); i++)
		_object_Outlines[i]->setNodeMask(0x0);
}

osg::Geode* FingerRange::createUserCursor() {
	osg::Geode* userCursor = new osg::Geode;

	osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth();
	linewidth->setWidth(2.0f);

	osg::ref_ptr<osg::Geometry> cursorLinesGeom = new osg::Geometry();

	osg::ref_ptr<osg::Vec3Array> cursorLineVertices = new osg::Vec3Array();
	cursorLineVertices->push_back(osg::Vec3(0.3f, 0.0f, 0.0f));
	cursorLineVertices->push_back(osg::Vec3(-0.3f, 0.0f, 0.0f));
	cursorLineVertices->push_back(osg::Vec3(0.0f, 0.3f, 0.0f));
	cursorLineVertices->push_back(osg::Vec3(0.0f, -0.3f, 0.0f));

	cursorLinesGeom->setVertexArray(cursorLineVertices);

	osg::ref_ptr<osg::Vec4Array> cursorLineColors = new osg::Vec4Array();
	cursorLineColors->push_back(osg::Vec4(0.952f, 0.952f, 0.082f, 1.0f));

	cursorLinesGeom->setColorArray(cursorLineColors);
	cursorLinesGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

	cursorLinesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 4));
	userCursor->addDrawable(cursorLinesGeom);
	userCursor->getOrCreateStateSet()->setAttributeAndModes(linewidth, osg::StateAttribute::ON);

	return userCursor;
}

void FingerRange::updateUserCursorPosition(int screen, int x, int y) {
	/*_user_Cursor_X = _scene_X_Min + ((_scene_X_Max - _scene_X_Min) / (_device_X_Max - _device_X_Min)) * (x - _device_X_Min);
	_user_Cursor_Y = -1 * (_scene_Y_Min + ((_scene_Y_Max - _scene_Y_Min) / (_device_Y_Max - _device_Y_Min)) * (y - _device_Y_Min));*/

	if (_screen_Number == screen) {
		if (_screen_Number == 2) {
			std::cout << "-1" << std::endl;
			x = _touchScreen_Res.at(1).x - x;
		}

		_cursor_tf_nd->setNodeMask(0xffffffff);

		_user_Cursor_X = _scene_X_Min + _normalize_Factors[0] * ((float)x - _device_X_Min);
		_user_Cursor_Y = -1 * (_scene_Y_Min + _normalize_Factors[1] * ((float)y - _device_Y_Min));

		if (!_first_Touch) {
			_first_TouchPoint = gmtl::Point3f(_user_Cursor_X, _user_Cursor_Y, 0.05f);
			_first_Touch = true;

			_initial_Touch_Distance = gmtl::length(gmtl::Vec3f(_first_TouchPoint - _target_1_Position));

			taskResult.firstTouchDistance = _initial_Touch_Distance;

			_first_TouchTime = clock();
		}

		//_user_Cursor_Y = (_scene_Y_Min + ((_scene_Y_Max - _scene_Y_Min) / (_device_Y_Max - _device_Y_Min)) * (y - _device_Y_Min));

		osg::Matrix osg_m;
		osg_m.setTrans(_user_Cursor_X, _user_Cursor_Y, 0.05f);

		_cursor_tf_nd->setMatrix(osg_m);
	}
	else
		showScreenErrorAlert(screen);
}

void FingerRange::DoubleTapUpdate() {
	//std::cout << std::boolalpha;
	if (_start_Session) {
		_expt_tf_nd->setNodeMask(0x0);
		_show_Expt_Info = false;
		_show_Device_Desc = true;
		_scene_tf_nd->setNodeMask(0xffffffff);
	}
	else if (_show_Expt_Info) {
		//					_session_Counter++;
		_show_Expt_Info = false;
		_show_Device_Desc = true;

		_desc_txt_nd->setText(_device_Desc_Text);
	}
	else if (_show_Device_Desc) {
		_show_Device_Desc = false;
		_show_Task_Info = true;

		osg::Matrixf osg_m;
		osg_m.makeTranslate(osg::Vec3(0.0f, 0.0f, 8.5f));
		_expt_tf_nd->setMatrix(osg_m);

		//_desc_txt_nd->setText(_task_Desc_Text);
		showInteractivePracticeTask();
	}
	//else if (_show_Task_Info || _show_Break_Info) {
	else if (_show_Break_Info) {
		osg::Matrixf osg_m;
		osg_m.makeTranslate(osg::Vec3(0.0f, 1.4f, 0.0f));
		_expt_tf_nd->setMatrix(osg_m);

		_desc_txt_nd->setAlignment(osgText::Text::CENTER_CENTER);

		std::cout << "Displaying SESSION INFO" << std::endl;
		//_show_Task_Info = false;
		_show_Break_Info = false;

		_show_Session_Info = true;

		_desc_txt_nd->setText(_session_Texts.at(_sessions_Order.at(_session_Counter) - 1));// _session_1_Desc_Text);
	}
	else if (_show_Session_Info) {
		if (_interactive_Task_Session == true)
			std::cout << "TRUE" << std::endl;
		else if (_interactive_Task_Session == false)
			std::cout << "False" << std::endl;

		std::cout << "Displaying SUBSESSION PRACTICE INFO" << std::endl;
		_show_Session_Info = false;
		_show_Practice_Info = true;

		//_desc_txt_nd->setText(_session_1_RT_PRACTICE);
		_interactionFinger = _subsession_Names.at(_subsessions_Order.at(_subsessions_Counter) - 1);
		std::cout << "INTERACTION FINGER: " << _interactionFinger << std::endl;

		_resultsFile = _subjectFolder + "/" + _interactionFinger + ".txt";

		_resultsFileStream.open(_resultsFile, ios::app);

		_resultsFileStream << "TRIAL NO" << "\t" << "Start Object Size" << "\t" << "Target Object Size" << "\t" << "Distance" << "\t" << "Index of Difficulty" << "\t" << "Task Completion Time" << "\t" << "Total Misses" << "\t" << "First Touch Distance" << "\t" << "Start Object Selection Time" << "\t" << "Start Object Misses" << "\t" << "Target Object Misses" << std::endl;

		_desc_txt_nd->setText(_subsession_Practice_Texts.at(_subsessions_Order.at(_subsessions_Counter) - 1));
	}
	else if (_show_Practice_Info) {
		_show_Practice_Info = false;
		_practice_Session = true;

		_expt_tf_nd->setNodeMask(0x0);

		_on_Screen_Text_nd->removeDrawable(_on_Screen_Text);

		_session_Text->setText("PRACTICE SESSION\n----------------");

		_on_Screen_Text->setText(_subsession_On_Screen_Text.at(_subsessions_Order.at(_subsessions_Counter) - 1));
		_on_Screen_Text->setFontResolution(256, 256);
		_on_Screen_Text_nd->addDrawable(_on_Screen_Text);

		_practice_Instruction_tf_nd->setNodeMask(0xffffffff);

		if (_practice_Instructions_Display_Count < 3) {
			if (_sessions_Order.at(_session_Counter) == 1)
				_practice_Instruction_Text->setText(_start_Object_Instruction1 + "\nTOP TOUCH SURFACE\n" + _start_Object_Instruction2);
			if (_sessions_Order.at(_session_Counter) == 2)
				_practice_Instruction_Text->setText(_start_Object_Instruction1 + "\nBOTTOM TOUCH SURFACE\n" + _start_Object_Instruction2);

			osg::Matrixf osg_m;
			osg_m.makeTranslate(osg::Vec3(_target_1_Position[0], _target_1_Position[1] - 2.0f, 0.0f));
			_practice_Instruction_tf_nd->setMatrix(osg_m);
		}

		_scene_tf_nd->setNodeMask(0xffffffff);

		std::cout << "Interaction Screen: " << _screen_Number << std::endl;

		_cursor_tf_nd->setNodeMask(0x0);
		_on_Object_Time = 0.5f;

		// updatePracticeObjects();
	}
	else if (_practice_Session) {
		//_scene_tf_nd->removeChild(_practice_Instruction_tf_nd);
		/*_practice_Instruction_tf_nd->setNodeMask(0x0);

		_practice_Session = false;
		std::cout << "Displaying Experiment Info" << std::endl;
		_show_ExptSession_Info = true;

		_expt_tf_nd->setNodeMask(0xffffffff);
		_scene_tf_nd->setNodeMask(0x0);

		_desc_txt_nd->setText(_subsession_Expt_Texts.at(_subsessions_Order.at(_subsessions_Counter) - 1));*/
		return;
	}
	else if (_show_ExptSession_Info) {
		std::cout << "Starting Experiment" << std::endl;
		_show_ExptSession_Info = false;

		_experiment_Session = true;

		_session_Text->setText("EXPERIMENT SESSION\n------------------");

		_expt_tf_nd->setNodeMask(0x0);
		_scene_tf_nd->setNodeMask(0xffffffff);
		_trialCounter_Text_tf_nd->setNodeMask(0xffffffff);
		_task_Text_tf_nd->setNodeMask(0xffffffff);

		//setObjectPosition(_object_Size, _experiment_Object_Positions.at(0)[0], _experiment_Object_Positions.at(0)[1]);
		_trial_Counter = -1;
		updateExperimentObjects();

		_start_Object_Selected = false;
		_cursor_tf_nd->setNodeMask(0x0);
	}
	else if (_experiment_Session) {
		//_trialCounter_Text_tf_nd->setNodeMask(0x0);
		//_task_Text_tf_nd->setNodeMask(0x0);

		//_subsessions_Counter++;
		//if (_subsessions_Counter < _subsessions_Order.size()) {
		//	_scene_tf_nd->setNodeMask(0x0);
		//	_expt_tf_nd->setNodeMask(0xffffffff);

		//	_show_Practice_Info = true;

		//	//_desc_txt_nd->setText(_session_1_RT_PRACTICE);
		//	_desc_txt_nd->setText(_subsession_Practice_Texts.at(_subsessions_Order.at(_subsessions_Counter) - 1));
		//}
		//else if (_session_Counter < _sessions_Order.size()) {
		//	_session_Counter++;

		//	_show_Break_Info = true;

		//	_scene_tf_nd->setNodeMask(0x0);
		//	_expt_tf_nd->setNodeMask(0xffffffff);

		//	_desc_txt_nd->setText(_session_Break);// _session_1_Desc_Text);

		//	resetCounters();
		//}
		return;
	}
}

void FingerRange::updateKeyboardCursor(float x, float y) {
	_user_Cursor_X = x;
	_user_Cursor_Y = y;

	osg::Matrix osg_m;
	osg_m.setTrans(_user_Cursor_X, _user_Cursor_Y, 0.05f);

	_cursor_tf_nd->setMatrix(osg_m);
}

void FingerRange::checkTargetProximity(double x, double y) {
	//	std::cout << "DX: " << x << ", TX: " << _target_1_Min[0] << ", Max: " << _target_1_Max[0] << std::endl;
	//	std::cout << "DY: " << y << ", TY: " << _target_1_Min[1] << ", Max: " << _target_1_Max[1] << std::endl;

	if ((x > _target_1_Min[0] && x < _target_1_Max[0]) && (y > _target_1_Min[1] && y < _target_1_Max[1]) && !_on_Start_Object) {
		_on_Start_Object = true;

		std::cout << "OSO: " << _on_Start_Object << std::endl;

		_startObject_Selected_Time = clock();
	}
	else if (_on_Start_Object && !_start_Object_Selected) {
		std::cout << "ELSE" << std::endl;
		_on_Start_Object = false;
	}

	if ((x > _target_2_Min[0] && x < _target_2_Max[0]) && (y > _target_2_Min[1] && y < _target_2_Max[1]) && _start_Object_Selected) {
		_on_Target_Object = true;

		_targetObject_Selected_Time = clock();
	}
	else if (!_target_Object_Selected)
		_on_Target_Object = false;
}

void FingerRange::resetCounters() {
	_subsessions_Order.clear();
	_subsession_Practice_Texts.clear();
	_subsession_Expt_Texts.clear();
	_subsession_On_Screen_Text.clear();
	_subsession_Names.clear();

	std::cout << "SESSION COUNTER: " << _session_Counter << std::endl;
	for (int i = 0; i < _sessions_Order.size(); i++)
		std::cout << "at(i): " << _sessions_Order.at(i) << std::endl;

	if (_session_Counter < _sessions_Order.size()) {
		if (_sessions_Order.at(_session_Counter) == 1) {
			_screen_Number = 1;

			_subsessions_Order.push_back(1);
			_subsessions_Order.push_back(2);

			_subsession_Practice_Texts.push_back(_session_1_LT_PRACTICE);
			_subsession_Practice_Texts.push_back(_session_1_RT_PRACTICE);
			_subsession_Expt_Texts.push_back(_session_1_LT_EXPT);
			_subsession_Expt_Texts.push_back(_session_1_RT_EXPT);

			_subsession_On_Screen_Text.push_back(_session_1_LT_On_Screen_Text);
			_subsession_On_Screen_Text.push_back(_session_1_RT_On_Screen_Text);

			SetNormalizeFactors(1);

			_subsession_Names.push_back("LT");
			_subsession_Names.push_back("RT");
		}
		else if (_sessions_Order.at(_session_Counter) == 2) {
			_screen_Number = 2;

			_subsessions_Order.push_back(1);
			_subsessions_Order.push_back(2);
			_subsessions_Order.push_back(3);
			_subsessions_Order.push_back(4);

			_subsession_Practice_Texts.push_back(_session_2_LI_PRACTICE);
			_subsession_Practice_Texts.push_back(_session_2_LM_PRACTICE);
			_subsession_Practice_Texts.push_back(_session_2_RI_PRACTICE);
			_subsession_Practice_Texts.push_back(_session_2_RM_PRACTICE);

			_subsession_Expt_Texts.push_back(_session_2_LI_EXPT);
			_subsession_Expt_Texts.push_back(_session_2_LM_EXPT);
			_subsession_Expt_Texts.push_back(_session_2_RI_EXPT);
			_subsession_Expt_Texts.push_back(_session_2_RM_EXPT);

			_subsession_On_Screen_Text.push_back(_session_2_LI_On_Screen_Text);
			_subsession_On_Screen_Text.push_back(_session_2_LM_On_Screen_Text);
			_subsession_On_Screen_Text.push_back(_session_2_RI_On_Screen_Text);
			_subsession_On_Screen_Text.push_back(_session_2_RM_On_Screen_Text);

			_subsession_Names.push_back("LI");
			_subsession_Names.push_back("LM");
			_subsession_Names.push_back("RI");
			_subsession_Names.push_back("RM");

			SetNormalizeFactors(2);
		}
	}
	else {
		_scene_tf_nd->setNodeMask(0x0);
		_expt_tf_nd->setNodeMask(0xffffffff);

		_desc_txt_nd->setText(_expt_End);// _session_1_Desc_Text);
	}

	std::random_shuffle(_subsessions_Order.begin(), _subsessions_Order.end());

	_subsessions_Counter = 0;
	_trial_Counter = 0;
	_random_Trial = _trial_Order.at(_trial_Counter);
	int temp;// = _random_Trial % _experiment_Object_Positions.size();

	if (_practice_Session) {
		_object_Size = _practice_Object_Sizes.at(rand() % _practice_Object_Sizes.size());

		temp = rand() % _practice_Object_Positions.size();
		_target_1_Position = _practice_Object_Positions.at(temp)[0];
		_target_2_Position = _practice_Object_Positions.at(temp)[1];
	}
	else {
		_object_Size = _experiment_Object_Sizes.at(_random_Trial / _experiment_Object_Positions.size());

		temp = _random_Trial % _experiment_Object_Positions.size();

		_target_1_Position = _experiment_Object_Positions.at(temp)[0];
		_target_2_Position = _experiment_Object_Positions.at(temp)[1];
	}

	_practice_Session = false;

	_start_Session = false;
	_show_Session_Info = false;
}

void FingerRange::drawCoordinates() {
	axes_geode = new osg::Geode;
	osg::Vec3 sp(0.0f, 0.0f, 0.0f);
	osg::Vec3 ep(1.0f, 0.0f, 0.0f);

	osg::ref_ptr<osg::Geometry> axis = new osg::Geometry;

	axes_geode->addDrawable(axis);

	osg::ref_ptr<osg::Vec3Array> points = new osg::Vec3Array;
	points->push_back(osg::Vec3f(0.0f, 0.0f, 0.0f));
	points->push_back(osg::Vec3f(1.0f, 0.0f, 0.0f));
	points->push_back(osg::Vec3f(0.0f, 0.0f, 0.0f));
	points->push_back(osg::Vec3f(0.0f, 1.0f, 0.0f));
	points->push_back(osg::Vec3f(0.0f, 0.0f, 0.0f));
	points->push_back(osg::Vec3f(0.0f, 0.0f, 1.0f));

	axis->setVertexArray(points);

	osg::DrawElementsUInt* xaxis = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
	xaxis->push_back(0);
	xaxis->push_back(1);
	axis->addPrimitiveSet(xaxis);

	osg::DrawElementsUInt* yaxis = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
	yaxis->push_back(2);
	yaxis->push_back(3);
	axis->addPrimitiveSet(yaxis);

	osg::DrawElementsUInt* zaxis = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
	zaxis->push_back(4);
	zaxis->push_back(5);
	axis->addPrimitiveSet(zaxis);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));

	axis->setColorArray(colors);
	axis->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	axisXForm = new osg::PositionAttitudeTransform();

	_scene_tf_nd->addChild(axisXForm);
	axisXForm->addChild(axes_geode);

	osg::Vec3 axesPosition(-2.5, 0, -2.5);
	axisXForm->setPosition(axesPosition);
}

//| Function: getScene
osg::Group* FingerRange::getScene() {
	return _root_nd.get();
}

bool FingerRange::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	switch (ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::KEYDOWN):
	{
		switch (ea.getKey())
		{
		case ' ':
			std::cout << "Space bar pressed" << std::endl;
			//if (!_show_Expt_Info)

			if (_start_Session) {
				_expt_tf_nd->setNodeMask(0x0);
				_show_Expt_Info = false;
				_show_Device_Desc = true;
				_scene_tf_nd->setNodeMask(0xffffffff);
			}
			else if (_show_Expt_Info) {
				//					_session_Counter++;
				_show_Expt_Info = false;
				_show_Device_Desc = true;

				//_expt_tf_nd->removeChild(_desc_nd);
				_desc_nd->removeDrawable(_desc_txt_nd);

				_desc_txt_nd->setText(_device_Desc_Text);
				_desc_nd->addDrawable(_desc_txt_nd);

				//showExperimentInfo();
			}
			else if (_show_Device_Desc) {
				_show_Device_Desc = false;
				_show_Task_Info = true;

				osg::Matrixf osg_m;
				osg_m.makeTranslate(osg::Vec3(0.0f, 0.0f, 8.5f));
				_expt_tf_nd->setMatrix(osg_m);

				//_desc_txt_nd->setText(_task_Desc_Text);
				showInteractivePracticeTask();
			}
			else if (_show_Task_Info || _show_Break_Info) {
				osg::Matrixf osg_m;
				osg_m.makeTranslate(osg::Vec3(0.0f, 1.4f, 0.0f));
				_expt_tf_nd->setMatrix(osg_m);

				_desc_txt_nd->setAlignment(osgText::Text::CENTER_CENTER);

				std::cout << "Displaying SESSION INFO" << std::endl;
				_show_Task_Info = false;
				_show_Break_Info = false;

				_show_Session_Info = true;

				_desc_nd->removeDrawable(_desc_txt_nd);

				//desc_txt_nd->setText(_session_Texts.at(_sessions_Order.at(_session_Counter - 1) - 1));// _session_1_Desc_Text);
				_desc_txt_nd->setText(_session_Texts.at(_sessions_Order.at(_session_Counter) - 1));// _session_1_Desc_Text);
				_desc_nd->addDrawable(_desc_txt_nd);
			}
			else if (_show_Session_Info) {
				std::cout << "Displaying SUBSESSION PRACTICE INFO" << std::endl;
				_show_Session_Info = false;
				_show_Practice_Info = true;

				_desc_nd->removeDrawable(_desc_txt_nd);

				_interactionFinger = _subsession_Names.at(_subsessions_Order.at(_subsessions_Counter) - 1);
				std::cout << "INTERACTION FINGER: " << _interactionFinger << std::endl;

				//_desc_txt_nd->setText(_session_1_RT_PRACTICE);
				_desc_txt_nd->setText(_subsession_Practice_Texts.at(_subsessions_Order.at(_subsessions_Counter) - 1));
				_desc_nd->addDrawable(_desc_txt_nd);
			}
			else if (_show_Practice_Info) {
				_show_Practice_Info = false;
				_practice_Session = true;

				_expt_tf_nd->setNodeMask(0x0);

				_on_Screen_Text_nd->removeDrawable(_on_Screen_Text);

				_session_Text->setText("PRACTICE SESSION\n----------------");

				_on_Screen_Text->setText(_subsession_On_Screen_Text.at(_subsessions_Order.at(_subsessions_Counter) - 1));
				_on_Screen_Text->setFontResolution(256, 256);
				_on_Screen_Text_nd->addDrawable(_on_Screen_Text);

				//				_scene_tf_nd->removeChild(_practice_Instruction_tf_nd);

				if (_practice_Instructions_Display_Count < 3) {
					if (_sessions_Order.at(_session_Counter) == 1)
						_practice_Instruction_Text->setText(_start_Object_Instruction1 + " TOP TOUCH SURFACE " + _start_Object_Instruction2);
					if (_sessions_Order.at(_session_Counter) == 2)
						_practice_Instruction_Text->setText(_start_Object_Instruction1 + " BOTTOM TOUCH SURFACE " + _start_Object_Instruction2);

					_practice_Instruction_Text->setColor(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
					//					_scene_tf_nd->addChild(_practice_Instruction_tf_nd);
				}

				_scene_tf_nd->setNodeMask(0xffffffff);

				// updatePracticeObjects();
			}
			else if (_practice_Session) {
				_practice_Session = false;
				std::cout << "Displaying Experiment Info" << std::endl;
				_show_ExptSession_Info = true;

				_expt_tf_nd->setNodeMask(0xffffffff);
				_scene_tf_nd->setNodeMask(0x0);

				_desc_nd->removeDrawable(_desc_txt_nd);

				//_desc_txt_nd->setText(_session_1_RT_EXPT);
				_desc_txt_nd->setText(_subsession_Expt_Texts.at(_subsessions_Order.at(_subsessions_Counter) - 1));
				_desc_nd->addDrawable(_desc_txt_nd);

				_practice_Instruction_tf_nd->setNodeMask(0x0);

				_trial_Counter = 0;
			}
			else if (_show_ExptSession_Info) {
				std::cout << "Starting Experiment" << std::endl;
				_show_ExptSession_Info = false;

				_experiment_Session = true;

				_session_Text->setText("EXPERIMENT SESSION\n------------------");

				_expt_tf_nd->setNodeMask(0x0);
				_scene_tf_nd->setNodeMask(0xffffffff);
				_trialCounter_Text_tf_nd->setNodeMask(0xffffffff);
				_task_Text_tf_nd->setNodeMask(0xffffffff);

				//setObjectPosition(_object_Size, _experiment_Object_Positions.at(0)[0], _experiment_Object_Positions.at(0)[1]);
				_trial_Counter = -1;
				updateExperimentObjects();

				_start_Object_Selected = false;
			}
			else if (_experiment_Session) {
				_trialCounter_Text_tf_nd->setNodeMask(0x0);
				_task_Text_tf_nd->setNodeMask(0x0);

				_subsessions_Counter++;
				std::cout << "TEST: " << _subsessions_Counter << std::endl;
				if (_subsessions_Counter < _subsessions_Order.size()) {
					_scene_tf_nd->setNodeMask(0x0);
					_expt_tf_nd->setNodeMask(0xffffffff);

					_show_Practice_Info = true;

					_desc_nd->removeDrawable(_desc_txt_nd);

					_interactionFinger = _subsession_Names.at(_subsessions_Order.at(_subsessions_Counter) - 1);
					std::cout << "INTERACTION FINGER: " << _interactionFinger << std::endl;

					//_desc_txt_nd->setText(_session_1_RT_PRACTICE);
					_desc_txt_nd->setText(_subsession_Practice_Texts.at(_subsessions_Order.at(_subsessions_Counter) - 1));
					_desc_nd->addDrawable(_desc_txt_nd);
				}
				else if (_session_Counter < _sessions_Order.size()) {
					_session_Counter++;

					_show_Break_Info = true;

					_scene_tf_nd->setNodeMask(0x0);
					_expt_tf_nd->setNodeMask(0xffffffff);

					_desc_nd->removeDrawable(_desc_txt_nd);

					_desc_txt_nd->setText(_session_Break);// _session_1_Desc_Text);
					_desc_nd->addDrawable(_desc_txt_nd);

					resetCounters();
				}

			}
			return false;
			break;
		case 'z':
			_show_Expt_Info = false;
			_practice_Session = true;

			_expt_tf_nd->setNodeMask(0x0);
			_scene_tf_nd->setNodeMask(0xffffffff);
			break;
		case '-':
			if (_scene_tf_nd->getNodeMask() == 0xffffffff) {
				if (_practice_Session)
					updatePracticeObjects();
				else if (_experiment_Session)
					updateExperimentObjects();
			}
			break;
		case 'a':
			_object_group_nd->setNodeMask(0x0);
			break;
		case 's':
			_object_group_nd->setNodeMask(0xffffffff);
			break;
		case osgGA::GUIEventAdapter::KEY_Left:
			keyboardX -= 0.05;
			keyboardY = _target_1_Position[1];
			keyboardZ = 0.05f;
			updateKeyboardCursor(keyboardX, keyboardY);
			break;
		case osgGA::GUIEventAdapter::KEY_Right:
			keyboardX += 0.05;
			keyboardY = _target_1_Position[1];
			keyboardZ = 0.05f;
			updateKeyboardCursor(keyboardX, keyboardY);
		case osgGA::GUIEventAdapter::KEY_Down:
			keyboardY -= 0.05f;
			updateKeyboardCursor(keyboardX, keyboardY);
			break;
		case osgGA::GUIEventAdapter::KEY_Up:
			keyboardY += 0.05f;
			updateKeyboardCursor(keyboardX, keyboardY);
			break;
		case 'e':
			_trial_Counter = 50;
			_subsessions_Counter = 5;
			_session_Counter = 4;

			_scene_tf_nd->setNodeMask(0x0);
			_expt_tf_nd->setNodeMask(0xffffffff);

			_desc_nd->removeDrawable(_desc_txt_nd);

			_desc_txt_nd->setText(_expt_End);// _session_1_Desc_Text);
			_desc_nd->addDrawable(_desc_txt_nd);

			break;
		case 'g':
			showScreenErrorAlert(1);
			//_object_group_nd->setNodeMask(0x0);
			//{
			//	osg::ref_ptr<osg::MatrixTransform> alertinfo_tf_nd = new osg::MatrixTransform;

			//	osg::ref_ptr<osg::Geode> alert_Text_Quad_Geode = new osg::Geode;
			//	osg::ref_ptr<osg::ShapeDrawable> alert_Quad_Shape = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 0.0f, -0.15f), 8.875f, 1.325f, 0.0f));
			//	alert_Quad_Shape->setColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 0.1f));

			//	alert_Text_Quad_Geode->addDrawable(alert_Quad_Shape.get());
			//	alertinfo_tf_nd->addChild(alert_Text_Quad_Geode.get());

			//	osg::ref_ptr<osg::Geode> alert_Text_nd = new osg::Geode();
			//	osg::ref_ptr<osgText::Text> alert_Text = new osgText::Text();

			//	alert_Text->setText("TEST");

			//	alert_Text->setColor(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
			//	alert_Text->setFont("Resources/Fonts/arialbd.ttf");
			//	alert_Text->setCharacterSize(.25f);
			//	alert_Text->setAlignment(osgText::Text::CENTER_CENTER);
			//	alert_Text->setAxisAlignment(osgText::TextBase::SCREEN);

			//	alert_Text_Quad_Geode->addDrawable(alert_Text.get());

			//	osg::Matrixf osg_m;

			//	osg_m.makeTranslate(osg::Vec3(0.0f, 0.0f, 0.0f));
			//	alertinfo_tf_nd->setMatrix(osg_m);
			//	alertinfo_tf_nd->addChild(alert_Text_nd);

			//	_scene_tf_nd->addChild(alertinfo_tf_nd);

			//	_thisViewer->renderingTraversals();

			//	delayTimer(2.0);

			//	alertinfo_tf_nd->setNodeMask(0x0);

			//	_object_group_nd->setNodeMask(0xffffffff);
			//	_thisViewer->renderingTraversals();
			//}
			break;

		default:
			return false;
		}
	}
	default:
		return false;
	}
}

void FingerRange::setViewer(osgViewer::Viewer* viewer) {
	_thisViewer = viewer;
}

void FingerRange::delayTimer(float seconds) {
	clock_t startTime = clock(); //Start timer

	while (true)
		if ((clock() - startTime) / CLOCKS_PER_SEC >= seconds)
			break;
}

void FingerRange::update() {
	if (!_start_Object_Selected) {
		if ((_user_Cursor_X > _target_1_Min[0] && _user_Cursor_X < _target_1_Max[0]) && (_user_Cursor_Y > _target_1_Min[1] && _user_Cursor_Y < _target_1_Max[1])) {
			if (!_on_Start_Object) {
				_on_Start_Object = true;

				_object_Outlines[0]->setColor(osg::Vec4(0.9529f, 0.9529f, 0.0823f, 1.0f));
				_object_Outlines[0]->setNodeMask(0xfffffff);

				_startObject_Selected_Time = clock();

				if (_interactive_Task_Session == true) {
					_practice_Instruction_Text->setText("Wait until this highlight turns RED\nand you will hear a confirmation sound.");
					//delayTimer(2.0f);
				}
			}
			else if ((clock() - _startObject_Selected_Time) / CLOCKS_PER_SEC > _on_Object_Time) {
				_start_Object_Selected = true;

				_start_ObjectAcquiredTime = (clock() - _first_TouchTime) / CLOCKS_PER_SEC;

				PlaySound("Resources/Sounds/chime_up.wav", NULL, SND_SYNC);

				taskResult.startObjectSelectionTime = _start_ObjectAcquiredTime;

				taskResult.startObjectMisses = _number_StartObject_Misses;

				_object_Outlines[0]->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0));

				if (_practice_Session || _interactive_Task_Session) {
					if (_interactive_Task_Session) {
						_scene_tf_nd->removeChild(_scene_tf_nd->getNumChildren() - 1);
						_practice_Instruction_Text->setText("Now, move your finger on the \nTOP TOUCH SURFACE\n" + _target_Object_Instruction2);

						osg::Matrixf osg_m;
						osg_m.makeTranslate(osg::Vec3(_target_2_Position[0] + 3.0f, _target_2_Position[1] - 3.0f, 0.0f));
						_practice_Instruction_tf_nd->setMatrix(osg_m);
						//showInteractivePracticeTask();

						osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
						osg::Vec4Array* colors = new osg::Vec4Array;

						osg::ref_ptr<osg::Geode> geode = new osg::Geode;
						osg::ref_ptr<osg::Geometry> outlineGeom = new osg::Geometry;

						osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth();
						linewidth->setWidth(5.0f);

						v->push_back(osg::Vec3(_target_2_Position[0] + 2.0f, _target_2_Position[1] - 2.0f, 0.0f));
						v->push_back(osg::Vec3(_target_2_Position[0] + 2.3f, _target_2_Position[1] - 1.7f, 0.0f));
						v->push_back(osg::Vec3(_target_2_Position[0] + 1.4f, _target_2_Position[1] - 0.8f, 0.0f));
						v->push_back(osg::Vec3(_target_2_Position[0] + 1.7f, _target_2_Position[1] - 0.5f, 0.0f));
						v->push_back(osg::Vec3(_target_2_Position[0] + 0.8f, _target_2_Position[1] - 0.5f, 0.0f));
						v->push_back(osg::Vec3(_target_2_Position[0] + 0.8f, _target_2_Position[1] - 1.4f, 0.0f));
						v->push_back(osg::Vec3(_target_2_Position[0] + 1.1f, _target_2_Position[1] - 1.1f, 0.0f));
						//v->push_back(osg::Vec3(_target_1_Position[0] - 2.0f, _target_1_Position[1] + 2.0f, 0.0f));

						outlineGeom->setVertexArray(v.get());

						colors->push_back(osg::Vec4(0.2f, 1.0f, 0.6f, 1.0f));
						outlineGeom->setColorArray(colors);
						outlineGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

						outlineGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, v->size()));
						outlineGeom->getOrCreateStateSet()->setAttributeAndModes(linewidth, osg::StateAttribute::ON);
						outlineGeom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

						geode->addDrawable(outlineGeom.get());

						_scene_tf_nd->addChild(geode.get());
					}

					if (_practice_Session && _practice_Instructions_Display_Count < 3) {
						if (_sessions_Order.at(_session_Counter) == 1)
							_practice_Instruction_Text->setText(_target_Object_Instruction1 + "\nTOP TOUCH SURFACE\n" + _target_Object_Instruction2);
						if (_sessions_Order.at(_session_Counter) == 2)
							_practice_Instruction_Text->setText(_target_Object_Instruction1 + "\nBOTTOM TOUCH SURFACE\n" + _target_Object_Instruction2);

						_practice_Instruction_Text->setColor(osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));

						osg::Matrixf osg_m;
						osg_m.makeTranslate(osg::Vec3(_target_2_Position[0], _target_2_Position[1] - 2.0f, 0.0f));
						_practice_Instruction_tf_nd->setMatrix(osg_m);
					}
				}
			}
		}
		else {
			if (_on_Start_Object) {
				_on_Start_Object = false;
				_number_StartObject_Misses++;
				_object_Outlines[0]->setNodeMask(0x0);
			}
		}

		_task_StartTime = clock();
	}

	if (_start_Object_Selected && !_target_Object_Selected) {
		if ((_user_Cursor_X > _target_2_Min[0] && _user_Cursor_X < _target_2_Max[0]) && (_user_Cursor_Y > _target_2_Min[1] && _user_Cursor_Y < _target_2_Max[1])) {
			if (!_on_Target_Object) {
				_on_Target_Object = true;

				_object_Outlines[1]->setColor(osg::Vec4(0.9529f, 0.9529f, 0.0823f, 1.0f));
				_object_Outlines[1]->setNodeMask(0xfffffff);

				_targetObject_Selected_Time = clock();

				if (_interactive_Task_Session == true) {
					_practice_Instruction_Text->setText("Wait until this highlight turns RED\nand you will hear a different confirmation sound");
				}
			}
			else if ((clock() - _targetObject_Selected_Time) / CLOCKS_PER_SEC > _on_Object_Time) {
				_task_EndTime = clock();

				_task_TotalTime = (_task_EndTime - _task_StartTime) / CLOCKS_PER_SEC;

				_target_Object_Selected = true;

				_object_Outlines[1]->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0));

				PlaySound("Resources/Sounds/chime_down.wav", NULL, SND_SYNC);

				taskResult.taskCompletionTime = _task_TotalTime;
				taskResult.targetObjectMisses = _number_TargetObject_Misses;
				taskResult.totalMisses = _number_StartObject_Misses + _number_TargetObject_Misses;

				//delayTimer(1.0);
				if (_practice_Session) {
					_practice_Instructions_Display_Count++;
				}
			}
		}
		else {
			if (_on_Target_Object) {
				_on_Target_Object = false;

				_number_TargetObject_Misses++;
				_object_Outlines[1]->setNodeMask(0x0);
			}
		}
	}

	if (_start_Object_Selected && _target_Object_Selected) {
		_on_Start_Object = false;
		_on_Target_Object = false;
		_start_Object_Selected = false;
		_target_Object_Selected = false;

		_object_Outlines[0]->setNodeMask(0x0);
		_object_Outlines[1]->setNodeMask(0x0);

		if (_interactive_Task_Session) {
			_on_Object_Time = 0.5f;

			_interactive_Task_Session = false;
			_scene_tf_nd->removeChild(_scene_tf_nd->getNumChildren() - 1);

			_desc_txt_nd->setText("You will repeat this task on different surfaces using different fingers.\nAsk the experimenter if you have any questions.");

			/*_practice_Instruction_Text->setText("You will repeat this task on different surfaces using different fingers.\nAsk the experimenter if you have any questions.");
			_practice_Instruction_Text->setCharacterSize(0.55f);
			_practice_Instruction_Text->setColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));*/

			osg::Matrixf osg_m;
			//osg_m.makeTranslate(osg::Vec3(0.0f, 0.0f, -8.5f));
			//_practice_Instruction_tf_nd->setMatrix(osg_m);

			_thisViewer->renderingTraversals();

			delayTimer(4.0);
			_scene_tf_nd->setNodeMask(0x0);

			osg_m.makeTranslate(osg::Vec3(0.0f, 1.4f, 0.0f));
			_expt_tf_nd->setMatrix(osg_m);

			_desc_txt_nd->setAlignment(osgText::Text::CENTER_CENTER);

			std::cout << "Displaying SESSION INFO" << std::endl;
			_show_Task_Info = false;
			_show_Break_Info = false;

			_show_Session_Info = true;

			_desc_txt_nd->setText(_session_Texts.at(_sessions_Order.at(_session_Counter) - 1));// _session_1_Desc_Text);
																							   //			delayTimer(4.0f);
		}

		if (_practice_Session) {
			_practice_Trial_Counter++;

			if (_practice_Trial_Counter < NUMBER_PRACTICE_TRIALS)
				updatePracticeObjects();
			else {
				_practice_Session = false;
				std::cout << "Displaying Experiment Info" << std::endl;
				_show_ExptSession_Info = true;

				_expt_tf_nd->setNodeMask(0xffffffff);
				_scene_tf_nd->setNodeMask(0x0);

				//_desc_txt_nd->setText(_session_1_RT_EXPT);
				_desc_txt_nd->setText(_subsession_Expt_Texts.at(_subsessions_Order.at(_subsessions_Counter) - 1));

				_trial_Counter = 0;
			}
		}
		if (_experiment_Session) {
			std::cout << "ES" << std::endl;

			if (_resultsFileStream.is_open())
				_resultsFileStream << _trial_Counter + 1 << "\t" << taskResult.startObjectSize << "\t" << taskResult.targetObjectSize << "\t" << taskResult.distance << "\t" << taskResult.indexofDifficulty << "\t" << taskResult.taskCompletionTime << "\t" << taskResult.totalMisses << "\t" << taskResult.firstTouchDistance << "\t" << taskResult.startObjectSelectionTime << "\t" << taskResult.startObjectMisses << "\t" << taskResult.targetObjectMisses << std::endl;
			else
				std::cout << "CANNOT WRITE RESULTS TO FILE" << std::endl;

			updateExperimentObjects();
		}
	}
}

int FingerRange::getNumberofOutlines() {
	return _object_Outlines.size();
}

void FingerRange::showScreenErrorAlert(int screenNo) {
	_object_group_nd->setNodeMask(0x0);
	{
		osg::ref_ptr<osg::MatrixTransform> alertinfo_tf_nd = new osg::MatrixTransform;

		osg::ref_ptr<osg::Geode> alert_Text_Quad_Geode = new osg::Geode;
		osg::ref_ptr<osg::ShapeDrawable> alert_Quad_Shape = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 0.0f, -0.15f), 8.875f, 1.325f, 0.0f));
		alert_Quad_Shape->setColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 0.1f));

		alert_Text_Quad_Geode->addDrawable(alert_Quad_Shape.get());
		alertinfo_tf_nd->addChild(alert_Text_Quad_Geode.get());

		osg::ref_ptr<osg::Geode> alert_Text_nd = new osg::Geode();
		osg::ref_ptr<osgText::Text> alert_Text = new osgText::Text();

		if (_screen_Number == 1)
			alert_Text->setText("INCORRECT INTERACTION SCREEN ALERT\n----------------------------------\nPlease use TOP screen for interaction");
		else if (_screen_Number == 2)
			alert_Text->setText("INCORRECT INTERACTION SCREEN ALERT\n----------------------------------\nPlease use BOTTOM screen for interaction");

		alert_Text->setColor(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
		alert_Text->setFont("Resources/Fonts/arialbd.ttf");
		alert_Text->setCharacterSize(0.35f);
		alert_Text->setAlignment(osgText::Text::CENTER_CENTER);
		alert_Text->setAxisAlignment(osgText::TextBase::SCREEN);

		alert_Text_Quad_Geode->addDrawable(alert_Text.get());

		osg::Matrixf osg_m;

		osg_m.makeTranslate(osg::Vec3(0.0f, 0.0f, 0.0f));
		alertinfo_tf_nd->setMatrix(osg_m);
		alertinfo_tf_nd->addChild(alert_Text_nd);

		_scene_tf_nd->addChild(alertinfo_tf_nd);

		delayTimer(2.0);

		alertinfo_tf_nd->setNodeMask(0x0);

		_object_group_nd->setNodeMask(0xffffffff);
	}
}