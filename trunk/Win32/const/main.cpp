
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

void spc(const char const *spc) //warning C4114: ���ʹ��ͬһ�����޶���
{
	spc="hello";
	spc++;
	printf("sc:%s\r\n",spc);
}

/*void sc(char const *sc)
{
	sc="hello";
	sc++;
	printf("sc:%s\r\n",sc);
}*/
void sc(char* const sc)//ָ��char* ��constָ��
{
	//sc="hello";
	//sc++;
	printf("sc:%s\r\n",sc);
}
void pc(const char* pc)//ָ��const char* �����ָ��
{
	pc="hello";
	pc++;
	printf("pc:%s\r\n",pc);
}
int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	sc("char const");
	pc("const char");

	system("pause");
	return 0;
}