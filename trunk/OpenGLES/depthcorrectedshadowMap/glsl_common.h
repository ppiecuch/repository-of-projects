///////////////////////////////////////////////////////////////////////////////
//
//	Simple Framework for OpenGL 2.0
//
//	note: requires GLEW (http://glew.sourceforge.net)
//
//	last update : 2005 SEP 24
//
//	author : KIST IMRC Jinwook Kim
//	e-mail : jwkim@imrc.kist.re.kr
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _GLSL_COMMON_
#define _GLSL_COMMON_

#include <GL/glew.h>
#include <GL/wglew.h>

#include <list>
#include <vector>
#include <string>

#define MAX_TEXTURE 8

using namespace std;

namespace GLSL_COMMON
{
	enum MouseButtonType	{ LEFT, MIDDLE, RIGHT };
	enum ProjectionType		{ PERSPECTIVE, ORTHOGONAL, PREDEFINED };
};

class glTransform
{
public:
						 glTransform();
	virtual void		 lookAt(float x, float y, float z, float h, float p, float r);
	void				 setProjectionMatrix(float fovy, float aspect, float zNear,  float zFar);

	void				 multTransform(void) const;
	void				 multInverseTransform(void) const;
	void				 getInverseEulerHPR(float *hpr, float *xyz = NULL) const;

	void				 setWindowSize(int x, int y);

	int					 getWindowWidth(void) const;
	int					 getWindowHeight(void) const;

protected:

	static int			 m_iWndSizeWidth, m_iWndSizeHeight;
	float				 m_fTransformMatrix[16], m_fPrevTransformMatrix[16], m_fProjectionMatrix[16];
	float				 m_fPrevSPoint[3], m_fPrevTransformPoint[3];
};

class glCamera : public glTransform
{
public:

						 glCamera();
						 glCamera(float near, float far, float fov);

	static glCamera		*getDefaultCamera(void);

	void				 applyProjection(GLSL_COMMON::ProjectionType type = GLSL_COMMON::PREDEFINED) const;
	virtual void		 lookAt(float x, float y, float z, float h, float p, float r);

	void				 mouseFunc(GLSL_COMMON::MouseButtonType button, int state, int x, int y);
	void				 motionFunc(int x, int y);
	void				 reshapeFunc(int w, int h);
	
	float				 getNear(void) const;
	float				 getFar(void) const;
	float				 getFOV(void) const;
	
	void				 setNearFar(float near, float far);
	void				 setFOV(float fov);
	void				 setOrthoSize(float left, float right, float bottom, float top);
	
protected:

	void				_init(float near, float far, float fov);
	void				_pos2spoint(float pt[3], int mx, int my);
	void				_pos2mv_point(float mv[3], int mx, int my, int z);

	int					 m_iZoom;
	float				 m_fNear, m_fFar, m_fFOV, m_fLeft, m_fRight, m_fBottom, m_fTop, m_fDistance;
	static	glCamera	*m_pDefaultCamera;
	GLSL_COMMON::MouseButtonType	 m_eMouseButton;
	GLSL_COMMON::ProjectionType		 m_eProjectionType;
};

class glColor
{
public:
					glColor();
					glColor(float r, float g, float b, float a = 1.0f);

	const float		&operator [] (int) const;
	float			&operator [] (int);

	friend glColor	 RGB2HSL(const glColor &);
	friend glColor	 HSL2RGB(const glColor &);

protected:

	float			m_fVal[4];
};

class glLight : public glCamera
{
public:

						 glLight();
						~glLight();

	void				 enable(void);
	void				 disable(void);
	bool				 isEnabled(void) const;
	
	void				 applyProjection(void);
	void				 loadProjectionMatrix(void) const;
	void				 update(void);
	
	void				 setPosition(float x, float y, float z);
	void				 setAmbient(const glColor&);
	void				 setDiffuse(const glColor&);
	void				 setSpecular(const glColor&);
	void				 setAspectRatio(float);

