#include "stdafx.h"
extern "C" __declspec(dllexport) void testexam(char *input); 
extern "C" __declspec(dllexport) int* testnew(); 
struct HideStruct;
extern "C" class __declspec(dllexport) Test{
public:
	void test();
	void test(HideStruct& obj);
};