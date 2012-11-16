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
	yon::core::yonSleep(10);
}

int main(int argc, char* argv[])
{
	PROFILE_REGISTER_FRAME();
	PROFILE_START_CALL(test);
	test();
	PROFILE_END_CALL(test);
	PROFILE_START_CALL(test);
	test();
	PROFILE_END_CALL(test);
	PROFILE_REGISTER_FRAME();

	PROFILE_REPORT();
	delete yon::debug::Profile;
	system("pause");
	return 0;
}