// editor.cpp : implementation file
//

#include "stdafx.h"
#include "GameEditor.h"
#include "editor.h"
#include  "GameEditorDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Ceditor dialog


Ceditor::Ceditor(CWnd* pParent /*=NULL*/)
	: CDialog(Ceditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(Ceditor)
	//}}AFX_DATA_INIT
	s="";
	s2="";
    r2=r1=false;
	pt.x=0;
	pt.y=0;

}


void Ceditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Ceditor)
	DDX_Control(pDX, IDC_COMBO2, m_combox2);
	DDX_Control(pDX, IDC_COMBO1, m_combox1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Ceditor, CDialog)
	//{{AFX_MSG_MAP(Ceditor)
	ON_CBN_EDITCHANGE(IDC_COMBO1, OnEditchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Ceditor message handlers

void Ceditor::OnEditchangeCombo1() 
{
//	MessageBox("change!");
	
}

void Ceditor::OnSelchangeCombo1() 
{
	int i=m_combox1.GetCurSel();
	s.Format("%d",i);
    InvalidateRect(Real_rect);
}

void Ceditor::OnPaint() 
{
	  CPaintDC dc(this); // device context for painting  窗口的设备描述表
	  
	   dc.SetBkMode(TRANSPARENT);
      //在这里绘制选图区的图像
	  CRect rect;   
	  GetDlgItem(IDC_STATICSELECT)->GetWindowRect(&rect);
      CRect Dlg_rect;
	  GetWindowRect(Dlg_rect);
	  
	 
	  Real_rect.left=rect.left-Dlg_rect.left;
	  Real_rect.top=rect.top-Dlg_rect.top;
	  Real_rect.right=Real_rect.left+rect.Width();
	  Real_rect.bottom=Real_rect.top+rect.Height();
	 
	  ///////////////     在这里绘制具体的图像 示例中只写出选择的数据

	   dc.TextOut(Real_rect.left+5,Real_rect.top+25,s);	

       //////////////////////////////////////
        
	 //在这里绘制预览区的图像
       CRect rect2;
	   GetDlgItem(IDC_PREVIEW)->GetWindowRect(&rect2);
	   
	   Real_rect2.left=rect2.left-Dlg_rect.left;
	   Real_rect2.top=rect2.top-Dlg_rect.top;
	   Real_rect2.right=Real_rect2.left+rect2.Width();
	   Real_rect2.bottom=Real_rect2.top+rect2.Height();

       dc.TextOut(Real_rect2.left+5,Real_rect2.top+25,s2);
	   CRect select_rect;
	 
	   if(r2)
	   {
		    select_rect.left=Real_rect2.left+5;
			select_rect.top=Real_rect2.top+25;	      
			select_rect.right=Real_rect2.right-5;
	        select_rect.bottom=Real_rect2.bottom-5;
			dc.SetTextColor(RGB(255,0,0));
			dc.MoveTo(select_rect.left,select_rect.top);
			dc.LineTo(select_rect.right,select_rect.top);
         	dc.LineTo(select_rect.right,select_rect.bottom);
			dc.LineTo(select_rect.left,select_rect.bottom);
			dc.LineTo(select_rect.left,select_rect.top);
		
			//dc.MoveTo(select_rect.right,select_rect.bottom);		
	   }
      
	  if (r1)
	  { 
		  
		  select_rect.left=pt.x-16;
		  select_rect.top=pt.y-16;	      
		  select_rect.right=pt.x+16;
		  select_rect.bottom=pt.y+16;
		  dc.SetTextColor(RGB(255,0,0));
		  dc.MoveTo(select_rect.left,select_rect.top);
		  dc.LineTo(select_rect.right,select_rect.top);
          dc.LineTo(select_rect.right,select_rect.bottom);
		  dc.LineTo(select_rect.left,select_rect.bottom);
		  dc.LineTo(select_rect.left,select_rect.top);


	  }
	  
	   

}

void Ceditor::OnSelchangeCombo2() 
{
	int i=m_combox2.GetCurSel();
	s2.Format("%d",i);
    InvalidateRect(Real_rect2);
	
}

void Ceditor::OnPreview() 
{
  
	
}

void Ceditor::OnLButtonDown(UINT nFlags, CPoint point) 
{

	int  tag=0;
	if (Real_rect2.left<=point.x&&point.x<=Real_rect2.right&&Real_rect2.top<=point.y&&point.y<=Real_rect2.bottom)
	
	{

        r2=true; 
       tag=1;
	   ((CGameEditorDlg *)this->GetParent()->GetParent())->tag=tag;
	   ((CGameEditorDlg *)this->GetParent()->GetParent())->souce="预览";

	}

	 else r2=false;

	if (Real_rect.left<=point.x&&point.x<=Real_rect.right&&Real_rect.top<=point.y&&point.y<=Real_rect.bottom)
	
	{

	//	MessageBox("你已选择了选图区对象");
        r1=true;
		pt=point;
		tag=2;
	  ((CGameEditorDlg *)this->GetParent()->GetParent())->tag=tag;
	   ((CGameEditorDlg *)this->GetParent()->GetParent())->souce="地图图元";
		
	}

	 else r1=false;
	 
	  

		  InvalidateRect(Real_rect2);
	      InvalidateRect(Real_rect);
	      if (!r1&&!r2)
	      {
			  ((CGameEditorDlg *)this->GetParent()->GetParent())->tag=-1;

	      }
	       
	     
		CDialog::OnLButtonDown(nFlags, point);
}
