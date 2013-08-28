// DeliverView.h : CDeliverView 类的接口
//


#pragma once


class CDeliverView : public CFormView
{
protected: // 仅从序列化创建
	CDeliverView();
	DECLARE_DYNCREATE(CDeliverView)

public:
	enum{ IDD = IDD_DELIVER_FORM };

// 属性
public:
	CDeliverDoc* GetDocument() const;

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
	virtual ~CDeliverView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DeliverView.cpp 中的调试版本
inline CDeliverDoc* CDeliverView::GetDocument() const
   { return reinterpret_cast<CDeliverDoc*>(m_pDocument); }
#endif

