// BarDemoView.h : CBarDemoView 类的接口
//


#pragma once


class CBarDemoView : public CFormView
{
protected: // 仅从序列化创建
	CBarDemoView();
	DECLARE_DYNCREATE(CBarDemoView)

public:
	enum{ IDD = IDD_BARDEMO_FORM };

// 属性
public:
	CBarDemoDoc* GetDocument() const;

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
	virtual ~CBarDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // BarDemoView.cpp 中的调试版本
inline CBarDemoDoc* CBarDemoView::GetDocument() const
   { return reinterpret_cast<CBarDemoDoc*>(m_pDocument); }
#endif

