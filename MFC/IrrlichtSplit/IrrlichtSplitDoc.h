// IrrlichtSplitDoc.h : CIrrlichtSplitDoc 类的接口
//


#pragma once


class CIrrlichtSplitDoc : public CDocument
{
protected: // 仅从序列化创建
	CIrrlichtSplitDoc();
	DECLARE_DYNCREATE(CIrrlichtSplitDoc)

// 属性
public:

	enum RENDER_MODE{
		POINT=1,
		LINE,
		PLANE
	};


// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CIrrlichtSplitDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	int m_nRenderMode;
	bool m_bUpdated;
};


