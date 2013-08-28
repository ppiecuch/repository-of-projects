// 
// CFloatEdit implementation 
// Daniel Gabroveanu, January 2, 1999
// updated - March 14, 1999
//

#include "stdafx.h"
#include "FloatTest.h"
#include "FloatEdit.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_LENGTH            100
#define INT_KEY_DELETE        46
#define INT_KEY_INSERT        45
#define MINUS_SIGN            '-'
#define PLUS_SIGN             '+'
#define DEFAULT_COMMA         ','
#define DEFAULT_DECIMAL_POINT '.'

#define RETRIEVE_BUFFER_SPACE 100

/////////////////////////////////////////////////////////////////////////////
// CFloatEdit

CFloatEdit::CFloatEdit():m_wnd(NULL)
{
  m_bCommas = TRUE; // by default use commas
  m_cComma = DEFAULT_COMMA; // comma and decimal characters
  m_cDecimalPoint = DEFAULT_DECIMAL_POINT; 
  m_nDecimals = 2; // the number of decimal places
  m_dblCurrentValue = 0.0;

  // take the local system settings
  LocalSettings();
}

CFloatEdit::~CFloatEdit()
{
}


BEGIN_MESSAGE_MAP(CFloatEdit, CEdit)
	//{{AFX_MSG_MAP(CFloatEdit)
	ON_WM_CHAR()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFloatEdit message handlers

double CFloatEdit::GetValue(void) // get the stored value
{
  // the m_dblCurrentValue is currently updated
  return m_dblCurrentValue;
}

void CFloatEdit::SetValue(double dblValue) // set the new stored value
{
  // set the current value to the member variable
  m_dblCurrentValue = dblValue;

  // determine the appropriate string
  CString strCurrent;
  toString(m_dblCurrentValue, strCurrent);

  // set the current string
  setStringAndSelection(strCurrent, 0);

  // set the selection to the first character
  SetSel(0, 0);
}

int CFloatEdit::GetDecimals( void ) // get the decimal places
{
  return m_nDecimals;
}

void CFloatEdit::SetDecimals( int nDecimals ) // set the decimal places
{
  m_nDecimals = nDecimals;
  SetValue(m_dblCurrentValue);
}

void CFloatEdit::EnableCommas(BOOL bNewHasCommas) // enable / disable commas
{
  m_bCommas = bNewHasCommas;
  SetValue(m_dblCurrentValue);  // refresh contents
}

void CFloatEdit::SetComma(char cNewComma) // setting the comma character
{
  m_cComma = cNewComma;
  SetValue(m_dblCurrentValue); // refresh contents
}

char CFloatEdit::GetComma(void)  // getting the comma characters
{
  return m_cComma;
}

void CFloatEdit::SetDecimalPoint(char cNewDecimal) // setting the decimal point character
{
  m_cDecimalPoint = cNewDecimal;
  SetValue(m_dblCurrentValue); // refresh contents
}

char CFloatEdit::GetDecimalPoint(void)  // getting the decimal point character
{
  return m_cDecimalPoint;
}


// private function implementation
void CFloatEdit::toString(double dblValue, CString & strCurrent)
{
  CString strNumber;
  CString strInteger, strDecimal; 
  CString strWork;

  int nDecimalPosition, nSign;

  strNumber = CString(_ecvt(dblValue, MAX_LENGTH, &nDecimalPosition, &nSign));
  strInteger = strNumber.Left(nDecimalPosition);
  strDecimal = midString(strNumber, nDecimalPosition, m_nDecimals);
  if(m_nDecimals != 0)
    strWork = strInteger + CString(m_cDecimalPoint) + strDecimal;
  else
    strWork = strInteger;

  // put commas if necessary
  putCommas(strWork, strCurrent);

  // attach the sign to the current string
  if(nSign == 1)
    strCurrent = CString(MINUS_SIGN) + strCurrent;
}

