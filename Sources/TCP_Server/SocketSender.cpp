#include "stdafx.h"
#include "TCP_Server/SocketSender.h"

static char* _primaryScreenIP = NULL;

SOCKET cSock = INVALID_SOCKET;
int *clientS;

SocketSender::SocketSender(SOCKET client) {
	std::cout << "SENDER!!" << std::endl;
	this->clientSocket = client;

	cSock = client;

	std::cout << "SENDER CREATED!!" << std::endl;
}

SocketSender::SocketSender(int *clientSoc){
	clientS = clientSoc;
	std::cout << "SENDER CREATED!!" << std::endl;
}

std::string SocketSender::GetPrimaryScreenIP(){
	return _primaryScreenIP;
}

void SocketSender::SetPrimaryScreenIP(std::string str){
	char buffer[512];
	buffer[0] = 0;

	std::cout << "SETTING PSIP" << str << std::endl;
	_primaryScreenIP = (char*)malloc(strlen(str.c_str()));
	strcpy(_primaryScreenIP, str.c_str());

	sprintf_s(buffer, "<Response><TYPE>SHOWPRIMARYIP</Name><VALUE>%s</VALUE></Response>\n", _primaryScreenIP);

	sendXMLToClient(buffer);
}

void SocketSender::SendPrimaryScreenIP(){
	std::string pIP = std::string(_primaryScreenIP);

	char buffer[512];
	buffer[0] = 0;
	sprintf_s(
		buffer,
		"<Response><TYPE>PRIMARYSCREENIP</TYPE><VALUE>%s</VALUE></Response>\n", _primaryScreenIP);

	sendXMLToClient(buffer);
}

void SocketSender::sendACK(){
	char ack_Response[] = "ACK";

	char buffer[512];
	buffer[0] = 0;
	sprintf_s(
		buffer,
		"<Response><ACK>ACK</ACK></Response>\n", ack_Response);

	sendXMLToClient(buffer);
}

void SocketSender::sendJustConnected(){
	char resp[] = "";

	char buffer[512];
	buffer[0] = '\0';
	strcat(buffer, "<P_ACK>\n");

	sendXMLToClient(buffer);
}

void SocketSender::sendXMLToClient(char xml[]) {
	int sent;
	// Send some XML to the client
	int len = strlen(xml);

	std::cout << "SENDING TO CLIENT..." << xml << std::endl;

	sent = send(*clientS, xml, len, 0);
	if (sent == SOCKET_ERROR) {
		printf("\nsend failed\n");
		fprintf(stderr, "send() failed: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		return;
	}

	std::cout << sent << " bytes sent to client" << std::endl;
}
