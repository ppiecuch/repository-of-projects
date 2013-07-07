/*
** Lua binding: test
** Generated automatically by tolua++-1.0.92 on 07/06/13 17:38:44.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_test_open (lua_State* tolua_S);

#include "MyClass.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_CMyClass (lua_State* tolua_S)
{
 CMyClass* self = (CMyClass*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CMyClass");
}

/* method: new of class  CMyClass */
#ifndef TOLUA_DISABLE_tolua_test_CMyClass_new00
static int tolua_test_CMyClass_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CMyClass",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CMyClass* tolua_ret = (CMyClass*)  Mtolua_new((CMyClass)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CMyClass");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CMyClass */
#ifndef TOLUA_DISABLE_tolua_test_CMyClass_new00_local
static int tolua_test_CMyClass_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CMyClass",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CMyClass* tolua_ret = (CMyClass*)  Mtolua_new((CMyClass)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CMyClass");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CMyClass */
#ifndef TOLUA_DISABLE_tolua_test_CMyClass_delete00
static int tolua_test_CMyClass_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMyClass",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMyClass* self = (CMyClass*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: staticGreet of class  CMyClass */
#ifndef TOLUA_DISABLE_tolua_test_CMyClass_staticGreet00
static int tolua_test_CMyClass_staticGreet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CMyClass",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   CMyClass::staticGreet(num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'staticGreet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Greet of class  CMyClass */
#ifndef TOLUA_DISABLE_tolua_test_CMyClass_Greet00
static int tolua_test_CMyClass_Greet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMyClass",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMyClass* self = (CMyClass*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Greet'", NULL);
#endif
  {
   self->Greet();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Greet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_test_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CMyClass","CMyClass","",tolua_collect_CMyClass);
  #else
  tolua_cclass(tolua_S,"CMyClass","CMyClass","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CMyClass");
   tolua_function(tolua_S,"new",tolua_test_CMyClass_new00);
   tolua_function(tolua_S,"new_local",tolua_test_CMyClass_new00_local);
   tolua_function(tolua_S,".call",tolua_test_CMyClass_new00_local);
   tolua_function(tolua_S,"delete",tolua_test_CMyClass_delete00);
   tolua_function(tolua_S,"staticGreet",tolua_test_CMyClass_staticGreet00);
   tolua_function(tolua_S,"Greet",tolua_test_CMyClass_Greet00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_test (lua_State* tolua_S) {
 return tolua_test_open(tolua_S);
};
#endif

