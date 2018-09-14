#include <stdlib.h>
#include <iostream>

#include "Server_Main.h"

//// Prototypes ////////////////////////////////////////////////////////
extern int DoWinsock(const char* pcHost, int nPort);

//// Constants /////////////////////////////////////////////////////////

// Default port to connect to on the server
const int kDefaultServerPort = 8080;

//// main //////////////////////////////////////////////////////////////

//void ServerMain::StartServer(){
DWORD ServerMain::m_ThreadFunc() {
	// Get host and (optionally) port from the command line
	const char* pcHost = "192.168.1.101";
	int nPort = 8080;// kDefaultServerPort;

	// Start Winsock up
	WSAData wsaData;
	int nCode;
	if ((nCode = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		std::cerr << "WSAStartup() returned error code " << nCode << "." << std::endl;
		return 255;
	}

	// Call the main example routine.
	int retval = DoWinsock(pcHost, nPort);

	// Shut Winsock back down and take off.
	WSACleanup();
	return 0;
}