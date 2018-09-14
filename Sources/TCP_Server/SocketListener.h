#ifndef SOCKET_LISTENER_H
#define SOCKET_LISTENER_H

#include <iostream>
#include "TCP_Server/CThread.h"
#include "TCP_Server/SocketSender.h"

#define BUFLEN 2048

class SocketListener : public CThread {
protected:
    WSADATA wsaData;
    SOCKET ClientSocket;
    struct addrinfo *result, hints;
    char recvbuf[BUFLEN];
    int iResult, iSendResult;
    int recvbuflen;
	bool listening;
	SocketSender* sender;

public:
	SocketListener(SOCKET socket);
	void setSender(SocketSender* sender);
	DWORD m_ThreadFunc();

protected:
	void HandleMessage(char* xml);
};

#endif