void CFloatEdit::putCommas(CString strSource, CString & strDestination)
{
  CString strWork = strSource;
  
  // remove existing commas
  removeCommas(strWork);

  strDestination = strWork;

  if(m_bCommas)
  {
    // split the current string into decimal and integer part
    CString strInteger, strDecimal;
    getIntegerAndDecimal(strWork, strInteger, strDecimal);

    // put commas into the integer part
    int nNumber = 0;
    CString strCommaInteger = CString("");
    for(int nCount = 0; nCount < strInteger.GetLength(); nCount ++)
    {
    
      strCommaInteger = 
        midString(strInteger, strInteger.GetLength() - nCount - 1, 1) 
        + strCommaInteger;

      nNumber ++;
      nNumber %= 3;
      if(nNumber == 0)
      {
        strCommaInteger = CString(m_cComma) + strCommaInteger;
      }
    }

    // reassemble the integer and the decimal part through the decimal point
    if(m_nDecimals == 0)
      strDestination = strCommaInteger;
    else
      strDestination = strCommaInteger + 
                    CString(m_cDecimalPoint) + strDecimal;

    // remove the comma if the first character
    removeFirstComma(strDestination);
  } // if m_bCommas - will be processes as is
}

void CFloatEdit::removeCommas( CString & strSource)
{
  int nCount;
  CString strWork = strSource;
  CString strResult = CString("");

  for(nCount = 0; nCount < strSource.GetLength(); nCount ++)
  {
    if(strWork.GetAt(nCount) != m_cComma)
    {
      strResult += CString(strWork.GetAt(nCount));
    }
  } // for

  strSource = strResult;
}


void CFloatEdit::deleteCharLeft(CString &strSource, int nSelection, int nNumber)
{
  int nCurrentSelection  = nSelection - nNumber;
  deleteCharRight(strSource, nCurrentSelection, nNumber);
}

void CFloatEdit::deleteCharRight(CString &strSource, int nSelection, int nNumber)
{
  // splitting the string into three parts, and assembling the first and the last
  CString str1, str3;
  str1 = strSource.Left(nSelection);
  str3 = midString(strSource, nSelection + nNumber, strSource.GetLength());

  strSource = str1 + str3;
}


BOOL CFloatEdit::bIsLeftDecimal(CString strSource, int nSelection)
{
  BOOL bResult = TRUE;

  int nPos = strSource.Find(m_cDecimalPoint);
  if(nPos != -1)
  {
    if(nSelection > nPos)
      bResult = FALSE;
  } // if

  return bResult;
}


BOOL CFloatEdit::bIsLeftCharacterComma(CString strSource, int nSelection)
{
  BOOL bResult = FALSE;

  if(nSelection > 0)
  {
    if(strSource.GetAt(nSelection - 1) == m_cComma)
    {
      bResult = TRUE;
    }
  }

  return bResult;
}

BOOL CFloatEdit::bIsRightCharacterComma(CString strSource, int nSelection)
{
  BOOL bResult = FALSE;

  if(nSelection < strSource.GetLength())
  {
    if(strSource.GetAt(nSelection) == m_cComma)
    {
      bResult = TRUE;
    }
  }

  return bResult;
}


BOOL CFloatEdit::bIsLeftCharacterDecimal(CString strSource, int nSelection)
{
  BOOL bResult = FALSE;

  if(nSelection > 0)
  {
    if(strSource.GetAt(nSelection - 1) == m_cDecimalPoint)
    {
      bResult = TRUE;
    }
  }

  return bResult;
}

BOOL CFloatEdit::bIsRightCharacterDecimal(CString strSource, int nSelection)
{
  BOOL bResult = FALSE;

  if(nSelection < strSource.GetLength())
  {
    if(strSource.GetAt(nSelection) == m_cDecimalPoint)
    {
      bResult = TRUE;
    }
  }

  return bResult;
}


void CFloatEdit::getStringAndSelection(CString& strCurrent, int & nSelection)
{
  GetWindowText(strCurrent);
  
  int nStartSel, nEndSel;
  GetSel(nStartSel, nEndSel);

  nSelection = nStartSel;
}

