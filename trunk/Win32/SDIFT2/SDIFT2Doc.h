// SDIFT2Doc.h : CSDIFT2Doc ��Ľӿ�
//


#pragma once


class CSDIFT2Doc : public CDocument
{
protected: // �������л�����
	CSDIFT2Doc();
	DECLARE_DYNCREATE(CSDIFT2Doc)

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
	virtual ~CSDIFT2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


