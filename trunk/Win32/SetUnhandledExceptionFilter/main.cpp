#include <stdio.h>
#include <windows.h>

//用CMD运行可以看到
//SetUnhandledExceptionFilter 拿到DEBUG的权限是小于VS的 必须要直接运行 才能Dump文件出来
//在调试器里面的话，异常总是会被调试器拿到。

#if 1

long WINAPI handler(EXCEPTION_POINTERS* lpExceptionInfo)
{
	printf("%d",5);
	return EXCEPTION_EXECUTE_HANDLER;	  //print:5(exit)
	//return EXCEPTION_CONTINUE_EXECUTION;  //print:55555555555....
	//return EXCEPTION_CONTINUE_SEARCH;
}
int main(int argc,char* argv[])
{
	printf("Going to assign value to null pointer!\n");

	SetUnhandledExceptionFilter(handler);

	*(int*)0 = 1;

	printf("Going to assign value to null pointer2!\n");

	return 0;
}

#else

class ExceptionHandler
{
private:
	class SingleTonHandler
	{
	public:
		/*static void handler()
		{
			// Exception from construction/destruction of global variables
			try
			{
				// re-throw
				throw;
			}
			catch (SegmentationFault &)
			{
				cout << "SegmentationFault" << endl;
			}
			catch (FloatingPointException &)
			{
				cout << "FloatingPointException" << endl;
			}
			catch (...)
			{
				cout << "Unknown Exception" << endl;
			}

			//if this is a thread performing some core activity
			abort();
			// else if this is a thread used to service requests
			// pthread_exit();
		}*/
		static long WINAPI handler(EXCEPTION_POINTERS* lpExceptionInfo)
		{
			printf("%d",5);
			return EXCEPTION_EXECUTE_HANDLER;
		}

		SingleTonHandler()
		{
			//set_terminate(handler);
			SetUnhandledExceptionFilter(handler);
		}
	};

public:
	ExceptionHandler()
	{
		static SingleTonHandler s_objHandler;
	}
};

//////////////////////////////////////////////////////////////////////////

class A
{
public:
	A()
	{
		//int i = 0, j = 1/i;
		*(int *)0 = 0;
	}
};

// Before defining any global variable, we define a dummy instance
// of ExceptionHandler object to make sure that
// ExceptionHandler::SingleTonHandler::SingleTonHandler() is invoked
ExceptionHandler g_objExceptionHandler;
A g_a;

//////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	return 0;
}
#endif