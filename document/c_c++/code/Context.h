#pragma once

class Context
{
private:
	Context();
	Context(const Context&);
	Context& operator = (const Context&);
public:
	static Context& GetInstance();

	CString str;
};
