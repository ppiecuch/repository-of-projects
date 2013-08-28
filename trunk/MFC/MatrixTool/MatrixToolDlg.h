// MatrixToolDlg.h : 头文件
//

#pragma once
#include <iostream>
#include <conio.h>
#include <math.h>
using namespace std;


// CMatrixToolDlg 对话框
class CMatrixToolDlg : public CDialog
{
// 构造
public:
	CMatrixToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MATRIXTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	* 将矩阵单位化
	*/
	inline void identityMatrix4(double m[16]){
		m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
		m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
		m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
		m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
	}
	/**
	* 计算矩阵src的逆矩阵保存到dest
	* 原理：首先将结果矩阵单位化，然后将源矩阵单位化，
	* 同时将运算规则应用于结果矩阵，最后结果矩阵便是源矩阵的逆矩阵
	*/
	inline bool inverseMatrix4(const double src[16],double result[16])
	{
		int i, j, k;
		double temp;
		double tempMatrix[4][4];                //临时矩阵
		//复制src矩阵存到临时矩阵中
		for (i=0; i<4; i++)
			for (j=0; j<4; j++)
				tempMatrix[i][j] = src[(i<<2)+j];
		//单位化result矩阵
		identityMatrix4(result);
		for (i = 0; i<4; i++)
		{
			//寻找主元
			k = i;
			for (j=i+1; j<4; j++)
			{
				if (fabs(tempMatrix[j][i]) > fabs(tempMatrix[i][i]))
					k = j;
			}
			//如果主元所在行不是第i行，进行行交换
			if (k != i)
			{
				for (j=0; j<4; j++)
				{
					temp = tempMatrix[i][j];
					tempMatrix[i][j] = tempMatrix[k][j];
					tempMatrix[k][j] = temp;
					//result伴随交换
					temp = result[(i<<2)+j];
					result[(i<<2)+j] = result[(k<<2)+j];
					result[(k<<2)+j] = temp;
				}
			}
			//判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵
			//TRACE("tempMatrix[%d][%d]=%.2f\r\n",i,i,tempMatrix[i][i] );
			if (tempMatrix[i][i] == 0)
				return false;
			//消去src的第i列除去i行以外的各行元素
			temp = tempMatrix[i][i];
			for (j=0; j<4; j++)
			{
				tempMatrix[i][j] /=temp;        //主对角线上的元素变为1
				result[(i<<2)+j] /= temp;       //伴随计算
			}
			for (j=0; j<4; j++)	
			{
				if (j != i)						//不是第i行
				{
					temp = tempMatrix[j][i];
					for (k=0; k<4; k++)        //第j行元素 - i行元素*j列i行元素
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
	* 转置矩阵
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
	 * 矩阵相乘
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

		CString strTemp = str; //此赋值不能少
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
		if(OpenClipboard())            // 将str复制到剪贴板 
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
