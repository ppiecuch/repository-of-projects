#include <stdio.h>
#include <stdlib.h>

#include "IProfile.h"
#include "CProfile.h"
#include "yonUtil.h"

namespace yon{
namespace debug{
	IProfile* Profile=new CProfile();
}
}

void test(){
	yon::core::yonSleep(100);
}
void abcdefghijklmn(){
	yon::core::yonSleep(10);
}
int demo(int a,char* c){
	yon::core::yonSleep(100);
	return 0;
}

class Test{
public:
	int abc(char* a){return 0;}
	static int bcd(int a){return 0;}
};

int main(int argc, char* argv[])
{
	//size_t addr1=reinterpret_cast<size_t>(&Test::bcd);
	//TODO 需要类对象指针
	//size_t addr2=reinterpret_cast<size_t>(&Test::abc);
	PROFILE_REGISTER_FRAME();
	PROFILE_START_CALL(PROFILE_ID_1,test);
	test();
	PROFILE_END_CALL(PROFILE_ID_1);
	PROFILE_START_CALL(PROFILE_ID_2,Test::bcd);
	test();
	PROFILE_END_CALL(PROFILE_ID_2);
	PROFILE_REGISTER_FRAME();

	PROFILE_REPORT();
	delete yon::debug::Profile;
	system("pause");
	return 0;
}