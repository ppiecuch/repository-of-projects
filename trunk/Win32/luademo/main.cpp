#include "MyClass.h"
#include "tolua++.h"
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#pragma comment(lib, "../bin/lua-5.1.4_d.lib")
#pragma comment(lib, "../bin/tolua++_d.lib")

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

 int  tolua_test_open (lua_State* tolua_S);

int main()
{
	EnableMemLeakCheck();
#if 1
	//����һ��lua��������
	lua_State *L; 
	L = lua_open();
	//����lua��ӿڣ�
	luaL_openlibs(L);
	//�����ǵ�ע�ắ��
	tolua_test_open(L); // ��mylib
	//���ˣ��Ϳ���ʹ��lua��صĹ����ˡ��磺����lua�ű�(luaL_dofileL, luaFileName);�ȵ�
	luaL_dofile(L,"base04.lua");
	//��ʹ����lua��һ��Ҫ�ص�lua��������l
	lua_close(L);

#else
	CMyClass c;
	c.Greet();
#endif

	system("pause");
	return 0;
}