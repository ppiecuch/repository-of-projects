// DialogBarView.h : CDialogBarView ��Ľӿ�
//


#pragma once


class CDialogBarView : public CView
{
protected: // �������л�����
	CDialogBarView();
	DECLARE_DYNCREATE(CDialogBarView)

// ����
public:
	CDialogBarDoc* GetDocument() const;

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
	virtual ~CDialogBarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DialogBarView.cpp �еĵ��԰汾
inline CDialogBarDoc* CDialogBarView::GetDocument() const
   { return reinterpret_cast<CDialogBarDoc*>(m_pDocument); }
#endif

