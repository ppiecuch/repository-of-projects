// DialogBarDoc.h : CDialogBarDoc ��Ľӿ�
//


#pragma once


class CDialogBarDoc : public CDocument
{
protected: // �������л�����
	CDialogBarDoc();
	DECLARE_DYNCREATE(CDialogBarDoc)

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
	virtual ~CDialogBarDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


