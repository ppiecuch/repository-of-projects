#pragma once
#include "MainFrm.h"
#include "Context.h"



// CRenderView ������ͼ

class CRenderView : public CFormView
{
	DECLARE_DYNCREATE(CRenderView)

protected:
	CRenderView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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


