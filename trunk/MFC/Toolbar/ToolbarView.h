// ToolbarView.h : CToolbarView ��Ľӿ�
//


#pragma once


class CToolbarView : public CView
{
protected: // �������л�����
	CToolbarView();
	DECLARE_DYNCREATE(CToolbarView)

// ����
public:
	CToolbarDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CToolbarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ToolbarView.cpp �еĵ��԰汾
inline CToolbarDoc* CToolbarView::GetDocument() const
   { return reinterpret_cast<CToolbarDoc*>(m_pDocument); }
#endif

