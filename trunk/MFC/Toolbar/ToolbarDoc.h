// ToolbarDoc.h : CToolbarDoc ��Ľӿ�
//


#pragma once


class CToolbarDoc : public CDocument
{
protected: // �������л�����
	CToolbarDoc();
	DECLARE_DYNCREATE(CToolbarDoc)

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
	virtual ~CToolbarDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


