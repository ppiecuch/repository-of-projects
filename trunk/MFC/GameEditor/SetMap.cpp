// SetMap.cpp : implementation file
//

#include "stdafx.h"
#include "GameEditor.h"
#include "SetMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetMap dialog


CSetMap::CSetMap(CWnd* pParent /*=NULL*/)
	: CDialog(CSetMap::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetMap)
	m_width = 0;
	m_height = 0;
	//}}AFX_DATA_INIT
}


void CSetMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetMap)
	DDX_Text(pDX, IDC_EDIT1, m_width);
	DDV_MinMaxInt(pDX, m_width, 0, 300);
	DDX_Text(pDX, IDC_EDIT2, m_height);
	DDV_MinMaxInt(pDX, m_height, 0, 150);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetMap, CDialog)
	//{{AFX_MSG_MAP(CSetMap)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetMap message handlers

void CSetMap::OnButton1() 
{
    UpdateData(TRUE);	
	CDialog::OnOK();
}


void CSetMap::OnButton2() 
{

	CDialog::OnCancel();
	
}
