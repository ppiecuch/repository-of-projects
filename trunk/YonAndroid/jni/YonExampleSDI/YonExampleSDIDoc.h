// YonExampleSDIDoc.h : CYonExampleSDIDoc ��Ľӿ�
//


#pragma once


class CYonExampleSDIDoc : public CDocument
{
protected: // �������л�����
	CYonExampleSDIDoc();
	DECLARE_DYNCREATE(CYonExampleSDIDoc)

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
	virtual ~CYonExampleSDIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


