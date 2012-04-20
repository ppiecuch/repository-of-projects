// virtualInDerived.cpp : 定义控制台应用程序的入口点。
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
	/* virtual 函数系动态绑定（dynamically bound），而缺省参数值却是静态绑定（statically bound）*/
	/* 绝对不要重新定义一个继承而来的缺省参数值，因为缺省参数值都是静态绑定而virtual函数——你唯*/
	/* 一应该覆写的东西——却是动态绑定                                                         */
	/********************************************************************************************/

	pc->draw();//Circle draw:0
	pr->draw();//Rectangle draw:0
	getchar();
	return 0;
}

