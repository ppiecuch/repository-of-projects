#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "delegate.h"

//refer to:http://rsdn.ru/article/cpp/delegates.xml


class App
{
public:
    typedef CDelegate1<void, string> Callback;

    void OutputToConsole(string str)
    {
        cout << str << endl;
    }

    static void OutputToFile(string str)
    {
        ofstream fout("output.txt", ios::out | ios::app);
        fout << str << endl;
        fout.close();
    }
};

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}


int main()
{
	EnableMemLeakCheck();

    App app;

	//创建委托。
    App::Callback callback = NULL;
    if(!callback.IsNull()) callback("1");

	//如此用有内存泄漏
	/*
    callback += NewDelegate(App::OutputToFile);
    if(!callback.IsNull()) callback("2");

    callback += NewDelegate(&app, &App::OutputToConsole);
    if(!callback.IsNull()) callback("3");

    callback -= NewDelegate(App::OutputToFile);
    if(!callback.IsNull()) callback("4");

    callback -= NewDelegate(&app, &App::OutputToConsole);
    if(!callback.IsNull()) callback("5");
	*/
	

	
	//这样才不会
	IDelegate1<void,string>* d0=NewDelegate(App::OutputToFile);
	IDelegate1<void,string>* d1=NewDelegate(&app, &App::OutputToConsole);

	callback += d0;
	if(!callback.IsNull()) callback("2");

	callback += d1;
	if(!callback.IsNull()) callback("3");

	callback -= d0;
	if(!callback.IsNull()) callback("4");

	callback -= d1;
	if(!callback.IsNull()) callback("5");

	system("pause");
}