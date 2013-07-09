#include <stdio.h>
#include <stdlib.h>

typedef char (&yes)[1];		//声明符号yes是一个元素类型为char，大小为1的数组的引用
typedef char (&no)[2];		//声明符号no是一个元素类型为char，大小为2的数组的引用

template <typename B, typename D>
struct Host
{
	operator B*() const;
	operator D*();
};

template <typename B, typename D>
struct is_base_of
{
	template <typename T> 
	static yes check(D*, T);
	static no check(B*, int);

	static const bool value = sizeof(check(Host<B,D>(), int())) == sizeof(yes);
};

#define NO_THROW_EXCEPTION

#ifdef NO_THROW_EXCEPTION

void mySubstituteThrow(const char* e,const char* file,const char* func,int line){
	printf("encounter %s at \"%s\" function in %s,%d\r\n",e,func,file,line);
}
#define myTry if (true)
#define myThrow(e) mySubstituteThrow(#e, __FILE__,__FUNCTION__, __LINE__)
#define myCatch(...) if(false)
#define myMesg(e) #e
#define myFile(e) #e
#define myFunc(e) #e
#define myLine(e) #e


struct MyException{};

#else

//refer to:https://groups.google.com/forum/#!topic/comp.lang.c++/O502L-ZTbBs
//refer to:http://bytes.com/topic/c/answers/217236-life-time-string-referenced-__file__-preprocessor-statement
//__FILE__ has file scope. It's lifetime is the current translation
//unit. It's usually implemented as a preprocessor macro.

//__func__ is the name of the current function in the TU, it has
//function scope. The function-local predefined variable __func__ is
//defined as if a definition of the form

//static const char __func__[] = "function-name ";
//had been provided at the opening brace of the function.

typedef unsigned int u32;
class myException
{
public:
	/*!
	Constructs an exception.
	@param[in] message The name of an exception.
	@param[in] file The file name in where an exception occurred.
	@param[in] line The line number at where an exception occurred.
	*/
	myException(const char* message, const char* file, const char* func, u32 line)
	{
		m_exception = message;
		m_file = file;
		m_func = func;
		m_line = line;
	}

	/*!
	Returns the name of this exception.
	@return The name of this exception.
	*/
	const char* getMessage() const
	{
		return m_exception;
	}

	/*!
	Returns the file name in where this exception occurred.
	@return The file name in where this exception occurred.
	*/
	const char* getFile() const
	{
		return m_file;
	}

	/*!
	Returns the function name in where this exception occurred.
	@return The function name in where this exception occurred.
	*/
	const char* getFunc() const
	{
		return m_func;
	}

	/*!
	Returns the line number at where this exception occurred.
	@return the line number at where this exception occurred.
	*/
	u32 getLine() const
	{
		return m_line;
	}

	myException operator()(const char* exception, const char* file, const char* func, u32 line)//此处是重载操作符()
	{
		return myException(exception,file,func,line);
	}
	
private:
	const char* m_exception;
	const char* m_file;
	const char* m_func;
	u32 m_line;
};

/*!
@ingroup ckDef
Defines an exception class.
@param[in] e The name of an exception class.
*/
#define myDefineException(e) \
	class e : public myException \
	{ \
	public: \
		e(const char* exception, const char* file, const char* func, u32 line) : myException(exception, file, func, line) {} \
		\
		e operator()(const char* exception, const char* file, const char* func, u32 line) \
		{ \
			return e(exception,file,func,line); \
		} \
	}

myDefineException(MyException);
myDefineException(MyException2);

/*!
Tries to catch an exception in the following block.
*/
#define myTry try


/*!
Throws an exception.
@param[in] e The name of an exception class.
*/
#define myThrow(e) throw e(#e, __FILE__, __FUNCTION__, __LINE__)


/*!
Catches the specified exception.
@param[in] ... An exception class and its argument.
*/
#define myCatch(...) catch (__VA_ARGS__)

#define myMesg(e) e.getMessage()
#define myFile(e) e.getFile()
#define myFunc(e) e.getFunc()
#define myLine(e) e.getLine()

#endif

#include <windows.h>
#include <iostream>  
#include <psapi.h>  

//refer to:http://win32easy.blogspot.com/2011/03/exception-handling-inform-your-users_26.html

