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


int main()
{
    App app;

	//´´½¨Î¯ÍÐ¡£
    App::Callback callback = NULL;
    if(!callback.IsNull()) callback("1");

    callback += NewDelegate(App::OutputToFile);
    if(!callback.IsNull()) callback("2");

    callback += NewDelegate(&app, &App::OutputToConsole);
    if(!callback.IsNull()) callback("3");

    callback -= NewDelegate(App::OutputToFile);
    if(!callback.IsNull()) callback("4");

    callback -= NewDelegate(&app, &App::OutputToConsole);
    if(!callback.IsNull()) callback("5");

	system("pause");
}