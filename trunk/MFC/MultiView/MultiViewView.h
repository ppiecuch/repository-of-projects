// MultiViewView.h : CMultiViewView ��Ľӿ�
//


#pragma once


class CMultiViewView : public CEditView
{
protected: // �������л�����
	CMultiViewView();
	DECLARE_DYNCREATE(CMultiViewView)

// ����
public:
	CMultiViewDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMultiViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MultiViewView.cpp �еĵ��԰汾
inline CMultiViewDoc* CMultiViewView::GetDocument() const
   { return reinterpret_cast<CMultiViewDoc*>(m_pDocument); }
#endif

