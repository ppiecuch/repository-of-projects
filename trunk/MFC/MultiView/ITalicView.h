#pragma once

#include "MultiViewDoc.h"


// CITalicView ��ͼ

class CITalicView : public CEditView
{
	DECLARE_DYNCREATE(CITalicView)

protected:
	CITalicView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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


