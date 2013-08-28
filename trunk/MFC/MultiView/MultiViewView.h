// MultiViewView.h : CMultiViewView 类的接口
//


#pragma once


class CMultiViewView : public CEditView
{
protected: // 仅从序列化创建
	CMultiViewView();
	DECLARE_DYNCREATE(CMultiViewView)

// 属性
public:
	CMultiViewDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMultiViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MultiViewView.cpp 中的调试版本
inline CMultiViewDoc* CMultiViewView::GetDocument() const
   { return reinterpret_cast<CMultiViewDoc*>(m_pDocument); }
#endif

