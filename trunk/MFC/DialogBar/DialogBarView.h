// DialogBarView.h : CDialogBarView 类的接口
//


#pragma once


class CDialogBarView : public CView
{
protected: // 仅从序列化创建
	CDialogBarView();
	DECLARE_DYNCREATE(CDialogBarView)

// 属性
public:
	CDialogBarDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CDialogBarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DialogBarView.cpp 中的调试版本
inline CDialogBarDoc* CDialogBarView::GetDocument() const
   { return reinterpret_cast<CDialogBarDoc*>(m_pDocument); }
#endif

