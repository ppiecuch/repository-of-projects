// FloatTest.h : main header file for the FLOATTEST application
//

#if !defined(AFX_FLOATTEST_H__74F00C26_A0D6_11D2_898F_8050F6598430__INCLUDED_)
#define AFX_FLOATTEST_H__74F00C26_A0D6_11D2_898F_8050F6598430__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFloatTestApp:
// See FloatTest.cpp for the implementation of this class
//

class CFloatTestApp : public CWinApp
{
public:
	CFloatTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFloatTestApp)
	public:
	virtual BOOL InitInstance();
	
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFloatTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOATTEST_H__74F00C26_A0D6_11D2_898F_8050F6598430__INCLUDED_)
