#include "TCP_Server/stdafx.h"
#include "TCP_Server/SocketListener.h"

#include <boost/algorithm/string.hpp>

#include "Application.h"

bool just_Connected = false;

Application *_app;

SocketListener::SocketListener(SOCKET socket) {
	recvbuflen = DEFAULT_BUFLEN;
	this->ClientSocket = socket;

	_app = new Application();
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

DWORD SocketListener::m_ThreadFunc() {
	listening = true;

	while ( listening ) {
		if (just_Connected){
			strcpy(recvbuf, "SERVER READY");
			just_Connected = false;

			HandleMessage(recvbuf);
		}

		int iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			//printf("Bytes received: %d\n", iResult);
			printf("Message from Client: %s\n", recvbuf);
			system("PAUSE");
			recvbuf[iResult-1] = 0; // null terminate the string according to the length

			// The message spec indicates the XML will end in a "new line"
			// character which can be either a newline or caraiage feed
			// Search for either and replace with a NULL to terminate
			if ( recvbuf[iResult-2] == '\n' || recvbuf[iResult-2] == '\r' )
				recvbuf[iResult-2] = 0;

			if (strstri(recvbuf, "QUIT") != NULL) {
				printf("Client connection closing with QUIT from thread\n");
				// closesocket(ClientSocket);
				if (this->isRunning()){
					this->Exit();
				}
				break;
			}

			if (just_Connected){
				strcpy(recvbuf, "SERVER READY");
				just_Connected = false;
			}

			HandleMessage(recvbuf);
		}
		else {
			std::cout << "ERROR\n";
			system("PAUSE");
			/*printf("Client connection closing from Thread\n");
			closesocket(ClientSocket);
			return 1;*/
		}
	}

	return 0;
}

void SocketListener::setSender(SocketSender* sender) {
	this->sender = sender;
	just_Connected = true;
}

void SocketListener::HandleMessage(char* xml) {
	std::cout << "XML: " << xml << std::endl;
	std::cout << "XML[0]: " << xml[0] << std::endl;
	std::cout << "XML[1]: " << xml[1] << std::endl;

	std::cout << "LEN: " << sizeof(xml) << std::endl;

	if ( this->sender == NULL )
		return;

	if (xml && ! xml[0]) {
		printf("c is empty\n");
		return;
	}

	if (strstri(xml, "SERVER READY")){
		if (sender != NULL)
			sender->sendJustConnected();
	}

	if(xml[0] != '<'){
		if(xml[0] == 'T'){
			if(xml[1] = 'X'){
				float x[2];

				char temp[7];
				x[0] = atof(std::strncpy(temp, xml + 2, 7));
				x[1] = atof(std::strncpy(temp, xml + 10, 7));

				std::cout << "X[0]: " << x[0] << ", X[1]: " << x[1] << std::endl;

				_app->transInput(1, x[0], x[1], 0);
			}
			if (xml[1] == 'Z'){
				float x[2];
				
				char temp[7];
				x[0] = atof(std::strncpy(temp, xml + 2, 7));
				x[1] = atof(std::strncpy(temp, xml + 10, strlen(xml)));

				std::cout << "X[0]: " << x[0] << ", X[1]: " << x[1] << std::endl;
				
				_app->transInput(2, x[0], x[1], 0);
			}
		}
		if(xml[0] == 'R'){
			if(xml[1] = 'X'){
				float x[2];

				char temp[7];
				x[0] = atof(std::strncpy(temp, xml + 2, 7));
				x[1] = atof(std::strncpy(temp, xml + 10, 7));

				std::cout << "X[0]: " << x[0] << ", X[1]: " << x[1] << std::endl;

				_app->rotationInput(1, x[0], x[1], 0);
			}
			if (xml[1] == 'Z'){
				float x[2];
				
				char temp[7];
				x[0] = atof(std::strncpy(temp, xml + 2, 7));
				x[1] = atof(std::strncpy(temp, xml + 10, strlen(xml)));

				std::cout << "X[0]: " << x[0] << ", X[1]: " << x[1] << std::endl;
				
				_app->rotationInput(3, x[0], x[1], 0);
			}
		}
		if(xml[0] == 'P'){
			if(xml[1] = 'X'){
				float x[2];

				char temp[7];
				x[0] = atof(std::strncpy(temp, xml + 2, 7));
				x[1] = atof(std::strncpy(temp, xml + 10, 7));

				std::cout << "X[0]: " << x[0] << ", X[1]: " << x[1] << std::endl;

				_app->panInput(1, x[0], 0, 0);
				_app->panInput(2, 0, 0, x[1]);

			}
			if (xml[1] == 'Z'){
				float x[2];
				
				char temp[7];
				x[0] = atof(std::strncpy(temp, xml + 2, 7));
				x[1] = atof(std::strncpy(temp, xml + 10, strlen(xml)));

				std::cout << "X[0]: " << x[0] << ", X[1]: " << x[1] << std::endl;
				
				_app->panInput(3, 0, 0, x[1]);
			}
		}
		if(xml[0] == 'S'){
			if(xml[3] == 'P'){
				_app->scaleInput("Positive");
			}
			else if(xml[3] == 'N'){
				_app->scaleInput("Negative");
			}
		}
	}
	
	else{
	if (strcmp(xml, "SETPRIMARYSCREENIP")){
		printf("Setting Primary Screen IP..");

		std::string incomingMessage = std::string(xml);
		unsigned int first = incomingMessage.find("<VALUE>") + std::strlen("<VALUE>");
		unsigned int last = incomingMessage.find("<\\VALUE>");

		std::string primaryScreenIP = incomingMessage.substr(first, last - first);

		sender->SetPrimaryScreenIP(primaryScreenIP);
	}

	if (strcmp(xml, "GETPRIMARYSCREENIP")){
		if (sender != NULL)
			sender->SendPrimaryScreenIP();
	}
	}
}
