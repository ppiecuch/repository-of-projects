// BarDemoDoc.h : CBarDemoDoc ��Ľӿ�
//


#pragma once


class CBarDemoDoc : public CDocument
{
protected: // �������л�����
	CBarDemoDoc();
	DECLARE_DYNCREATE(CBarDemoDoc)

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
	virtual ~CBarDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


