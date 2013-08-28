// MatrixToolDlg.h : ͷ�ļ�
//

#pragma once
#include <iostream>
#include <conio.h>
#include <math.h>
using namespace std;


// CMatrixToolDlg �Ի���
class CMatrixToolDlg : public CDialog
{
// ����
public:
	CMatrixToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MATRIXTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	char m_separator;
public:
	afx_msg void OnBnClickedButtonInverse();
	afx_msg void OnBnClickedButtonInvert();
	afx_msg void OnBnClickedButtonCopyResult();
	void OnRadioSeparator (UINT nID);
protected:
	CString m_strInput;
	CString m_strOutput;

	/**
	* ������λ��
	*/
	inline void identityMatrix4(double m[16]){
		m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
		m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
		m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
		m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
	}
	/**
	* �������src������󱣴浽dest
	* ԭ�����Ƚ��������λ����Ȼ��Դ����λ����
	* ͬʱ���������Ӧ���ڽ������������������Դ����������
	*/
	inline bool inverseMatrix4(const double src[16],double result[16])
	{
		int i, j, k;
		double temp;
		double tempMatrix[4][4];                //��ʱ����
		//����src����浽��ʱ������
		for (i=0; i<4; i++)
			for (j=0; j<4; j++)
				tempMatrix[i][j] = src[(i<<2)+j];
		//��λ��result����
		identityMatrix4(result);
		for (i = 0; i<4; i++)
		{
			//Ѱ����Ԫ
			k = i;
			for (j=i+1; j<4; j++)
			{
				if (fabs(tempMatrix[j][i]) > fabs(tempMatrix[i][i]))
					k = j;
			}
			//�����Ԫ�����в��ǵ�i�У������н���
			if (k != i)
			{
				for (j=0; j<4; j++)
				{
					temp = tempMatrix[i][j];
					tempMatrix[i][j] = tempMatrix[k][j];
					tempMatrix[k][j] = temp;
					//result���潻��
					temp = result[(i<<2)+j];
					result[(i<<2)+j] = result[(k<<2)+j];
					result[(k<<2)+j] = temp;
				}
			}
			//�ж���Ԫ�Ƿ�Ϊ0, ����, �����A�������Ⱦ���,�����������
			//TRACE("tempMatrix[%d][%d]=%.2f\r\n",i,i,tempMatrix[i][i] );
			if (tempMatrix[i][i] == 0)
				return false;
			//��ȥsrc�ĵ�i�г�ȥi������ĸ���Ԫ��
			temp = tempMatrix[i][i];
			for (j=0; j<4; j++)
			{
				tempMatrix[i][j] /=temp;        //���Խ����ϵ�Ԫ�ر�Ϊ1
				result[(i<<2)+j] /= temp;       //�������
			}
			for (j=0; j<4; j++)	
			{
				if (j != i)						//���ǵ�i��
				{
					temp = tempMatrix[j][i];
					for (k=0; k<4; k++)        //��j��Ԫ�� - i��Ԫ��*j��i��Ԫ��
					{
						tempMatrix[j][k] -= tempMatrix[i][k]*temp;
						result[(j<<2)+k] -= result[(i<<2)+k]*temp;
					}
				}
			}
		}
		return true;
	}

	/**
	* ת�þ���
	*/
	inline void invertMatrix(double m[16],double r[16])
	{
		double temp[16];
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				temp[j*4+i]=m[4*i+j];
			}
		}
		for(int i=0;i<16;i++)
			r[i]=temp[i];
	}

	/*
	 * �������
	 */
	inline void multipleMatrix(double m1[16],double m2[16], double m3[16])
	{
		m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

		m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

		m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

		m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
	}

	inline void printMatrix(double m[16]){
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				if(j>0)
					cout<<",";
				cout<<m[i*4+j];
				TRACE(",%g",m[i*4+j]);
			}
			cout<<endl;
		}
		TRACE("\r\n");
	}

	inline int splitString(CString str, char split, CStringArray& strArray)
	{
		strArray.RemoveAll();

		CString strTemp = str; //�˸�ֵ������
		int nIndex = 0; //
		while(true)
		{
			nIndex = strTemp.Find(split);
			if( nIndex >= 0 )
			{
				strArray.Add( strTemp.Left( nIndex ) );
				strTemp = strTemp.Right( strTemp.GetLength() - nIndex - 1 );
			}
			else break;
		}
		strArray.Add( strTemp );
		return strArray.GetSize();
	}

	inline void copyToClipBoard(CString& str) 
	{ 
		if(OpenClipboard())            // ��str���Ƶ������� 
		{ 
			HGLOBAL clipbuffer; 
			char * buffer; 
			EmptyClipboard(); 
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, str.GetLength()+1); 
			buffer = (char*)GlobalLock(clipbuffer);
			size_t len = strlen(LPCSTR(str)) + 1;
			strcpy_s(buffer,len,LPCSTR(str)); 
			GlobalUnlock(clipbuffer); 
			SetClipboardData(CF_TEXT,clipbuffer); 
			CloseClipboard(); 
		} 
	}



public:
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonConvert();
	afx_msg void OnBnClickedButtonTransform();
	afx_msg void OnBnClickedButtonFormat();
	afx_msg void OnBnClickedButtonSort();
	afx_msg void OnBnClickedButtonGroup();
};
