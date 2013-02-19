// SDIFT2View.h : CSDIFT2View ��Ľӿ�
//


#pragma once


class CSDIFT2View : public CView
{
protected: // �������л�����
	CSDIFT2View();
	DECLARE_DYNCREATE(CSDIFT2View)

// ����
public:
	CSDIFT2Doc* GetDocument() const;

	FT_Library	m_pFTLib;
	FT_Face		m_pFTFace;
	int			m_iZoom;


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
	virtual ~CSDIFT2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // SDIFT2View.cpp �еĵ��԰汾
inline CSDIFT2Doc* CSDIFT2View::GetDocument() const
   { return reinterpret_cast<CSDIFT2Doc*>(m_pDocument); }
#endif

