// Drawing.h: interface for the CDrawing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWING_H__9395007D_81C6_4197_9BF4_D0D97136E6A8__INCLUDED_)
#define AFX_DRAWING_H__9395007D_81C6_4197_9BF4_D0D97136E6A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDrawing  
{
public:
	void DrawScene4();
	void DrawTestScene();
	void DrawRenderScene();
	void InitSetting();
	void DrawScene1();
	void PrepareSetting(GLfloat xpos, GLfloat ypos, GLfloat xrot, GLfloat yrot, GLfloat zoom);
	void SpecialEffect();
	void SetRC();
	void SetView(CView *pView, int index);
	CDrawing();
	virtual ~CDrawing();

private:

	GLdouble    m_dNearPlane; 
	GLdouble    m_dFarPlane; 
	GLfloat		m_zoom,
				m_xpos,
				m_ypos,
				m_xrot,
				m_yrot,
				m_zrot;	
	bool multitexturing;
};

#endif // !defined(AFX_DRAWING_H__9395007D_81C6_4197_9BF4_D0D97136E6A8__INCLUDED_)
