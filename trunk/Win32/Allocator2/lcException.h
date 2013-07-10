#ifndef _LUCID_EXCEPTION_H_
#define _LUCID_EXCEPTION_H_

#include "lcTypes.h"

namespace lc{

class Exception
{
public:
	/*!
	Constructs an exception.
	@param[in] message The name of an exception.
	@param[in] file The file name in where an exception occurred.
	@param[in] line The line number at where an exception occurred.
	*/
	Exception(const c8* message, const c8* file, const c8* func, s32 line)
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
	const c8* getMessage() const
	{
		return m_exception;
	}

	/*!
	Returns the file name in where this exception occurred.
	@return The file name in where this exception occurred.
	*/
	const c8* getFile() const
	{
		return m_file;
	}

	/*!
	Returns the function name in where this exception occurred.
	@return The function name in where this exception occurred.
	*/
	const c8* getFunc() const
	{
		return m_func;
	}

	/*!
	Returns the line number at where this exception occurred.
	@return the line number at where this exception occurred.
	*/
	s32 getLine() const
	{
		return m_line;
	}

	Exception operator()(const c8* exception, const c8* file, const c8* func, s32 line)
	{
		return Exception(exception,file,func,line);
	}

private:
	const c8* m_exception;
	const c8* m_file;
	const c8* m_func;
	s32 m_line;
};

/*!
Defines an exception class.
@param[in] e The name of an exception class.
*/
#define lcDefineException(e) \
class e : public lc::Exception \
{ \
public: \
	e(const c8* exception, const c8* file, const c8* func, s32 line) : Exception(exception, file, func, line) {} \
	e operator()(const c8* exception, const c8* file, const c8* func, s32 line) \
	{ \
		return e(exception,file,func,line); \
	} \
}

}

#ifdef LC_WITH_EXCEPTION

#define lcTry try
#define lcThrow(e) throw e(#e, LC_FILE,LC_FUNC,LC_LINE)
#define lcCatch(...) catch (__VA_ARGS__)
#define lcGetMesg(e) e.getMessage()
#define lcGetFile(e) e.getFile()
#define lcGetFunc(e) e.getFunc()
#define lcGetLine(e) e.getLine()

#else

#define lcTry if(true)
#define lcThrow(e) printf("encounter %s at \"%s\" function in %s,%d\r\n",#e,LC_FILE,LC_FUNC,LC_LINE)
#define lcCatch(...) if(false)
#define lcGetMesg(e) 0
#define lcGetFile(e) 0
#define lcGetFunc(e) 0
#define lcGetLine(e) 0

#endif


#endif