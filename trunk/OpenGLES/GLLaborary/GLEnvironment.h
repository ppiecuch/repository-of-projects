#pragma once

#define TEXTUER_COUNT 5
class GLEnvironment
{
protected:
	static GLhandleARB p;
public:
	GLEnvironment(void);
	~GLEnvironment(void);

	/** 多重纹理函数 */
	static PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
	static PFNGLACTIVETEXTUREARBPROC   glActiveTextureARB;
	//static CBMPLoader m_texture[TEXTUER_COUNT];
	static OpenGL21::TGAImage m_texture[TEXTUER_COUNT];
	static unsigned int m_textureId[TEXTUER_COUNT];
	static bool hasTexture;
	/** 检查是否支持扩展 */
	static bool isExtensionSupported(const char* string);
	static bool initMultiTexture();
	static bool loadTexture();
	static void destroy();
	static bool check();
	static void setShaders();
};
