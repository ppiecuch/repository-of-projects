// GameEditor.h : main header file for the GAMEEDITOR application
//

#if !defined(AFX_GAMEEDITOR_H__32065444_ACB8_4482_864E_C0F901E11513__INCLUDED_)
#define AFX_GAMEEDITOR_H__32065444_ACB8_4482_864E_C0F901E11513__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGameEditorApp:
// See GameEditor.cpp for the implementation of this class
//

class CGameEditorApp : public CWinApp
{
public:
	CGameEditorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameEditorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGameEditorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEEDITOR_H__32065444_ACB8_4482_864E_C0F901E11513__INCLUDED_)