#pragma comment(lib, "psapi.lib")  


//refer to:http://msdn.microsoft.com/zh-cn/library/7e3a913x(v=vs.80).aspx
//Stringizing Operator (#)
//The number-sign or "stringizing" operator (#) converts macro parameters to string literals without expanding the parameter definition. 
//It is used only with macros that take arguments. If it precedes a formal parameter in the macro definition, 
//the actual argument passed by the macro invocation is enclosed in quotation marks and treated as a string literal. 
//The string literal then replaces each occurrence of a combination of the stringizing operator and formal parameter within the macro definition.

// Definition of function GetExceptionName....  
#define EX_CASE(code) \
	case code: \
	return #code;  

LPCSTR GetExceptionName(DWORD code)  
{  
	 switch(code)  
	 {
		 EX_CASE(EXCEPTION_ACCESS_VIOLATION);  
		 EX_CASE(EXCEPTION_DATATYPE_MISALIGNMENT);  
		 EX_CASE(EXCEPTION_BREAKPOINT);  
		 EX_CASE(EXCEPTION_SINGLE_STEP);  
		 EX_CASE(EXCEPTION_ARRAY_BOUNDS_EXCEEDED);  
		 EX_CASE(EXCEPTION_FLT_DENORMAL_OPERAND);  
		 EX_CASE(EXCEPTION_FLT_DIVIDE_BY_ZERO);  
		 EX_CASE(EXCEPTION_FLT_INEXACT_RESULT);  
		 EX_CASE(EXCEPTION_FLT_INVALID_OPERATION);  
		 EX_CASE(EXCEPTION_FLT_OVERFLOW);  
		 EX_CASE(EXCEPTION_FLT_STACK_CHECK);  
		 EX_CASE(EXCEPTION_FLT_UNDERFLOW);  
		 EX_CASE(EXCEPTION_INT_DIVIDE_BY_ZERO);  
		 EX_CASE(EXCEPTION_INT_OVERFLOW);  
		 EX_CASE(EXCEPTION_PRIV_INSTRUCTION);  
		 EX_CASE(EXCEPTION_IN_PAGE_ERROR);  
		 EX_CASE(EXCEPTION_ILLEGAL_INSTRUCTION);  
		 EX_CASE(EXCEPTION_NONCONTINUABLE_EXCEPTION);  
		 EX_CASE(EXCEPTION_STACK_OVERFLOW);  
		 EX_CASE(EXCEPTION_INVALID_DISPOSITION);  
		 EX_CASE(EXCEPTION_GUARD_PAGE);  
		 EX_CASE(EXCEPTION_INVALID_HANDLE);  

		 //refer to:http://support.microsoft.com/default.aspx?scid=kb;en-us;Q185294
		 //SYMPTOMS
		 //When you call Win32 Structured Exception Handling (SEH) APIs, such as GetExceptionCode() and GetExceptionInformation(), sometimes the system reports the following exception code:
		 //0xE06D7363
		 //Sometimes GetLastError() also returns this value. This exception code will be used for any error that is raised by the Microsoft Visual C++ compiler through a call to "throw".

		 //CAUSE
		 //All Visual C++ exceptions thrown from code generated by the Microsoft Visual C++ compiler contain this error code. Because this is a compiler-generated error, 
		 //the code is not listed in the Win32 API header files. The code is actually a cryptic mnemonic device, with the initial "E" standing for "exception" and the final 
		 //3 bytes (0x6D7363) representing the ASCII values of "msc".

		 //RESOLUTION
		 //.....
	 case 0xE06D7363:
		 return "C++ Exception";  

	 default:
		 return "Unknown exception";  
	 }
}

#undef EX_CASE  

HMODULE GetExceptionModule(LPVOID address, LPSTR moduleName /* must support MAX_PATH characters */)  
{  
	HMODULE moduleList[1024];  
	DWORD sizeNeeded = 0;  
	if(FALSE == EnumProcessModules(GetCurrentProcess(), moduleList, 1024, &sizeNeeded) || sizeNeeded < sizeof(HMODULE))  
		return NULL;  

	int curModule = -1;  
	for(unsigned int i = 0; i < (sizeNeeded / sizeof(HMODULE)); ++i)  
	{  
		if((DWORD)moduleList[i] < (DWORD)address)  
		{  
			if(curModule == -1)  
				curModule = i;  
			else  
			{  
				if((DWORD)moduleList[curModule] < (DWORD)moduleList[i])  
					curModule = i;  
			}  
		}  
	}  

	if(curModule == -1)  
		return NULL;  

	if(!GetModuleFileNameA(moduleList[curModule], moduleName, MAX_PATH))  
		return NULL;  

	return moduleList[curModule];  
}  
//发生异常时系统的处理顺序(by Jeremy Gordon, Hume): 

