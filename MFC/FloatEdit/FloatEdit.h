// 
// CFloatEdit prototypes
// Daniel Gabroveanu, January 2, 1999
//

#if !defined(AFX_FLOATEDIT_H__449B9681_9A73_11D2_B5AC_5254ABDD288C__INCLUDED_)
#define AFX_FLOATEDIT_H__449B9681_9A73_11D2_B5AC_5254ABDD288C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FloatEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFloatEdit window

class CFloatEdit : public CEdit
{
// Construction and methodes
public:
	CFloatEdit();
	virtual ~CFloatEdit();
  
  double GetValue(void); // get the stored value
  void SetValue(double dblValue); // set the new stored value

  int GetDecimals( void ); // get the decimal places
  void SetDecimals( int nDecimals ); // set the decimal places
  
  void EnableCommas(BOOL bNewHasCommas); // enable / disable commas
  
  void SetComma(char cNewComma); // setting the comma character
  char GetComma(void);  // getting the comma characters

  void SetDecimalPoint(char cNewDecimal); // setting the decimal point character
  char GetDecimalPoint(void);  // getting the decimal point character

  void LocalSettings(void); // apply the local settings for comma and decimal point

  void SetEnterMsgId( CWnd* wnd,UINT msgId);
                               

private:

// Attributes
private:
  double m_dblCurrentValue; // the current value
  BOOL m_bCommas;       // TRUE if has commas
  char m_cComma;        // the comma character
  char m_cDecimalPoint; // the decimal point character
  int m_nDecimals;      // the number of decimal places after the decimal point
  CString m_strSavedString;
  int m_nSavedSelection;
  CWnd* m_wnd;
  UINT m_msgId;


// Operations

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFloatEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
private:
  void removeFirstComma(CString &);
  void processSign( UINT nCharacter);
  void processCharacter( UINT cCurrent);
  void toString(double dblValue, CString & strCurrent);

  void putCommas(CString strSource, CString & strDestination);
  void removeCommas( CString & strSource);
  
  void deleteCharLeft(CString& strSource, int nSelection, int nNumber);
  void deleteCharRight(CString& strSource, int nSelection, int nNumber);
  
  BOOL bIsRightCharacterSign(CString strCurrent, int nSelection);
  BOOL bIsLeftDecimal(CString strSource, int nSelection);
  
  BOOL bIsLeftCharacterComma(CString strSource, int nSelection);
  BOOL bIsRightCharacterComma(CString strSource, int nSelection);
  
  BOOL bIsLeftCharacterDecimal(CString strSource, int nSelection);
  BOOL bIsRightCharacterDecimal(CString strSource, int nSelection);

  void getStringAndSelection(CString& strCurrent, int & nSelection);
  void setStringAndSelection(CString strNew, int nSelection);

  void getIntegerAndDecimal(CString strCurrent, CString & strInteger, CString & strDecimal);
  void saveStringAndSelection(void);
  void deleteSelection(void);

  void processDigit(UINT cCurrent);
  void processDelete();
  void processBackspace();
  void processDecimalPoint();

  void getLocaleInformation(char & cDecimal, char & cComma);

  BOOL hasSelection(void);

  CString midString(CString strIn, int nStart, int nLength);

	// Generated message map functions
protected:
	//{{AFX_MSG(CFloatEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOATEDIT_H__449B9681_9A73_11D2_B5AC_5254ABDD288C__INCLUDED_)
