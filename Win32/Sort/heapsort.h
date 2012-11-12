#ifndef _YON_CORE_HEAPSORT_H_
#define _YON_CORE_HEAPSORT_H_

#include "yonTypes.h"

namespace yon{
namespace core{

//最坏的情况下,时间复杂度O(nlogn)
//堆排序仅需要一个记录大小供交换用的辅助存储空间

//T需要支持赋值运算符和<关系运算符

//已知elements[s...e]中的元素除elements[s]外均满足堆的定义，
//本函数调用elements[s]的位置使elements[s...e]成为一个大顶堆
template<class T>
inline void heaprise(T* elements,u32 s,u32 e){
	/*
	for(u32 i=s<<1;i<=e;i<<=1){
		//比较左右孩子，选择较大的
		if(i<e&&elements[i]<elements[i+1])
			++i;
		//如果elements[s]大于先于所选的孩子，即满足堆的定义，不需排序
		if(array[s]<elements[i]==false)
			return;
		//否则，对换位置，s保持指向待排序的元素
		T t=elements[i];
		elements[i]=elements[s];
		elements[s]=t;
		s=i;
	}
	*/
	//优化
	T t=elements[s];
	for(u32 i=s<<1;i<=e;i<<=1){
		//比较左右孩子，选择较大的
		if(i<e&&elements[i]<elements[i+1])
			++i;
		//如果elements[s]大于先于所选的孩子，即满足堆的定义，不需排序
		if(t<elements[i]==false)
			break;
		//否则，对换位置，s保持指向待排序的元素
		elements[s]=elements[i];
		s=i;
	}
	elements[s]=t;
}

//对顺序表elements进行堆排序,调整为由小到大
template<class T>
inline void heapsort(T* elements,u32 size){
	/*
	u32 i;
	//将elements[1...size]建成大顶堆
	for(i=size/2;i>0;--i)
		heaprise(elements,i,size);
	//抽走堆顶元素，并用未抽过的堆尾元素替换
	for(i=size;i>1;--i){
		T t=elements[1];
		elements[1]=elements[i];
		elements[i]=t;
		//将elements[1...(i-1)]重新调整为大顶堆
		heaprise(elements,1,i-1);
	}*/

	if(size<=1)
		return;

	u32 i;
	T* virtualArray=elements-1;
	//将elements[0...size-1]建成大顶堆
	for(i=size/2;i>0;--i)
		heaprise(virtualArray,i,size);
	//抽走堆顶元素，并用未抽过的堆尾元素替换
	for(i=size;i>1;--i){
		T t=virtualArray[1];
		virtualArray[1]=virtualArray[i];
		virtualArray[i]=t;
		//将elements[0...(i-2)]重新调整为大顶堆
		heaprise(virtualArray,1,i-1);
	}
}
}
}
#endif