//1.系统首先判断异常是否应发送给目标程序的异常处理例程,如果决定应该发送,并且目标程序正在被调试,则系统 
//挂起程序并向调试器发送EXCEPTION_DEBUG_EVENT消息.

//2.如果你的程序没有被调试或者调试器未能处理异常,系统就会继续查找你是否安装了线程相关的异常处理例程,如果 
//你安装了线程相关的异常处理例程,系统就把异常发送给你的程序seh处理例程,交由其处理. 

//3.每个线程相关的异常处理例程可以处理或者不处理这个异常,如果他不处理并且安装了多个线程相关的异常处理例程, 
//可交由链起来的其他例程处理. 

//4.如果这些例程均选择不处理异常,如果程序处于被调试状态,操作系统仍会再次挂起程序通知debugger. 

//5.如果程序未处于被调试状态或者debugger没有能够处理,并且你调用SetUnhandledExceptionFilter安装了最后异 
//常处理例程的话,系统转向对它的调用. 

//6.如果你没有安装最后异常处理例程或者他没有处理这个异常,系统会调用默认的系统处理程序,通常显示一个对话框, 
//你可以选择关闭或者最后将其附加到调试器上的调试按钮.如果没有调试器能被附加于其上或者调试器也处理不了,系统 
//就调用ExitProcess终结程序. 

//7.不过在终结之前,系统仍然对发生异常的线程异常处理句柄来一次展开,这是线程异常处理例程最后清理的机会. 

//这个函数的返回值有三种情况：
//EXCEPTION_EXECUTE_HANDLER equ 1 表示我已经处理了异常,可以优雅地结束了  
//EXCEPTION_CONTINUE_SEARCH equ 0 表示我不处理,其他人来吧,于是windows调用默认的处理程序显示一个错误框,并结束  
//EXCEPTION_CONTINUE_EXECUTION equ -1 表示错误已经被修复,请从异常发生处继续执行 

long WINAPI onUnhandledException(EXCEPTION_POINTERS* exceptionInfo){
	char szFileName[_MAX_PATH];
	::GetModuleFileNameA(NULL, szFileName, _MAX_PATH);

	char message[MAX_PATH + 255];  
	char module[MAX_PATH];  
	char* moduleName = NULL;  
	if(GetExceptionModule(exceptionInfo->ExceptionRecord->ExceptionAddress, module))  
		moduleName = module;  
	else  
		moduleName = "Unknown module!";  

	sprintf_s(message,   
		"An exception has occured which was not handled!\nException: %s\nExe: %s\nCode: %s\nModule: %s",
		szFileName,
		GetExceptionName(exceptionInfo->ExceptionRecord->ExceptionCode),  
		moduleName  
		);  

	MessageBoxA(0, message, "Error!", MB_OK); 
	return EXCEPTION_EXECUTE_HANDLER;
}


void test1(){
	printf("test1\r\n");
}
void test2(){
	printf("test2\r\n");
	myThrow(MyException);
}

class Base{};
class Derive : public Base{};
class Super : public Derive{};


int main(int argc, char* argv[])
{
	SetUnhandledExceptionFilter(onUnhandledException);

	bool r1=is_base_of<Base,Derive>::value;
	bool r2=is_base_of<Base,Super>::value;
	bool r3=is_base_of<Base,Base>::value;
	printf("%d,%d,%d\r\n",r1,r2,r3);

	myTry{
		test1();
	}myCatch(...){
		test2();
	}

	myTry{
		test2();
	}myCatch(MyException e){
		printf("%s,%s,%s,%d\r\n",myMesg(e),myFile(e),myFunc(e),myLine(e));
		//TODO
		myThrow(e);
		//myThrow(MyException);
	}
	system("pause");
	return 0;
} 