	void				 getPosition(float pos[4]) const;
	void				 getAmbientColor(float color[4]) const;
	void				 getDiffuseColor(float color[4]) const;
	void				 getSpecularColor(float color[4]) const;
	int					 getID(void) const;
	const float			*getEyeCoordPosition(void) const;
	
	static glLight		*getLight(unsigned int idx);
	static int			 getNumLights(void);

protected:

	bool				 m_bEnabled;
	float				 m_fAspectRatio;
	int					 m_iID;
	static vector <glLight *> m_pLights;
};

class glTexture
{
	enum TEX_PARAM_NAME { WRAP_S, WRAP_T, WRAP_R, MIN_FILTER, MAG_FILTER, DEPTH_MODE, COMPARE_MODE, COMPARE_FUNC, GENERATE_MIPMAP };
	
public:

						 glTexture();
						~glTexture();

	void				 load(const char file[], bool generate_normal = false);
	void				 init(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, void *buf = NULL);
	bool				 save(const char file[], bool overwrite = false);

	void				 setTextureUnit(int texture_unit);
	void				 bind(int unit);
	void				 disable(int unit);
	void				 setTarget(GLenum target);
	void				 texGen(int pgen, int penv = GL_MODULATE);		// pgen = GL_OBJECT_LINEAR, GL_EYE_LINEAR, or GL_SPHERE_MAP
																		// penv = GL_MODULATE, GL_DECAL, GL_BLEND, or GL_REPLACE
	void				 setParameter(GLenum pname, GLint param);
	GLuint				 getID(void) const;
	int					 getWidth(void) const;
	int					 getHeight(void) const;
	int					 getDepth(void) const;
	bool				 isCubeMap(void) const;
	GLenum				 getTarget(void) const;
	int					 getInternalFormat(void) const;
	GLenum				 getFormat(void) const;
	GLenum				 getType(void) const;
	GLint				 getParameter(int) const;

protected:

	void				_clear(void);
	bool				_loadBMP(const char file[]);
	bool				_loadDDS(const char file[]);

	GLuint				 m_iTextureID;
	GLenum				 m_eTarget;
	int					 m_iHeight;
	int					 m_iWidth;
	int					 m_iDepth;
	int					 m_iMipmapCount;
	int 				 m_iInternalFormat;
	GLenum				 m_eFormat;
	GLenum				 m_eType;
	GLint				 m_iParam[9];

	void				*m_pData;
};

class glFramebuffer
{
public:
						 glFramebuffer();
						~glFramebuffer();

	void				 setTexture(const glTexture &, GLenum attachment = GL_COLOR_ATTACHMENT0_EXT);
	void				 enableDepthRenderbuffer(void);
	void				 bind(void);
	void				 release(void);

protected:

	GLuint				 m_iFBID;
	GLuint				 m_iRBID;
	const glTexture		*m_pTexture;
	bool				 m_bBound;
};

void drawOBJ(const char file[]);
void drawBox(float x, float y, float z, float texcoord = 1.0f);
void drawTorus(float ro, float ri, int longitude, int latitude);

class glDisplayList
{
public:

						 glDisplayList();
						~glDisplayList();

	void				 newList(GLenum mode = GL_COMPILE);
	void				 endList(void);
	void				 call(void) const;

protected:

	GLuint				 m_iID;
};

class glText
{
public:

	static void			 print(int x, int y, const char * string, ...);
	static void			 setFont(const wchar_t font_name[], int font_height, int font_width = 0);

protected:

	static GLuint		 base;

};

class glProgram;

class glShader
{
public:

						 glShader();
						~glShader();

						// shaderType = GL_VERTEX_SHADER or GL_FRAGMENT_SHADER 
	void				 loadShaderSource(GLenum shaderType, const GLchar *shader_src, bool isFile = true);
	GLuint				 addDefinition(const GLchar *definition);
	const char			*getInfoLog(void);
	GLuint				 getID(GLuint) const;

protected:

	void				_loadShader(void);

