#include <WinSock2.h>
#include <ws2tcpip.h>
#include <IPHlpApi.h>

// For multi-threading
#include <Windows.h>
#include <process.h>

#include <iostream>
#include <string>

#include <boost/algorithm/string.hpp>

#include "ws-util.h"
#include "Application.h"

#pragma comment(lib,"ws2_32.lib")

const int buffer_Size = 1024;

bool just_Connected = true;
SOCKET thisSocket = NULL;

Application *_appl;

//// Prototypes ////////////////////////////////////////////////////////

SOCKET SetUpListener(const char* pcAddress, int nPort);
void AcceptConnections(SOCKET ListeningSocket);
bool HandleIncomingRequests(SOCKET sd);

//// DoWinsock /////////////////////////////////////////////////////////
// The module's driver function -- we just call other functions and
// interpret their results.
int DoWinsock(const char* pcAddress, int nPort){
	std::cout << "Establishing the listener..." << std::endl;

	SOCKET ListeningSocket = SetUpListener(pcAddress, htons(nPort));
	if (ListeningSocket == INVALID_SOCKET) {
		std::cout << std::endl << WSAGetLastErrorMessage("establish listener") << std::endl;
		return 3;
	}

	std::cout << "Waiting for connections..." << std::flush;

	while (1) {
		AcceptConnections(ListeningSocket);
		std::cout << "Acceptor restarting..." << std::endl;
	}
#if defined(_MSC_VER)
	return 0;   // warning eater
#endif
}

//// SetUpListener /////////////////////////////////////////////////////
// Sets up a listener on the given interface and port, returning the
// listening socket if successful; if not, returns INVALID_SOCKET.
SOCKET SetUpListener(const char* pcAddress, int nPort){
	u_long nInterfaceAddr = inet_addr(pcAddress);

	if (nInterfaceAddr != INADDR_NONE) {
		SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);

		if (sd != INVALID_SOCKET) {
			sockaddr_in sinInterface;
			sinInterface.sin_family = AF_INET;
			sinInterface.sin_addr.s_addr = nInterfaceAddr;
			sinInterface.sin_port = nPort;

			if (::bind(sd, (sockaddr*)&sinInterface, sizeof(sockaddr_in) ) != SOCKET_ERROR) {
				listen(sd, SOMAXCONN);
				return sd;
			}
			else {
				std::cerr << WSAGetLastErrorMessage("bind() failed") << std::endl;
			}
		}
	}

	return INVALID_SOCKET;
}

//// RequestHandler ///////////////////////////////////////////////////////
// Handles the incoming data by reflecting it back to the sender.
DWORD WINAPI RequestHandler(void* sd_){
	int nRetval = 0;
	SOCKET sd = (SOCKET)sd_;

	if (!HandleIncomingRequests(sd)) {
		std::cerr << std::endl << WSAGetLastErrorMessage("Incoming Error. Failed!!!") << std::endl;
		nRetval = 3;
	}

	std::cout << "Shutting connection down..." << std::flush;
	if (ShutdownConnection(sd)) {
		std::cout << "Connection is down." << std::endl;
	}
	else {
		std::cerr << std::endl << WSAGetLastErrorMessage("Connection shutdown failed") << std::endl;
		nRetval = 3;
	}

	return nRetval;
}

//// AcceptConnections /////////////////////////////////////////////////
// Spins forever waiting for connections.  For each one that comes in, 
// we create a thread to handle it and go back to waiting for
// connections.  If an error occurs, we return.
void AcceptConnections(SOCKET ListeningSocket){
	sockaddr_in sinRemote;
	int nAddrSize = sizeof(sinRemote);

	while (1) {
		SOCKET sd = accept(ListeningSocket, (sockaddr*)&sinRemote, &nAddrSize);

		if (sd != INVALID_SOCKET) {
			std::cout << "Accepted connection from " <<
				inet_ntoa(sinRemote.sin_addr) << ":" <<
				ntohs(sinRemote.sin_port) << "." <<
				std::endl;

			DWORD nThreadID;

			CreateThread(0, 0, RequestHandler, (void*)sd, 0, &nThreadID);
		}
		else {
			std::cerr << WSAGetLastErrorMessage("accept() failed") <<
				std::endl;
			return;
		}
	}
}

char* strstri(char* t, char* s) {
	int i, j;

	for (i = 0; t[i] != '\0'; i++) {
		for (j = 0; s[j] != '\0'; j++) {
			if (toupper(s[j]) == toupper(t[i + j]))
				continue;
			else
				break;
		}

		// if the whole string was successfully compared, exit the loop
		if (s[j] == '\0')
			break;
	}

	if (s[j] == '\0') {
		// returns a pointer to the first occurrence 
		// of s within t, just like the original strstr
		return (i + t);

	}

	// Not found
	return 0;
}

