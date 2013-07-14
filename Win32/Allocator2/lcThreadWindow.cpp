#include "lcThreadWindow.h"


namespace lc{

	DWORD TheadWindow::threadProc(LPVOID data){
		TheadWindow* thread = static_cast<TheadWindow*>(data);
		thread->m_pRunnable->run();
		thread->m_eState = STOPPED;
		return 0;
	}

	TheadWindow::TheadWindow(IRunnable* r):m_eState(NONE),m_handle(NULL),m_threadId(-1),m_pRunnable(r){
		//r->grab();
		DWORD dwCreationFlags = CREATE_SUSPENDED;
		m_handle = ::CreateThread(NULL, 0, threadProc, this, dwCreationFlags, &m_threadId);
		if(m_handle)
			m_eState = READY;
	}

	TheadWindow::~TheadWindow(){
		if(m_pRunnable)
		{
			//m_pRunnable->drop();
			delete m_pRunnable;
			m_pRunnable=NULL;
		}
	}


	void TheadWindow::start(){
		if(::ResumeThread(m_handle)!=-1)
			m_eState=RUNNING;
	}

	thread::State TheadWindow::getState(){
		return m_eState;
	}
}