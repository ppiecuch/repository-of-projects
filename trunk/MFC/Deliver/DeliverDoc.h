// DeliverDoc.h : CDeliverDoc ��Ľӿ�
//


#pragma once


class CDeliverDoc : public CDocument
{
protected: // �������л�����
	CDeliverDoc();
	DECLARE_DYNCREATE(CDeliverDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CDeliverDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


