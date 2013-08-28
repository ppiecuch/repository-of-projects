// IrrlichtSplitView.h : CIrrlichtSplitView 类的接口
//


#pragma once


class CIrrlichtSplitView : public CFormView
{
protected: // 仅从序列化创建
	CIrrlichtSplitView();
	DECLARE_DYNCREATE(CIrrlichtSplitView)

public:
	enum{ IDD = IDD_IRRLICHTSPLIT_FORM };

// 属性
public:
	CIrrlichtSplitDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CIrrlichtSplitView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // IrrlichtSplitView.cpp 中的调试版本
inline CIrrlichtSplitDoc* CIrrlichtSplitView::GetDocument() const
   { return reinterpret_cast<CIrrlichtSplitDoc*>(m_pDocument); }
#endif

