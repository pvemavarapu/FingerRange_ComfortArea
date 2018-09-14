#include "TCP_Server/CThread.h"
#include "TCP_Server/SocketListener.h"
#include "TCP_Server/SocketSender.h"

#include "FingerRange.h"

//#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT_STR "8080"

#define MAX_CONNECTIONS 20

class ServerSocket : public CThread {
public:
	WSADATA wsaData;
	SOCKET ListenSocket, ClientSocket;
	struct addrinfo *result, hints;
	char recvbuf[DEFAULT_BUFLEN];
	int iResult, iSendResult;
	int recvbuflen;
	bool listening;

	USHORT clientPort;

	void HandleMessage(char* xml);

	ServerSocket(FingerRange &app);
	~ServerSocket();
	SocketSender* getSender();
	SocketListener* getListener();
	HANDLE getThread();

	DWORD m_ThreadFunc();
	DWORD __stdcall ClientThread(void* p);

	static DWORD WINAPI StaticThreadStart(void *p);

	void SetInteractionScreen(int number);

protected:
	SocketSender* sender;

	int screen_Number;
};

