// MultiViewDoc.h : CMultiViewDoc ��Ľӿ�
//


#pragma once


class CMultiViewDoc : public CDocument
{
protected: // �������л�����
	CMultiViewDoc();
	DECLARE_DYNCREATE(CMultiViewDoc)

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
	virtual ~CMultiViewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_strText;
};


