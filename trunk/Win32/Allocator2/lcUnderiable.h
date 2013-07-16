#ifndef _LUCID_UNDERIABLE_H_
#define _LUCID_UNDERIABLE_H_

#include "lcDeclare.h"

namespace lc{

	//外部类用法：
	//class YouClass;
	//template<>
	//class Underiable<YouClass>{
	//private:
	//	Underiable(){};
	//	Underiable(const Underiable&) {}
	//	friend class YouClass;
	//};
	//注意：
	//class YouClass : public Underiable<YouClass> 则起不了作用
	//class YouClass : public virtual Underiable<YouClass>{ 可以

	template<class T>
	class Underiable{
	public:
		~Underiable(){}
	private:
		Underiable(){};
		Underiable(const Underiable&) {}

		friend class timer;
	};
}
#endif