#include <stdio.h>
#include <stdlib.h>

//#define NO_THROW_EXCEPTION

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

private:
	const char* m_exception;
	const char* m_file;
	const char* m_func;
	u32 m_line;
};

typedef char (&yes)[1];
typedef char (&no)[2];

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


template<class T>
bool isClass(){
	return is_base_of<myException,T>();
}

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
	}

myDefineException(MyException);

/*!
Tries to catch an exception in the following block.
*/
#define myTry try


/*!
Throws an exception.
@param[in] e The name of an exception class.
*/
//#define myThrow(e) do{if(isClass<e>())throw e(#e, __FILE__, __FUNCTION__, __LINE__); else throw e;}while(0)
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


void test1(){
	printf("test1\r\n");
}
void test2(){
	printf("test2\r\n");
	myThrow(MyException);
}

int main(int argc, char* argv[])
{
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
		//myThrow(e);
		myThrow(MyException);
	}
	system("pause");
	return 0;
}