#include "MyClass.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#pragma comment(lib, "../bin/lua-5.1.4_d.lib")
#pragma comment(lib, "../bin/tolua++_d.lib")

int main()
{
#if 1
	//����һ��lua��������
	lua_State *L; 
	L = lua_open();
	//����lua��ӿڣ�
	luaL_openlibs(L);
	//���ˣ��Ϳ���ʹ��lua��صĹ����ˡ��磺����lua�ű�(luaL_dofileL, luaFileName);�ȵ�
	luaL_dofile(L,"test1.lua");
	//��ʹ����lua��һ��Ҫ�ص�lua��������l
	lua_close(L);

#else
	CMyClass c;
	c.Greet();
#endif

	system("pause");
	return 0;
}