void CFloatEdit::setStringAndSelection(CString strNew, int nSelection)
{
  SetWindowText(strNew.GetBuffer(1));
  
  // adjusting the selection value
  nSelection = nSelection < 0 ? 0 : nSelection;
  nSelection = nSelection > strNew.GetLength() ? strNew.GetLength() : nSelection;

  SetSel(nSelection, nSelection);

  // recalculate the value for m_dblCurrentValue
  CString strWork = strNew;
  CString strInteger, strDecimal;

  // will work by default with the decimal point
  removeCommas(strWork);
  getIntegerAndDecimal(strWork, strInteger, strDecimal);
  strWork = strInteger + CString(".") + strDecimal;

  m_dblCurrentValue = atof(strWork.GetBuffer(1));
}


void CFloatEdit::getIntegerAndDecimal(CString strCurrent, 
                                      CString & strInteger, 
                                      CString & strDecimal)
{
  CString strWork;
  strWork = strCurrent;

  // remove the commas from the given string
  removeCommas(strWork);

  // search for the decimal point
  int nPos = strWork.Find(CString(m_cDecimalPoint));
  if(nPos == -1)
  {
    strInteger = strWork;
    strDecimal = CString("");
  }
  else
  {
    strInteger = strWork.Left(nPos);
    strDecimal = midString(strWork, nPos + 1, strWork.GetLength());
  }
}

void CFloatEdit::saveStringAndSelection(void)
{
  CString strCurrent;
  int nStartSel, nEndSel;

  GetSel(nStartSel, nEndSel);
  GetWindowText(strCurrent);

  m_strSavedString = strCurrent;
  m_nSavedSelection = nStartSel;
}


void CFloatEdit::deleteSelection(void)
{
  // get the current selection
  int nSelStart, nSelEnd;
  GetSel(nSelStart, nSelEnd);

  // remove it
  if(nSelEnd > nSelStart)
  {
    ReplaceSel("");
  }
  
  // recalculate the value stored in the filled number
  CString strControl;
  GetWindowText(strControl);
  
  setStringAndSelection(strControl, nSelStart);
  SetValue(m_dblCurrentValue);

  SetSel(nSelStart, nSelStart);
}

void CFloatEdit::processCharacter( UINT cCurrent)
{
  // will process the input character inserted there
  // the character can be 
  // - digit

  if(cCurrent == MINUS_SIGN || cCurrent == PLUS_SIGN)
  {
    deleteSelection();
    processSign(cCurrent);
  }


  if(cCurrent >= '0' && cCurrent <= '9')
  {
    deleteSelection();
    processDigit(cCurrent);
  }

  // - backspace
  if(cCurrent == 8)
  {
    if(hasSelection())
      deleteSelection();
    else
      processBackspace();
  }

  // - decimal point
  if(cCurrent == (UINT) m_cDecimalPoint)
  {
    deleteSelection();
    processDecimalPoint();
  }

}


void CFloatEdit::processDigit(UINT cCurrent)
{

  // get the current text and selection
  CString strCurrent;
  int nSelection;

  getStringAndSelection(strCurrent, nSelection);

  // if the selection is on the left of the decimal point
  if(bIsLeftDecimal(strCurrent, nSelection))
  { 
    int nDelta;
    CString strDestination;

    nDelta = strCurrent.GetLength() - nSelection;
    
    strCurrent = strCurrent.Left(nSelection) + 
      CString((char) cCurrent) + 
      midString(strCurrent, nSelection, strCurrent.GetLength());

    putCommas(strCurrent, strDestination);
    strCurrent = strDestination;
    nSelection = strCurrent.GetLength() - nDelta;

  }
  // if the selection is on the right of the decimal point
  else
  { 
    // if there are digits available, will overwrite the next character
    if(nSelection < strCurrent.GetLength())
    {
      strCurrent.SetAt(nSelection, (char) cCurrent);
      nSelection++;

    } // if
  }

  // if the current selection is on the left of the comma, will jump
  if(bIsRightCharacterComma(strCurrent, nSelection))
  {
    nSelection ++;
  }
  
  // set the current text and selection
  setStringAndSelection(strCurrent, nSelection);
}

