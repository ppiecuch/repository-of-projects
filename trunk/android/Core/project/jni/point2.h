#ifndef _POINT2_H_
#define _POINT2_H_

//#include <sstream>
#include <iostream>
using namespace std;
template <typename T> class Point2{
public:
	T x,y;
	//constructor
	Point2(){ x = 0; y = 0;}
	Point2(Point2 &p){ x=p.x; y=p.y;}
	Point2(T x,T y){ this->x = x; this->y = y;}
	//operator overload
	Point2& operator = (const Point2& p){x=p.x; y=p.y; return *this;}
	Point2 operator + (const Point2& p) const{return Point2(x+p.x,y+p.y);}
	Point2 operator - (const Point2& p) const{return Point2(x-p.x,y-p.y);}
	Point2 operator * (float scalar) const{return Point2(x*scalar,y*scalar);}
	Point2 operator / (float scalar) const{return Point2(x/scalar,y/scalar);}
	Point2 operator - () const{return Point2(-x,-y);}

	bool operator == (const Point2& p) const { return x==p.x && y==p.y; }
	bool operator != (const Point2& p) const { return !(*this==p); }
	Point2& operator += (const Point2& p) { x += p.x; y += p.y; return *this; }
	Point2& operator -= (const Point2& p) { x -= p.x; y -= p.y; return *this; }
	Point2& operator *= (float scalar) { x *= scalar; y *= scalar; return *this; }
	Point2& operator /= (float scalar) { x /= scalar; y /= scalar; return *this; }
	friend ostream& operator<<(ostream& out, const Point2& p){out<<'{'<<p.x<<','<<p.y<<'}';return out;}
	//指向const Point2对象的指针的const引用
	friend ostream& operator<<(ostream& out, const Point2* const &p){out<<'{'<<p->x<<','<<p->y<<'}';return out;}
	//string toString() const {stringstream ss; ss<<'{'; ss<<x; ss<<','; ss<<y; ss<<'}'; return ss.str();}
};
#endif
