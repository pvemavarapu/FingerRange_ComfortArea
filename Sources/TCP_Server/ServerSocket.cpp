#include "stdafx.h"
#include "TCP_Server/ServerSocket.h"

DWORD __stdcall ClientThread(void* p);

SocketListener* listener;
SOCKET ListenSocket, ClientSocket;

bool just_Connected = true;

FingerRange *_application, *_Tempapp;

bool _rotate;
bool _translate;
bool _pan;
bool _scale;
bool _register;

static int _registeredScreens = 0;

std::vector<FingerRange::TouchScreenRes> deviceRes;

ServerSocket::ServerSocket(FingerRange &app) {
	ListenSocket = INVALID_SOCKET;
	recvbuflen = DEFAULT_BUFLEN;
	listening = true;
	result = NULL;
	sender = NULL;
	listener = NULL;

	_application = &app;

	_translate = true;
	_pan = false;
	_rotate = false;

	_register = false;
}

ServerSocket::~ServerSocket() {
}

SocketSender* ServerSocket::getSender() {
	return sender;
};

SocketListener* ServerSocket::getListener() {
	return listener;
};

HANDLE ServerSocket::getThread() {
	return this->m_hThread;
}

void ServerSocket::HandleMessage(char* xml) {
	std::string str = std::string(xml);

	//if (this->sender == NULL){
	//if (sender == NULL){
	//	std::cout << "NULL SENDER, RETURNING NULL" << std::endl;
	//	return;
	//}

//	std::cout << "MESSAGE: " << xml << std::endl;

	if (xml && !xml[0]) {
		std::cout << "Empty input from the device" << std::endl;
		return;
	}

	if (!_register){
		if (str.substr(0, 10).compare("<REGISTER>") == 0){
			int pos = str.rfind(",");

			std::cout << "Registering Screen Number: " << std::atoi(str.substr(10, 1).c_str()) << "Resolution - " << std::atoi(str.substr(12, pos - 12).c_str()) << " X " << std::atoi(str.substr(pos + 1).c_str()) << std::endl;

			_application->registerTouchScreens(std::atoi(str.substr(10, 1).c_str()), std::atoi(str.substr(12, pos - 12).c_str()), std::atoi(str.substr(pos + 1).c_str()));

			std::cout << "Registered Screen Number: " << std::atoi(str.substr(10, 1).c_str()) << std::endl;

			//if (sender != NULL){
				sender->sendJustConnected();
			//}

			deviceRes = _application->getDeviceResolution();

			_registeredScreens++;

			if(_registeredScreens == 2)
				_register = true;
		}
	}

	if (str.find("DTAP") != std::string::npos) {
		_application->DoubleTapUpdate();
	}
	else if (str[0] == 'C'){
		int pos = str.rfind(",");

		//int a = boost::lexical_cast<int>(str[1]);
		int s = str[1] - 48;
		int x = std::atoi(str.substr(3, pos - 1).c_str());
		int y = std::atoi(str.substr(pos + 1).c_str());

//		if (_translate){
			//std::cout << "TRANSLATE" << std::endl;
			_application->updateUserCursorPosition(s, x, y);
//		}
		//else if (_rotate){
		//	std::cout << "ROTATE" << std::endl;
		//	//_application->rotationInput(a, x, y, 0.0f);
		//}
		//else if (_pan){
		//	//std::cout << "PAN" << std::endl;
		//	//			_application->panInput(a, x, y, 0.0f);
		//}
	}
	else if (str.compare("ROTATE") == 0){
		_rotate = true;
		_pan = false;
		_translate = false;
	}
	else if (str.compare("PAN") == 0){
		_rotate = false;
		_pan = true;
		_translate = false;
	}
	else if ((str.compare("RESETROTATE") == 0) || (str.compare("RESETPAN") == 0)){
		_rotate = false;
		_pan = false;
		_translate = true;
	}
	//else if (str.compare("DTAP") == 0) {
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

DWORD WINAPI ServerSocket::StaticThreadStart(void* p){
	ServerSocket* ss = (ServerSocket*)p;
	return ss->ClientThread(p);
}

DWORD __stdcall ServerSocket::ClientThread(void* p){
	static int client_Number = 1;

	std::cout << "Created and running thread for client number: " << client_Number << std::endl;
	client_Number++;

	int *csock = (int*)p;

	while (true){
		char recvBuffer[1024] = { 0 };
		char clientBuffer[1024];
		int buffer_len = 1024;
		size_t bytecount;

//		if ((bytecount = recv(*csock, recvBuffer, buffer_len, 0)) == SOCKET_ERROR){
		if ((bytecount = recv(*csock, recvBuffer, sizeof(recvBuffer) - 1, 0)) == SOCKET_ERROR){
			std::cout << "Error receiving data -" << WSAGetLastError() << std::endl;;
			goto FINISH;
		}

//		std::cout << "Received bytes - " << bytecount << "\nReceived string -- " <<  recvBuffer << std::endl;

		//recvBuffer[bytecount - 1] = 0; // null terminate the string according to the length

		if (strstri(recvBuffer, "QUIT") != NULL) {
			std::cout << "Client connection closing with QUIT from thread" << std::endl;

			closesocket(*csock);
			if (this->isRunning()){
				this->Exit();
			}
			break;
		}

		/*
		clientBuffer[0] = '\0';
		strcat(clientBuffer, " <Response><Name>RandomNumberResponse</Name><Number>57</Number></Response>\n");
		if ((bytecount = send(*csock, clientBuffer, strlen(clientBuffer), 0)) == SOCKET_ERROR){
		fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
		goto FINISH;
		}
		printf("Sent bytes %d\n", bytecount);*/

		/*if (sender == NULL)
			std::cout << "NULL SENDER" << std::endl;*/

		HandleMessage(recvBuffer);

		if ((bytecount = send(*csock, clientBuffer, strlen(clientBuffer), 0)) == SOCKET_ERROR){
			std::cout << "Error sending data -" << WSAGetLastError() << std::endl;;
			goto FINISH;
		}
	}

FINISH:
	std::cout << "finish ERROR" << std::endl;
	free(csock);
	return 0;
}

DWORD ServerSocket::m_ThreadFunc() {
	std::cout << "Starting Server..." << std::endl;
	std::cout << " SERVER INFO:\n-------------";

	std::cout << "Initialize the winsock library ... \n";

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed" << std::endl;
		return 1;
	}
	std::cout << "WSAStartup Success!!\n";

	ZeroMemory(&hints, sizeof(hints));
	
	// TCP SOCKET
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//UDP SOCKET
	/*hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;*/

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT_STR, &hints, &result);
	if (iResult != 0) {
		std::cout << "getaddrinfo failed" << std::endl;
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		std::cout << "socket failed" << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	std::cout << "Socket Creation Success!!\n";

	//// Set the mode of the socket to be nonblocking
	//u_long iMode = 1;

	// Setup the TCP listening socket
	iResult = ::bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		std::cout << "Socket Bind Failed -- " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	std::cout << "Socket Bind Success!!\n";

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		std::cout << "listen failed" << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	int* csock;
	sockaddr_in client_addr;
	int clen;
	int addr_size = sizeof(SOCKADDR);

	std::cout << "Listing for clients on port " << DEFAULT_PORT_STR << std::endl;

	while (1) {
		std::cout << "TRUE" << std::endl;

		clen = sizeof(client_addr);
		csock = (int*)malloc(sizeof(int));

		if ((*csock = accept(ListenSocket, (SOCKADDR*)&client_addr, &addr_size)) != INVALID_SOCKET){
			std::cout << "Received connection from " << inet_ntoa(client_addr.sin_addr) << " , port " << client_addr.sin_port << std::endl;

			struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&client_addr;
			struct in_addr ipAddr = pV4Addr->sin_addr;

			clientPort = pV4Addr->sin_port;

			std::cout << "CREATING SENDER..." << std::endl;

			sender = new SocketSender(csock);

			//if (sender == NULL)
			//	std::cout << "NULL SENDER CREATED" << std::endl;
			//else
				CreateThread(0, 0, StaticThreadStart, (void*)csock, 0, 0);
		}
		else{
			std::cout << "Error accepting " << WSAGetLastError() << std::endl;
		}
	}

	return 0;
}

void ServerSocket::SetInteractionScreen(int number){
	screen_Number = number;
	std::cout << "SS: " << screen_Number << std::endl;
}