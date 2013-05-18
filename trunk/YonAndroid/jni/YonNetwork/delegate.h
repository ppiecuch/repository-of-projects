// delegate.h: 委托类
// Author: Guo Huafeng
// NOTE: 只实现了1个参数
// 实现细节参考MyGUI的IDelegate
#ifndef _DELEGATE_H_
#define _DELEGATE_H_


//IDelegate接口
template <typename TP1>
class IDelegate
{
	public:
		virtual ~IDelegate() { }
		virtual void invoke( TP1 p1 ) = 0;
};

class IDelegateVoid
{
public:
	virtual ~IDelegateVoid() { }
	virtual void invoke() = 0;
};

//类成员函数委托
template <typename T, typename TP1>
class CMethodDelegate : public IDelegate<TP1>
{
public:
	typedef void (T::*Method)( TP1 p1 );

	CMethodDelegate(T* _object, Method _method)
		: mObject(_object)
		, mMethod(_method)
	{
	}

	virtual void invoke( TP1 p1 )
	{
		(mObject->*mMethod)( p1 );	//调用对象方法
	}
private:
	T* mObject;		//对象指针
	Method mMethod;	//对象方法
};

template <typename T>
class CMethodDelegateVoid : public IDelegateVoid
{
public:
	typedef void (T::*Method)();

	CMethodDelegateVoid(T* _object, Method _method)
		: mObject(_object)
		, mMethod(_method)
	{
	}

	virtual void invoke()
	{
		(mObject->*mMethod)();	//调用对象方法
	}
private:
	T* mObject;		//对象指针
	Method mMethod;	//对象方法
};

// 创建委托实用函数
template <typename T, typename TP1>
inline  IDelegate <TP1>* createDelegate( T* _object, void (T::*_method)( TP1 p1 ) )
{
	return new CMethodDelegate<T, TP1> (_object, _method);
}

template <typename T>
inline  IDelegateVoid* createDelegateVoid( T* _object, void (T::*_method)() )
{
	return new CMethodDelegateVoid<T> (_object, _method);
}

// 暴露给客户使用的委托类
template  <typename TP1>
class CDelegate
{
public:
	CDelegate () : mDelegate(0) { }
	~CDelegate ()
	{
		clear();
	}

	bool empty() const
	{
		return mDelegate == 0;
	}

	void clear()
	{
		if (mDelegate)
		{
			delete mDelegate;
			mDelegate = 0;
		}
	}

	//重载=
	CDelegate<TP1>& operator=(IDelegate<TP1>* _delegate)
	{
		delete mDelegate;
		mDelegate = _delegate;
		return *this;
	}

	//重载()
	void operator()( TP1 p1 )
	{
		if (mDelegate == 0) return;
		mDelegate->invoke( p1 );
	}

private:
	IDelegate<TP1>* mDelegate;		//委托

};

class CDelegateVoid
{
public:
	CDelegateVoid () : mDelegate(0) { }
	~CDelegateVoid ()
	{
		clear();
	}

	bool empty() const
	{
		return mDelegate == 0;
	}

	void clear()
	{
		if (mDelegate)
		{
			delete mDelegate;
			mDelegate = 0;
		}
	}

	//重载=
	CDelegateVoid& operator=(IDelegateVoid* _delegate)
	{
		delete mDelegate;
		mDelegate = _delegate;
		return *this;
	}

	//重载()
	void operator()()
	{
		if (mDelegate == 0) return;
		mDelegate->invoke();
	}

private:
	IDelegateVoid* mDelegate;		//委托

};


#endif