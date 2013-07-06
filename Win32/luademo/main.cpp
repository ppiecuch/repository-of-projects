#include "MyClass.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#pragma comment(lib, "../bin/lua-5.1.4_d.lib")
#pragma comment(lib, "../bin/tolua++_d.lib")

int main()
{
#if 1
	//创建一个lua解析器：
	lua_State *L; 
	L = lua_open();
	//调用lua库接口：
	luaL_openlibs(L);
	//至此，就可以使用lua相关的功能了。如：加载lua脚本(luaL_dofileL, luaFileName);等等
	luaL_dofile(L,"test1.lua");
	//在使用完lua后，一定要关掉lua解析器。l
	lua_close(L);

#else
	CMyClass c;
	c.Greet();
#endif

	system("pause");
	return 0;
}