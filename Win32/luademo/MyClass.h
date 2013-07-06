#pragma once

#include <iostream>
#include <string>

class CMyClass
{
public:
	CMyClass(void){}
	~CMyClass(void){}

	static void staticGreet()
	{
		std::cout << "static Hello World!" << std::endl;
	}

	void Greet()
	{
		std::cout << "Hello World!" << std::endl;
	}
};