void CFloatEdit::processDelete()
{
  CString strCurrent;
  int nSelection;

  getStringAndSelection(strCurrent, nSelection);

  if(bIsRightCharacterDecimal(strCurrent, nSelection))
    nSelection ++;

  if(bIsLeftDecimal(strCurrent, nSelection))
  {    // on the left side of the decimal point
    int nDelta = strCurrent.GetLength() - nSelection;
    
    if(bIsRightCharacterComma(strCurrent, nSelection))
    {
      deleteCharRight(strCurrent, nSelection, 2);
      nDelta -= 2;
    }
    else
    {
      deleteCharRight(strCurrent, nSelection, 1);
      nDelta -= 1;
    }
    
    CString strDestination;
    putCommas(strCurrent, strDestination);
    strCurrent = strDestination;
    nSelection = strCurrent.GetLength() - nDelta;
  }
  else // on the right side of the decimal point
  {
    if(nSelection < strCurrent.GetLength())
    {
      strCurrent = strCurrent.Left(nSelection) + 
        midString(strCurrent, nSelection + 1, strCurrent.GetLength()) +
        CString("0");
    } 
  }

  setStringAndSelection(strCurrent, nSelection);
}

void CFloatEdit::processBackspace()
{
  CString strCurrent;
  int nSelection;

  getStringAndSelection(strCurrent, nSelection);

  if(bIsLeftCharacterDecimal(strCurrent, nSelection))
    nSelection --;

  if(bIsLeftDecimal(strCurrent, nSelection))
  {    // on the left side of the decimal point
    int nDelta = strCurrent.GetLength() - nSelection;
    
    if(bIsLeftCharacterComma(strCurrent, nSelection))
    {
      deleteCharLeft(strCurrent, nSelection, 2);
    }
    else
    {
      deleteCharLeft(strCurrent, nSelection, 1);
    }
    
    CString strDestination;
    putCommas(strCurrent, strDestination);
    strCurrent = strDestination;
    nSelection = strCurrent.GetLength() - nDelta;
  }
  else // on the right side of the decimal point
  {
    strCurrent = strCurrent.Left(nSelection-1) + 
      midString(strCurrent, nSelection, strCurrent.GetLength()) +
      CString("0");

    nSelection --;
  }

  setStringAndSelection(strCurrent, nSelection);
}

void CFloatEdit::processDecimalPoint()
{
  CString strCurrent;
  int nSelection;

  getStringAndSelection(strCurrent, nSelection);

  // if on the left side, delete all through the decimal point
  // if on the right side, not delete anything
  // afterwards, will move the selection after the decimal point

  int nPos = strCurrent.Find(CString(m_cDecimalPoint));
  nPos = nPos == -1 ? strCurrent.GetLength() : nPos;

  if(bIsLeftDecimal(strCurrent, nSelection))
  {
    strCurrent = strCurrent.Left(nSelection) + midString(strCurrent, nPos, strCurrent.GetLength());
  }

  CString strDestination;
  putCommas(strCurrent, strDestination);
  strCurrent = strDestination;

  nPos = strCurrent.Find(CString(m_cDecimalPoint));
  nPos = nPos == -1 ? strCurrent.GetLength() : nPos;
  nSelection = nPos + 1;
  nSelection = 
    nSelection > strCurrent.GetLength() ? 
    strCurrent.GetLength() : nSelection;

  setStringAndSelection(strCurrent, nSelection);
}

