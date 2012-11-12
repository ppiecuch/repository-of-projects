#include<stdlib.h>
#include <stdio.h>

#include "yonArray.h"

using namespace yon;

#if 0

template<typename T>
class comparator{
public:
	int operator()(const T& t1,const T& t2) const{
		return t1-t2;
	}
};
struct Obj{
	int a;
};
class mycomparator{
public:
	int operator()(const Obj& t1,const Obj& t2) const{
		return t1.a-t2.a;
	}
};
template<typename T,typename TComparator=comparator<T> >
class Array{
public:
	T elements[2];

	T& operator[](int i){
		return elements[i];
	}
	const T& operator[](int i) const {
		return elements[i];
	}
	void sort(){
		if(TComparator()(elements[0],elements[1])>0)
		{
			T tmp=elements[1];
			elements[1]=elements[0];
			elements[0]=tmp;
			printf("sort\n");
		}
		else
			printf("no sort\n");
	}
};

int main(int argc,char ** argv)
{

	Array<int> arr;
	arr[0]=10;
	arr[1]=1;
	arr.sort();

	Array<Obj,mycomparator> arr2;
	arr2[0].a=10;
	arr2[1].a=20;
	arr2.sort();
	system("pause");
	return 0;
}
#else

struct Obj{
	Obj(int n):a(n){}
	int a;
};

struct mycomparator{
	int operator()(const Obj* o1,const Obj* o2){
		return o1->a-o2->a;
	}
};
int main(int argc,char ** argv)
{
	core::array<int> arr;
	arr.push_back(0);
	arr.push_back(3);
	arr.push_back(2);
	arr.push_back(1);
	arr.sort();
	for(u32 i=0;i<arr.size();++i)
		printf("%d\n",arr[i]);

	core::array<Obj*,mycomparator> arr2;
	arr2.push_back(new Obj(0));
	arr2.push_back(new Obj(3));
	arr2.push_back(new Obj(1));
	arr2.sort();

	for(u32 i=0;i<arr2.size();++i)
		printf("%d\n",arr2[i]->a);
	
	system("pause");
	return 0;
}
#endif