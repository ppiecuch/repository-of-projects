// ProgressDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "02Progress.h"
#include "ProgressDlg.h"


// CProgressDlg �Ի���

IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
{

}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProgressDlg ��Ϣ�������

BOOL CProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_progress.SetRange(0,100);
	m_progress.SetStep(1);
	m_progress.SetPos(0);

	SetTimer(1,100,NULL);//���ý���������ʱ��


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CProgressDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	m_progress.StepIt();
	if(m_progress.GetPos()>=100)
	{
		this->EndDialog(0);
	}

	CDialog::OnTimer(nIDEvent);
}