void CFloatEdit::processSign(UINT nCharacter)
{
  // toggles the sign character on the first position
  CString strCurrent;
  int nSelection;

  getStringAndSelection(strCurrent, nSelection);

  if(nCharacter == MINUS_SIGN)
  {
    // toggles the sign
    if(bIsRightCharacterSign(strCurrent, 0))
    {
      strCurrent = strCurrent.Right(strCurrent.GetLength() - 1);
      nSelection --;
    }
    else
    {
      strCurrent = CString(MINUS_SIGN) + strCurrent;
      nSelection ++;
    }
  }

  if(nCharacter == PLUS_SIGN)
  {  
    // removes the sign character if existent
    if(bIsRightCharacterSign(strCurrent, 0))
    {
      strCurrent = strCurrent.Right(strCurrent.GetLength() - 1);
      nSelection --;
    }
  }

  setStringAndSelection(strCurrent, nSelection);
}

void CFloatEdit::OnChar(UINT nChar, UINT, UINT) 
{
  processCharacter(nChar);
}

BOOL CFloatEdit::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == INT_KEY_DELETE)
  {
    if(hasSelection())
      deleteSelection();
    else
      processDelete();

    return TRUE;
  }

  if(pMsg->message == WM_KEYDOWN && pMsg->wParam == INT_KEY_INSERT)
  {
    return TRUE;
  }

  if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
  {
	 if(m_wnd)
	 {
		 return m_wnd->SendMessage(m_msgId);
	 }
  }
	
  return CEdit::PreTranslateMessage(pMsg);
}


void CFloatEdit::removeFirstComma(CString & strDestination)
{
  BOOL bSign = FALSE;
  CString strWork = strDestination;

  if(strWork.GetLength() > 0)
  {
    if(strWork.GetAt(0) == MINUS_SIGN)
    {
      bSign = TRUE;
      strWork = strWork.Right(strWork.GetLength() - 1);
    }
  }

  if(strWork.Left(1) == CString(m_cComma))
    strWork = strWork.Right(strWork.GetLength() - 1);

  if(bSign)
    strWork = CString(MINUS_SIGN) + strWork;

  strDestination = strWork;
}


BOOL CFloatEdit::bIsRightCharacterSign(CString strCurrent, int nSelection)
{
  // return true whether the right character is the sign character
  BOOL bResult = FALSE; 
  
  if(nSelection < strCurrent.GetLength() && nSelection >= 0)
  {
    if(strCurrent.GetAt(nSelection) == MINUS_SIGN)
      bResult = TRUE;
  } // if

  return bResult;
}

void CFloatEdit::OnRButtonDown(UINT, CPoint) 
{
  // disable the right menu button menu
}

CString CFloatEdit::midString(CString strIn, int nStart, int nLength)
{
  // getting hte mid string, as Mid member from CString does
  CString strResult = CString("");

  if(nStart >= 0 && nStart < strIn.GetLength())
  {
    strResult = strIn.Mid(nStart, nLength);
  }

  return strResult;
}


BOOL CFloatEdit::hasSelection(void)
{
  BOOL bResult = FALSE;

  // getting the current selection
  int nSelStart, nSelEnd;
  GetSel(nSelStart, nSelEnd);

  // setting the result
  if(nSelEnd > nSelStart)
    bResult = TRUE;


  return bResult;
}

void CFloatEdit::getLocaleInformation(char & cDecimal, char & cComma)
{
  struct lconv * pLocalStructure;
  pLocalStructure = localeconv();
  
  cDecimal = * (pLocalStructure->decimal_point);
  cComma = *( pLocalStructure->currency_symbol);
}

void CFloatEdit::LocalSettings(void)
{
  char cDecimal[RETRIEVE_BUFFER_SPACE];
  char cComma[RETRIEVE_BUFFER_SPACE];

  ::GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SDECIMAL, cDecimal, RETRIEVE_BUFFER_SPACE);
  ::GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_STHOUSAND, cComma, RETRIEVE_BUFFER_SPACE);

  m_cDecimalPoint = *cDecimal;

  // decimal point and thousands separator should be different
  if(*cDecimal != *cComma)
    m_cComma = *cComma;
}
void CFloatEdit::SetEnterMsgId(CWnd* wnd,UINT msgId)
{
	this->m_wnd=wnd;
	this->m_msgId=msgId;
}
