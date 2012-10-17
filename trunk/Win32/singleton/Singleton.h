#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template <class T>
class Singleton
{
public:
	static T& getInstance()
	{
		static T instance;
		return instance;
	};
private:
	Singleton(void){};
	~Singleton(void){};
	Singleton(const Singleton&){};
	Singleton & operator= (const Singleton &){};
};

#define DECLARE_SINGLETON_CLASS(type) \
	friend class Singleton<type>;
#endif