void sendXMLToClient(char xml[]) {
	// Send some XML to the client
	int len = strlen(xml);

	printf("\nXML TO CLIENT: %s\n", xml);

	int sent = send(thisSocket, xml, len, 0);

	if (sent == SOCKET_ERROR) {
		printf("\nsend failed\n");
		fprintf(stderr, "send() failed: %d\n", WSAGetLastError());
		closesocket(thisSocket);
		return;
	}

	printf("%i bytes sent to client\n", sent);
}

void sendJustConnected(){
	char resp[] = "JUST_CONNECTED";

	char buffer[512];
	buffer[0] = 0;
	sprintf_s(
		buffer,
		"<Response><ACK>JUST_CONNECTED</ACK></Response>\n", resp);

	sendXMLToClient(buffer);
}

void sendACK(){
	char ack_Response[] = "ACK";

	char buffer[512];
	buffer[0] = 0;
	sprintf_s(
		buffer,
		"<Response><ACK>ACK</ACK></Response>\n", ack_Response);

	sendXMLToClient(buffer);
}

void HandleMessage(char* xml) {
	if (strstri(xml, "SERVER READY")){
		sendJustConnected();
	}

	if (strstri(xml, "TRANS_XY")){
		std::string incomingMessage = std::string(xml);
		unsigned int first = incomingMessage.find("<COORDS>") + std::strlen("<COORDS>");
		unsigned int last = incomingMessage.find("<\\COORDS>");

		std::string coords = incomingMessage.substr(first, last - first);

		float x[2];

		std::istringstream iss(coords);
		std::string token;

		int i = 0;
		while (std::getline(iss, token, ',')){
			boost::trim(token);
			x[i] = atof(token.c_str());
			i++;
		}

		_appl->transInput(1, x[0], x[1], 0);

		//sendACK();
	}

	if (strstri(xml, "TRANS_Z")){
		/*std::string incomingMessage = std::string(xml);
		unsigned int first = incomingMessage.find("<COORDS>") + std::strlen("<COORDS>");
		unsigned int last = incomingMessage.find("<\\COORDS>");

		std::string coords = incomingMessage.substr(first, last - first);

		float x[2];

		std::string myText("some-text-to-tokenize");
		std::istringstream iss(coords);
		std::string token;

		int i = 0;
		while (std::getline(iss, token, ',')){
			boost::trim(token);
			x[i] = atof(token.c_str());
			i++;
		}

		_app->transInput(2, x[0], x[1], 0);*/
		sendACK();
	}

	if (strstri(xml, "SETPRIMARYSCREENIP")){
		printf("Setting Primary Screen IP..");

		std::string incomingMessage = std::string(xml);
		unsigned int first = incomingMessage.find("<VALUE>") + std::strlen("<VALUE>");
		unsigned int last = incomingMessage.find("<\\VALUE>");

		std::string primaryScreenIP = incomingMessage.substr(first, last - first);

		char buffer[512];
		buffer[0] = 0;
		sprintf_s(
			buffer,
			"<Response><TYPE>PRIMARYSCREENIP</TYPE><VALUE>%s</VALUE></Response>\n", primaryScreenIP);

		sendXMLToClient(buffer);
			//SetPrimaryScreenIP(primaryScreenIP);

	}

	//if (strstri(xml, "GETPRIMARYSCREENIP")){
	//	if (sender != NULL)
	//		SendPrimaryScreenIP();
	//}
}

//// HandleIncomingRequests ///////////////////////////////////////////////
// Bounces any incoming packets back to the client.  We return false
// on errors, or true if the client closed the socket normally.
bool HandleIncomingRequests(SOCKET sd){
	// Read data from client
	char acReadBuffer[buffer_Size];
	int nReadBytes;

	thisSocket = sd;

	do {
		if (just_Connected){
			printf("JUST CONNECTED!!!!\n");
			strcpy(acReadBuffer, "SERVER READY");
			just_Connected = false;

			HandleMessage(acReadBuffer);
		}

		nReadBytes = recv(sd, acReadBuffer, buffer_Size, 0);
		if (nReadBytes > 0) {
			std::cout << "Message Received: " << acReadBuffer << "\n\t\tSIZE: " << nReadBytes << std::endl;

			int nSentBytes = 0;

			while (nSentBytes < nReadBytes) {
				int nTemp = send(sd, acReadBuffer + nSentBytes, nReadBytes - nSentBytes, 0);

				if (nTemp > 0) {
					std::cout << "Sent " << nTemp << " bytes back to client." << std::endl;
					nSentBytes += nTemp;
				}
				else if (nTemp == SOCKET_ERROR) {
					return false;
				}
				else {
					// Client closed connection before we could reply to
					// all the data it sent, so bomb out early.
					std::cout << "Peer unexpectedly dropped connection!" << std::endl;
					return true;
				}
			}
		}
		else if (nReadBytes == SOCKET_ERROR) {
			return false;
		}
	} while (nReadBytes != 0);

	std::cout << "Connection closed by peer." << std::endl;
	return true;
}