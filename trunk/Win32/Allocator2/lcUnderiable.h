#ifndef _LUCID_UNDERIABLE_H_
#define _LUCID_UNDERIABLE_H_

#include "lcDeclare.h"

namespace lc{

	//�ⲿ���÷���
	//class YouClass;
	//template<>
	//class Underiable<YouClass>{
	//private:
	//	Underiable(){};
	//	Underiable(const Underiable&) {}
	//	friend class YouClass;
	//};
	//ע�⣺
	//class YouClass : public Underiable<YouClass> ����������
	//class YouClass : public virtual Underiable<YouClass>{ ����

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