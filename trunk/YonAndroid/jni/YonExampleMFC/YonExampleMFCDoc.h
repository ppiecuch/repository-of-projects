// YonExampleMFCDoc.h : CYonExampleMFCDoc ��Ľӿ�
//


#pragma once


class CYonExampleMFCDoc : public CDocument
{
protected: // �������л�����
	CYonExampleMFCDoc();
	DECLARE_DYNCREATE(CYonExampleMFCDoc)

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
	virtual ~CYonExampleMFCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


