// IrrlichtSplitView.h : CIrrlichtSplitView ��Ľӿ�
//


#pragma once


class CIrrlichtSplitView : public CFormView
{
protected: // �������л�����
	CIrrlichtSplitView();
	DECLARE_DYNCREATE(CIrrlichtSplitView)

public:
	enum{ IDD = IDD_IRRLICHTSPLIT_FORM };

// ����
public:
	CIrrlichtSplitDoc* GetDocument() const;

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
	virtual ~CIrrlichtSplitView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // IrrlichtSplitView.cpp �еĵ��԰汾
inline CIrrlichtSplitDoc* CIrrlichtSplitView::GetDocument() const
   { return reinterpret_cast<CIrrlichtSplitDoc*>(m_pDocument); }
#endif

