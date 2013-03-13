// SDIFT2View.cpp : CSDIFT2View 类的实现
//

#include "stdafx.h"
#include "SDIFT2.h"

#include "SDIFT2Doc.h"
#include "SDIFT2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSDIFT2View

IMPLEMENT_DYNCREATE(CSDIFT2View, CView)

BEGIN_MESSAGE_MAP(CSDIFT2View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CSDIFT2View 构造/析构

CSDIFT2View::CSDIFT2View() : m_pFTLib(NULL), m_pFTFace(NULL), m_iZoom(2)
{
	// TODO: 在此处添加构造代码

}

CSDIFT2View::~CSDIFT2View()
{
	if(m_pFTFace)
	{
		FT_Done_Face(m_pFTFace);
		m_pFTFace = NULL;
	}
	if(m_pFTLib)
	{
		FT_Done_FreeType(m_pFTLib);
		m_pFTLib = NULL;
	}
}

BOOL CSDIFT2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CSDIFT2View 绘制

void CSDIFT2View::OnDraw(CDC* pDC)
{
	CSDIFT2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc || !m_pFTFace)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	FT_Glyph	glyph	= NULL;
	FT_Error	error	= 0;
	error = FT_Load_Glyph(m_pFTFace, FT_Get_Char_Index(m_pFTFace, 21183), FT_LOAD_DEFAULT);
	if(!error)
	{
		error = FT_Get_Glyph(m_pFTFace->glyph, &glyph);
		if(!error)
		{
			FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
			FT_BitmapGlyph	bitmap_glyph	= (FT_BitmapGlyph)glyph;
			FT_Bitmap&		bitmap			= bitmap_glyph->bitmap;
			for(int i=0; i<bitmap.rows; ++i)
			{
				for(int j=0; j<bitmap.width; ++j)
				{
					for(int k=0; k<m_iZoom; ++k)
					{
						for(int l=0; l<m_iZoom; ++l)
						{
							pDC->SetPixel(CPoint(100+j*m_iZoom+l, 100+i*m_iZoom+k), bitmap.buffer[i*bitmap.width+j]);
						}
					}
				}
			}
			FT_Done_Glyph(glyph);
			glyph = NULL;
		}
	}
}


// CSDIFT2View 打印

BOOL CSDIFT2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CSDIFT2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CSDIFT2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CSDIFT2View 诊断

#ifdef _DEBUG
void CSDIFT2View::AssertValid() const
{
	CView::AssertValid();
}

void CSDIFT2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSDIFT2Doc* CSDIFT2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSDIFT2Doc)));
	return (CSDIFT2Doc*)m_pDocument;
}
#endif //_DEBUG


// CSDIFT2View 消息处理程序

void CSDIFT2View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	if(m_pFTFace)
	{
		return;
	}
	FT_Error	error	= 0;
	error = FT_Init_FreeType(&m_pFTLib);
	if(error)
	{
		m_pFTLib = NULL;
		TRACE(_T("There is some error when Init Library..."));
		return;
	}

	error = FT_New_Face(m_pFTLib, "C:\\WINDOWS\\Fonts\\STKAITI.TTF", 0, &m_pFTFace);
	if(error)
	{
		m_pFTFace = NULL;
		TRACE(_T("There is some error when New Face..."));
		return;
	}
	FT_Set_Char_Size(m_pFTFace, 12<<6, 12<<6, 300, 300);
}
