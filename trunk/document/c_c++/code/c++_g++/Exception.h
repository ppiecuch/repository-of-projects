#include <string>

class Exception
{
protected:
	std::string msg;
public:
	Exception(){}
	Exception(const char* msg){this->msg=std::string(msg);}
	Exception(std::string msg){this->msg=msg;}
	std::string getMessage(){return msg;}
	virtual std::string getClassName()=0;
};