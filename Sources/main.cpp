//|___________________________________________________________________
//!
//! \file main.cpp
//!
//! \brief Main program.
//!
//! Author: Prabhakar Vemavarapu.
//|___________________________________________________________________

//|___________________
//|
//| Includes
//|___________________

#include <stdio.h>
#include <iostream>

#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>

#include "TCP_Server/stdafx.h"
#include "TCP_Server/ServerSocket.h"

#include <conio.h>
#include <iphlpapi.h>

#include "FingerRange.h"

//#define OSG_USE_IO_DOT_H

//|___________________
//|
//| Global Variables
//|___________________

FingerRange *application;                 // A vrJuggler application instance
ServerSocket *server;

void update(){
	application->update();
}
//|____________________________________________________________________
//|
//| Function: main
//|
//! \param arc       [in] Argument count.
//! \param argv      [in] Array of arguments.
//! \return Exit code.
//!
//! Program's entry point.
//|____________________________________________________________________

int main(int argc, char* argv[]){
	application = new FingerRange();                // Instantiates application instance      

	application->initScene();

	IP_ADAPTER_INFO  *pAdapterInfo;
	ULONG            ulOutBufLen;
	DWORD            dwRetVal;

	pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
	ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS) {
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) != ERROR_SUCCESS) {
		printf("GetAdaptersInfo call failed with %d\n", dwRetVal);
	}

	PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
	while (pAdapter) {
		std::string tempstr = pAdapter->Description;

		if (pAdapter->Type == 71 && tempstr.find("Virtual") == std::string::npos) {
			//printf("Adapter Name: %s\n", pAdapter->AdapterName);
			//printf("Adapter Desc: %s\n", pAdapter->Description);
			//printf("\tAdapter Addr: \t");
			//for (UINT i = 0; i < pAdapter->AddressLength; i++) {
			//	if (i == (pAdapter->AddressLength - 1))
			//		printf("%.2X\n", (int)pAdapter->Address[i]);
			//	else
			//		printf("%.2X-", (int)pAdapter->Address[i]);
			//}
			printf("IP Address: %s\n", pAdapter->IpAddressList.IpAddress.String);
			//printf("IP Mask: %s\n", pAdapter->IpAddressList.IpMask.String);
			//printf("\tGateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);
			//printf("\t***\n");
			//if (pAdapter->DhcpEnabled) {
			//	printf("\tDHCP Enabled: Yes\n");
			//	printf("\t\tDHCP Server: \t%s\n", pAdapter->DhcpServer.IpAddress.String);
			//}
			//else
			//	printf("\tDHCP Enabled: No\n");
		}
		pAdapter = pAdapter->Next;
	}
	if (pAdapterInfo)
		free(pAdapterInfo);

	printf("Native server starting\n");

	// Start server socket listener
	server = new ServerSocket(*application);
	server->startThread();

	// initialize the viewer.
	osgViewer::Viewer viewer;
	std::cout << "Setting SceneData...\n";
	osg::ref_ptr<osg::Group> this_Root_nd = new osg::Group;

	this_Root_nd = application->getScene();

	// set up the use of stereo by default.
	//osg::DisplaySettings::instance()->setStereo(true);

	//osg::DisplaySettings::instance()->setStereoMode(osg::DisplaySettings::StereoMode::ANAGLYPHIC);// getStereoMode(

	if (this_Root_nd != NULL){
		viewer.setSceneData(this_Root_nd.get());
	}
	else{
		std::cout << "NULL" << std::endl;
	}

	viewer.setUpViewOnSingleScreen(0);

	viewer.realize();

	osg::Vec3d eye(0.0, -1.0, 0.0);
	osg::Vec3d center(0.0, 0.0, 0.0);
	osg::Vec3d up(0.0, 0.0, 1.0);

	//viewer.getCamera()->setProjectionMatrixAsOrtho(-8.0f, 8.0f, -4.5f, 4.5f, -17.0f, 2.0f);
	//viewer.getCamera()->setProjectionMatrixAsOrtho(-16.0f, 16.0f, -9.0f, 9.0f, -17.0f, 2.0f);
	viewer.getCamera()->setProjectionMatrixAsOrtho(-20.0f, 20.0f, -11.25f, 11.25f, -17.0f, 2.0f);
	//viewer.getCamera()->setProjectionMatrixAsOrtho(-36.0f, 36.0f, -20.25f, 20.25f, -17.0f, 2.0f);
	//viewer.getCamera()->setReferenceFrame(osg::Transform::RELATIVE_RF);

	viewer.getCamera()->setClearColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));

	double left, right, top, bottom, n, f;
	viewer.getCamera()->getProjectionMatrixAsOrtho(left, right, bottom, top, n, f);

	application->setSceneBounds(left, right, top, bottom, n, f);

	viewer.getCamera()->setViewMatrix(osg::Matrix::identity());
	viewer.getCamera()->setViewMatrixAsLookAt(eye, center, up);

	application->setViewMatrix(viewer.getCamera()->getViewMatrix());
	application->setProjectionMatrix(viewer.getCamera()->getProjectionMatrix());

	viewer.addEventHandler(application);

	osgViewer::Viewer::Windows thisWindow;

	viewer.getWindows(thisWindow);
	thisWindow[0]->setCursor(osgViewer::GraphicsWindow::NoCursor);

	application->setViewer(&viewer);
	
	while (!viewer.done()){
		application->update();
		viewer.frame();
	}

//	delete application;
//	std::system("PAUSE");

	return 0;
}
