// IrrlichtSplitDoc.h : CIrrlichtSplitDoc ��Ľӿ�
//


#pragma once


class CIrrlichtSplitDoc : public CDocument
{
protected: // �������л�����
	CIrrlichtSplitDoc();
	DECLARE_DYNCREATE(CIrrlichtSplitDoc)

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CIrrlichtSplitDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	bool m_bUpdated;

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
// ����
public:
	bool HasUpdated(void);
};


