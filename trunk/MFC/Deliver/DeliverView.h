// DeliverView.h : CDeliverView ��Ľӿ�
//


#pragma once


class CDeliverView : public CFormView
{
protected: // �������л�����
	CDeliverView();
	DECLARE_DYNCREATE(CDeliverView)

public:
	enum{ IDD = IDD_DELIVER_FORM };

// ����
public:
	CDeliverDoc* GetDocument() const;

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
	virtual ~CDeliverView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DeliverView.cpp �еĵ��԰汾
inline CDeliverDoc* CDeliverView::GetDocument() const
   { return reinterpret_cast<CDeliverDoc*>(m_pDocument); }
#endif

