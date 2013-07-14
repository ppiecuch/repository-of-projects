#ifndef _LUCID_THREADWINDOW_H_
#define _LUCID_THREADWINDOW_H_

#include "lcThread.h"
#include <windows.h>

namespace lc{


class TheadWindow : public thread{
	HANDLE m_handle;
	DWORD m_threadId;
	IRunnable* m_pRunnable;
	thread::State m_eState;
	static DWORD WINAPI threadProc(LPVOID data);
public:
	TheadWindow(IRunnable* r);
	~TheadWindow();
	virtual void start();
	virtual thread::State getState();
};

}
#endif