#if !defined(SOCKET_SENDER__INCLUDED_)
#define SOCKET_SENDER__INCLUDED_

#include <iostream>
#include <string>

#include <WinSock2.h>

#define DEFAULT_BUFLEN 512

class SocketSender {
protected:
	WSADATA wsaData;
	SOCKET clientSocket;
	struct addrinfo *result, hints;
	char recvbuf[DEFAULT_BUFLEN];
	int iResult, iSendResult;
	int recvbuflen;

public:
	SocketSender(SOCKET client);
	SocketSender(int *clientSoc);

	void sendHostnameResponse();
	void sendMemoryResponse();
	void sendRandomNumberResponse();

	void SetPrimaryScreenIP(std::string str);
	std::string GetPrimaryScreenIP();
	void SendPrimaryScreenIP();

	void sendACK();
	void sendJustConnected();

protected:
	void sendXMLToClient(char* xml);
};

#endif