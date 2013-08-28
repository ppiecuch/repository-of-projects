#pragma once
#include "afxdlgs.h"
#include "attributespage.h"
#include "materialspage.h"



// COperateView 窗体视图

class COperateView : public CFormView
{
	DECLARE_DYNCREATE(COperateView)

protected:
	COperateView();           // 动态创建所使用的受保护的构造函数
	virtual ~COperateView();

public:
	enum { IDD = IDD_DIALOG_OPERATE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
protected:
	CPropertySheet m_sheet;
	CAttributesPage m_attributesPage;
	CMaterialsPage m_materialsPage;
};


