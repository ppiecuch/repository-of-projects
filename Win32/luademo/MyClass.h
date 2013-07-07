#pragma once

#include <iostream>
#include <string>

class CMyClass
{
public:
	CMyClass(void){}
	~CMyClass(void){}

	static void staticGreet(int num)
	{
		std::cout << "static Hello World:" << num << "!" << std::endl;
	}

	void Greet()
	{
		std::cout << "Hello World!" << std::endl;
	}
};
