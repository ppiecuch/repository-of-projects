#pragma once
#include "IrrlichtSplitDoc.h"
class CIrrlichtSplitDoc;




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
	void onRenderMode(UINT nID);
	virtual void OnInitialUpdate();
};


