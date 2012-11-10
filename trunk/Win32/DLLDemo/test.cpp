#include "stdafx.h"
#include "test.h"
#include <iostream>

struct HideStruct{
	int a;
};

void Test::test(){
	HideStruct obj;
	obj.a=10;
	std::cout<<obj.a<<std::endl;
}

void Test::test(HideStruct& obj){
	std::cout<<obj.a<<std::endl;
}

void testexam(char *input)
{
	std::cout<<"test"<<std::endl;
}
int* testnew()
{
	return new int;
}