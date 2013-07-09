#include <stdio.h>
#include <stdlib.h>

typedef char (&yes)[1];		//��������yes��һ��Ԫ������Ϊchar����СΪ1�����������
typedef char (&no)[2];		//��������no��һ��Ԫ������Ϊchar����СΪ2�����������

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

	myException operator()(const char* exception, const char* file, const char* func, u32 line)//�˴������ز�����()
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
//�����쳣ʱϵͳ�Ĵ���˳��(by Jeremy Gordon, Hume): 

//1.ϵͳ�����ж��쳣�Ƿ�Ӧ���͸�Ŀ�������쳣��������,�������Ӧ�÷���,����Ŀ��������ڱ�����,��ϵͳ 
//������������������EXCEPTION_DEBUG_EVENT��Ϣ.

//2.�����ĳ���û�б����Ի��ߵ�����δ�ܴ����쳣,ϵͳ�ͻ�����������Ƿ�װ���߳���ص��쳣��������,��� 
//�㰲װ���߳���ص��쳣��������,ϵͳ�Ͱ��쳣���͸���ĳ���seh��������,�����䴦��. 

//3.ÿ���߳���ص��쳣�������̿��Դ�����߲���������쳣,������������Ұ�װ�˶���߳���ص��쳣��������, 
//�ɽ������������������̴���. 

//4.�����Щ���̾�ѡ�񲻴����쳣,��������ڱ�����״̬,����ϵͳ�Ի��ٴι������֪ͨdebugger. 

//5.�������δ���ڱ�����״̬����debuggerû���ܹ�����,���������SetUnhandledExceptionFilter��װ������� 
//���������̵Ļ�,ϵͳת������ĵ���. 

//6.�����û�а�װ����쳣�������̻�����û�д�������쳣,ϵͳ�����Ĭ�ϵ�ϵͳ�������,ͨ����ʾһ���Ի���, 
//�����ѡ��رջ�������丽�ӵ��������ϵĵ��԰�ť.���û�е������ܱ����������ϻ��ߵ�����Ҳ������,ϵͳ 
//�͵���ExitProcess�ս����. 

//7.�������ս�֮ǰ,ϵͳ��Ȼ�Է����쳣���߳��쳣��������һ��չ��,�����߳��쳣���������������Ļ���. 

//��������ķ���ֵ�����������
//EXCEPTION_EXECUTE_HANDLER equ 1 ��ʾ���Ѿ��������쳣,�������ŵؽ�����  
//EXCEPTION_CONTINUE_SEARCH equ 0 ��ʾ�Ҳ�����,����������,����windows����Ĭ�ϵĴ��������ʾһ�������,������  
//EXCEPTION_CONTINUE_EXECUTION equ -1 ��ʾ�����Ѿ����޸�,����쳣����������ִ�� 

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