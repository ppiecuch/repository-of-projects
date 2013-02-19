// SDIFT2View.h : CSDIFT2View 类的接口
//


#pragma once


class CSDIFT2View : public CView
{
protected: // 仅从序列化创建
	CSDIFT2View();
	DECLARE_DYNCREATE(CSDIFT2View)

// 属性
public:
	CSDIFT2Doc* GetDocument() const;

	FT_Library	m_pFTLib;
	FT_Face		m_pFTFace;
	int			m_iZoom;


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
	virtual ~CSDIFT2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // SDIFT2View.cpp 中的调试版本
inline CSDIFT2Doc* CSDIFT2View::GetDocument() const
   { return reinterpret_cast<CSDIFT2Doc*>(m_pDocument); }
#endif

