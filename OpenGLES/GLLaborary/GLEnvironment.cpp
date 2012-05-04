#include "StdAfx.h"
#include "GLEnvironment.h"

GLEnvironment::GLEnvironment(void)
{
}

GLEnvironment::~GLEnvironment(void)
{
}

PFNGLMULTITEXCOORD2FARBPROC GLEnvironment::glMultiTexCoord2fARB = NULL;
PFNGLACTIVETEXTUREARBPROC   GLEnvironment::glActiveTextureARB = NULL;
//CBMPLoader GLEnvironment::m_texture[TEXTUER_COUNT];
OpenGL21::TGAImage GLEnvironment::m_texture[TEXTUER_COUNT];
unsigned int GLEnvironment::m_textureId[TEXTUER_COUNT];
bool GLEnvironment::hasTexture=false;
GLhandleARB GLEnvironment::p=NULL;
/** 检查是否支持扩展 */
bool GLEnvironment::isExtensionSupported(const char* string)
{
	char *extension;			/**< 指向扩展字符串的指针 */
	char *end;				    /**< 最后一个字符指针 */
	int idx;

	extension = (char*)glGetString(GL_EXTENSIONS);
	if (extension == NULL)
		return false;	

	/** 得到最后一个字符 */
	end = extension + strlen(extension);	

	/** 循环查找字符串string */
	while (extension < end)
	{
		/** 找到空格前的一个字符串 */
		idx = strcspn(extension, " ");

		/** 检查是否找到字符串string */
		if ( (strlen(string) == idx) && (strncmp(string, extension, idx) == 0))
		{
			return true;
		}

		/** 当前串不是string,继续下一个字符串 */
		extension += (idx + 1);
	}
	/** 没有找到,则返回false */
	return false;   
}
bool GLEnvironment::initMultiTexture()
{
	/** 检查是否支持扩展 */
	if (isExtensionSupported("GL_ARB_multitexture"))
	{

		/** 获得函数指针地址 */
		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
		glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");

		return true;
	}
	else 
		return false;
}
void GLEnvironment::destroy()
{
	if(hasTexture==false)
		return;
	for(int i=0; i<TEXTUER_COUNT; i++)
	{
		//m_texture[i].FreeImage();
		//glDeleteTextures(1,&m_texture[i].ID);

		glDeleteTextures(1,&m_textureId[i]);
	}
	glDeleteObjectARB(p);
	hasTexture=false;
}

//temp
/** 载入纹理数据  */
bool GLEnvironment::loadTexture()
{
// 	if(hasTexture)
// 		return true;
// 	/// 文件名 
// 	char* fileName[TEXTUER_COUNT] ={ "media/rock.bmp","media/dirt.bmp","media/brick.bmp","media/grass.bmp","media/blend.bmp"};
// 
// 	/// 载入四幅位图 
// 	for(int i=0; i<TEXTUER_COUNT; i++)
// 	{
// 		if(i<TEXTUER_COUNT-1)
// 		{
// 			if(!m_texture[i].Load(fileName[i]))
// 			{
// 				MessageBox(NULL,_T("载入位图文件失败!"),_T("错误"),MB_OK);
// 				exit(-1);
// 			}	
// 		}
// 		else
// 		{
// 			if(!m_texture[i].LoadTemp())
// 			{
// 				MessageBox(NULL,_T("载入位图文件失败!"),_T("错误"),MB_OK);
// 				exit(-1);
// 			}	
// 		}
// 	}
// 	hasTexture=true;
// 	return true;

	if(hasTexture)
		return true;
	/// 文件名 
	char* fileName[TEXTUER_COUNT] ={"media/rock.tga","media/dirt.tga","media/brick.tga","media/grass.tga","media/blend.tga"};

	/// 载入四幅位图 
	for(int i=0; i<TEXTUER_COUNT; i++)
	{
		m_texture[i].CreateFromFile(fileName[i]);

		glGenTextures(1, &m_textureId[i]);
		glBindTexture(GL_TEXTURE_2D, m_textureId[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
// 		if(i==TEXTUER_COUNT-1)
// 		{
// 			unsigned char* temp=new unsigned char[m_texture[i].GetWidth()*m_texture[i].GetHeight()*4];
// 			for(int j=0;j<m_texture[i].GetWidth()*m_texture[i].GetHeight();j++)
// 			{
// 				temp[j*4+0]=temp[j*4+1]=temp[j*4+2]=((unsigned char*)m_texture[i].GetImageData())[j*4];
// 				temp[j*4+3]=((unsigned char*)m_texture[i].GetImageData())[j*4+1];
// 			}
// 			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, m_texture[i].GetWidth(),
// 				m_texture[i].GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE,
// 				temp);
// 			delete[] temp;
// 		}
// 		else
// 		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, m_texture[i].GetWidth(),
				m_texture[i].GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE,
				m_texture[i].GetImageData());
		//}
	}
	hasTexture=true;
	return true;

}

// check whether GLEW and GLSL are supported
bool GLEnvironment::check()
{
	const char * version = (const char*)glGetString( GL_VERSION );

	TRACE("OpenGL version : %s\r\n", version);


	// check OpenGL
	if ( glewIsSupported("GL_VERSION_2_0") )
	{
		TRACE("Ready for OpenGL 2.0\r\n");
	}
	else
	{
		TRACE("OpenGL 2.0 not supported\r\n");

		return false;
	}

	// check extensions
	if ( GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader )
	{
		TRACE("Ready for GLSL\r\n");
	}
	else
	{
		TRACE("Not totally ready\r\n");

		return false;
	}


	return true;

}

void GLEnvironment::setShaders()
{
	GLhandleARB v,f;
	char *vs = NULL,*fs = NULL;

	//创建着色器对象
	v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	vs = textFileRead("test.vert");
	fs = textFileRead("test.frag");

	const char *vv = vs;
	const char *ff = fs;

	//加载着色器源码
	glShaderSourceARB(v,1,&vv,NULL);
	glShaderSourceARB(f,1,&ff,NULL);

	free(vs);
	free(fs);

	//编译着色器源码
	glCompileShaderARB(v);
	glCompileShaderARB(f);

	/*printInfoLog(v);
	printInfoLog(f);*/

	//创建程序对象（相当于着色器对象的容器）
	p = glCreateProgramObjectARB();

	//将着色器对象绑定到程序对象上
	glAttachObjectARB(p,v);
	glAttachObjectARB(p,f);

	//链接着色器对象
	glLinkProgramARB(p);
	//printInfoLog(p);

	//安装为OpenGL状态机的一部分
	glUseProgramObjectARB(p);

	glDeleteObjectARB(v);
	glDeleteObjectARB(f);



}