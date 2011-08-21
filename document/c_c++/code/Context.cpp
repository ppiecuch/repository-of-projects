#include "StdAfx.h"
#include "Context.h"

Context::Context(void): str(_T(""))
{
}

Context& Context::GetInstance()  
{  
	static Context m_instance;
	return m_instance;
}  