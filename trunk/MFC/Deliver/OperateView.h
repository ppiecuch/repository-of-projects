#pragma once
#include "MainFrm.h"
#include "Context.h"
#include "afxcmn.h"
#include "TabDialog.h"



// COperateView ������ͼ

class COperateView : public CFormView
{
	DECLARE_DYNCREATE(COperateView)

protected:
	COperateView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
protected:
	afx_msg LRESULT OnDownload(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_str;
	afx_msg void OnEnChangeEdit1();
	CTabCtrl m_tab;
	CTabDialog m_dialog1;
	virtual void OnInitialUpdate();
};


