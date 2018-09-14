#ifndef CTHREAD_H
#define CTHREAD_H

#include "stdafx.h"

typedef unsigned (WINAPI* PCTHREAD_THREADFUNC)(void* threadParam);

class CThread {
public:
	CThread();
	void startThread();
	static DWORD WINAPI ThreadFunc(LPVOID param);
	void waitForExit();
	void Exit();
	bool isRunning();

protected:
	virtual DWORD m_ThreadFunc();
	HANDLE m_hThread;
	unsigned int m_threadId;
	bool m_fRunning;
};
#endif
