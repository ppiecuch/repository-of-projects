#pragma once

#include "MultiViewDoc.h"


// CITalicView 视图

class CITalicView : public CEditView
{
	DECLARE_DYNCREATE(CITalicView)

protected:
	CITalicView();           // 动态创建所使用的受保护的构造函数
	virtual ~CITalicView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* /*pDC*/);
};


