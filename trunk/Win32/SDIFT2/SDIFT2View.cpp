// SDIFT2View.cpp : CSDIFT2View ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CSDIFT2View ����/����

CSDIFT2View::CSDIFT2View() : m_pFTLib(NULL), m_pFTFace(NULL), m_iZoom(2)
{
	// TODO: �ڴ˴���ӹ������

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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CSDIFT2View ����

void CSDIFT2View::OnDraw(CDC* pDC)
{
	CSDIFT2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc || !m_pFTFace)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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


// CSDIFT2View ��ӡ

BOOL CSDIFT2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CSDIFT2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CSDIFT2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CSDIFT2View ���

#ifdef _DEBUG
void CSDIFT2View::AssertValid() const
{
	CView::AssertValid();
}

void CSDIFT2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSDIFT2Doc* CSDIFT2View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSDIFT2Doc)));
	return (CSDIFT2Doc*)m_pDocument;
}
#endif //_DEBUG


// CSDIFT2View ��Ϣ�������

void CSDIFT2View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
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
