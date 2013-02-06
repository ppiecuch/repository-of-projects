
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

struct SUCLogin{
	char str[255];
};
struct SUCPayment{
	int orderId;
};

struct SUnionEvent{
	int msg;
	union{
		SUCLogin* pUCLogin;
		SUCPayment* pUCPayment;
	};
};

struct SCustomEvent{
	int type;
	union{
		SUnionEvent unionEvent;
	};
};

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	SCustomEvent evt;
	evt.unionEvent.pUCLogin=new SUCLogin();


	system("pause");
	return 0;
}