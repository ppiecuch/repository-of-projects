// virtualInDerived.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

class Shape{
public:
	enum ShapeColor{Red,Green,Blue};
	virtual void draw(ShapeColor color=Red) const=0;
};

class Rectangle:public Shape{
public:
	virtual void draw(ShapeColor color = Green) const{
		printf("Rectangle draw:%d\n",color);
	}
};

class Circle:public Shape{
public:
	virtual void draw(ShapeColor color) const{
		printf("Circle draw:%d\n",color);
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	Shape* ps;
	Shape* pc=new Circle;
	Shape* pr=new Rectangle;

	/********************************************************************************************/
	/* virtual ����ϵ��̬�󶨣�dynamically bound������ȱʡ����ֵȴ�Ǿ�̬�󶨣�statically bound��*/
	/* ���Բ�Ҫ���¶���һ���̳ж�����ȱʡ����ֵ����Ϊȱʡ����ֵ���Ǿ�̬�󶨶�virtual����������Ψ*/
	/* һӦ�ø�д�Ķ�������ȴ�Ƕ�̬��                                                         */
	/********************************************************************************************/

	pc->draw();//Circle draw:0
	pr->draw();//Rectangle draw:0
	getchar();
	return 0;
}

