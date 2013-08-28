#pragma once
#include "MainFrm.h"
#include "Context.h"



// CRenderView 窗体视图

class CRenderView : public CFormView
{
	DECLARE_DYNCREATE(CRenderView)

protected:
	CRenderView();           // 动态创建所使用的受保护的构造函数
	virtual ~CRenderView();

public:
	enum { IDD = IDD_DIALOG_RENDER };
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
	afx_msg void OnEnChangeEdit1();
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	CString m_str;
protected:
	afx_msg LRESULT OnUpload(WPARAM wParam, LPARAM lParam);
};


