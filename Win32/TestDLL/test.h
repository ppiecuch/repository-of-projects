#include "stdafx.h"
extern "C" __declspec(dllimport) void testexam(char *input); 
extern "C" __declspec(dllimport) int* testnew(); 
struct HideStruct;
extern "C" class __declspec(dllimport) Test{
public:
	void test();
	void test(HideStruct& obj);
};