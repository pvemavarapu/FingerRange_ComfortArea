#ifndef SERVER_MAIN_H
#define SERVER_MAIN_H

#include "CThread.h"

class ServerMain{
	ServerMain();
	~ServerMain();

public:
	void StartServer();
	
	HANDLE getThread();
	DWORD m_ThreadFunc();
};

#endif