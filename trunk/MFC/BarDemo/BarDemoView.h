// BarDemoView.h : CBarDemoView ��Ľӿ�
//


#pragma once


class CBarDemoView : public CFormView
{
protected: // �������л�����
	CBarDemoView();
	DECLARE_DYNCREATE(CBarDemoView)

public:
	enum{ IDD = IDD_BARDEMO_FORM };

// ����
public:
	CBarDemoDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CBarDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // BarDemoView.cpp �еĵ��԰汾
inline CBarDemoDoc* CBarDemoView::GetDocument() const
   { return reinterpret_cast<CBarDemoDoc*>(m_pDocument); }
#endif