	GLenum				 m_eShaderType;
	string				 m_szShaderSrc;
	vector <GLuint>		 m_iShaderIDs;
	vector <string>		 m_szDefinitions;
	list <glProgram *>	 m_pProgramList;
    
friend class glProgram;
};

class glProgram
{
public:

						 glProgram();
						~glProgram();

	void				 attach(glShader *pShaderA, glShader *pShaderB = NULL);
	void				 setMask(glShader *pShader, GLuint mask);

	void				 enable(bool flag = true);
	void				 disable(void);
	bool				 isEnabled(void) const;

	void				 setUniform(const GLchar name[], int v0);
	void				 setUniform(const GLchar name[], int v0, int v1);
	void				 setUniform(const GLchar name[], int v0, int v1, int v2);
	void				 setUniform(const GLchar name[], int v0, int v1, int v2, int v3);
	void				 setUniform(const GLchar name[], int count, int dim, const int *v);
	void				 setUniform(const GLchar name[], float v0);
	void				 setUniform(const GLchar name[], float v0, float v1);
	void				 setUniform(const GLchar name[], float v0, float v1, float v2);
	void				 setUniform(const GLchar name[], float v0, float v1, float v2, float v3);
	void				 setUniform(const GLchar name[], int count, int dim, const float *v);
	void				 setUniformMatrix(const GLchar name[], int dim, const float *v);

	void				 getUniform(const GLchar name[], GLfloat values[]);
	void				 getUniform(const GLchar name[], GLint values[]);
	
	const char			*getInfoLog(void);

protected:
	
	struct UNIFORM_PAIR { string name; enum UNIFORM_TYPE { INTEGER, FLOAT, MATRIX } type; int count; int dim; void *val; };

	void				_applyUniform(const UNIFORM_PAIR *pPair = NULL) const;
	void				_link(void);
	void				_detach(glShader *);

	GLuint				 m_iProgramID;
	struct SHADER_PAIR	{ glShader *pShader; GLuint mask; };
	list <SHADER_PAIR>	 m_pShaderList;
	bool				 m_bLinked;
	bool				 m_bEnabled;
	int					 m_iMask;

	list <UNIFORM_PAIR *>	m_pUniformList;

	void				_setUniform(const GLchar name[], UNIFORM_PAIR::UNIFORM_TYPE type, int count, int dim, const void *val);

friend class glShader;
};

class glTimer
{
public:
	static void			tic();
	static float		toc();
	static float		getFPS(void);
	static void			suspend(void);
	static void			resume(void);

protected:
						glTimer();

	static float		m_fResolution;
	static int			m_nLowshift;
	static LONGLONG		m_sStart;
	static glTimer		m_sDefaultTimer;
	static bool			m_bSuspend;
	static LONGLONG		m_sSuspend;
};

class glMaterial
{
public:
					glMaterial();
	
	void			apply(void) const;
	void			setAmbient(const glColor &);
	void			setDiffuse(const glColor &);
	void			setSpecular(const glColor &);
	void			setEmission(const glColor &);
	void			setFace(GLenum);
	void			setShininess(float);
protected:
	GLenum			m_eFace;
	glColor			m_sAmbient;
	glColor			m_sDiffuse;
	glColor			m_sSpecular;
	glColor			m_sEmission;
	float			m_fShininess;

};

class glModel : public glDisplayList
{
public:
					 glModel();

	void			 setProgram(glProgram *program);
	void			 setTexture(glTexture *texture, int unit = 0);
	void			 setMaterial(glMaterial *material);

	void			 draw(bool enTex = true) const;

protected:
	glProgram		*m_pProgram;
	glMaterial		*m_pMaterial;
	glTexture		*m_pTexture[MAX_TEXTURE];
};

class glGeom
{
public:
					 glGeom();

protected:
	
	struct float3	{ float val[3]; };
	struct float2	{ float val[2]; };
	struct face		{ int vertex[3]; int normal[3]; int tex[3]; };

	vector <float3>	m_sVertex;
	vector <float3>	m_sNormal;
	vector <float2>	m_sTexCoord;
	vector <face>	m_sFace;
};

#endif