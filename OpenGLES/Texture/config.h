//加载OpenGL ES2需要的库及头文件

#include "macro.h"

#if defined(GLES2)
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib, "libGLESv2.lib")
#include <GLES2/gl2.h>
#elif defined(GL)
#include <GL/glut.h>
#endif


#include "PngFile.h"

#include "matrix4x4.h"

#if defined(GL)
inline wchar_t * c2w(const char *str)
{
	size_t len = strlen(str) + 1;
	size_t converted = 0;
	wchar_t *wstr;
	wstr=(wchar_t*)malloc(len*sizeof(wchar_t));
	mbstowcs_s(&converted, wstr, len, str, _TRUNCATE);
	return wstr;
}
#endif

