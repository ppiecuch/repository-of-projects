#ifndef _YON_CORE_HEAPSORT_H_
#define _YON_CORE_HEAPSORT_H_

#include "yonTypes.h"

namespace yon{
namespace core{

//��������,ʱ�临�Ӷ�O(nlogn)
//���������Ҫһ����¼��С�������õĸ����洢�ռ�

//T��Ҫ֧�ָ�ֵ�������<��ϵ�����

//��֪elements[s...e]�е�Ԫ�س�elements[s]�������ѵĶ��壬
//����������elements[s]��λ��ʹelements[s...e]��Ϊһ���󶥶�
template<class T>
inline void heaprise(T* elements,u32 s,u32 e){
	/*
	for(u32 i=s<<1;i<=e;i<<=1){
		//�Ƚ����Һ��ӣ�ѡ��ϴ��
		if(i<e&&elements[i]<elements[i+1])
			++i;
		//���elements[s]����������ѡ�ĺ��ӣ�������ѵĶ��壬��������
		if(array[s]<elements[i]==false)
			return;
		//���򣬶Ի�λ�ã�s����ָ��������Ԫ��
		T t=elements[i];
		elements[i]=elements[s];
		elements[s]=t;
		s=i;
	}
	*/
	//�Ż�
	T t=elements[s];
	for(u32 i=s<<1;i<=e;i<<=1){
		//�Ƚ����Һ��ӣ�ѡ��ϴ��
		if(i<e&&elements[i]<elements[i+1])
			++i;
		//���elements[s]����������ѡ�ĺ��ӣ�������ѵĶ��壬��������
		if(t<elements[i]==false)
			break;
		//���򣬶Ի�λ�ã�s����ָ��������Ԫ��
		elements[s]=elements[i];
		s=i;
	}
	elements[s]=t;
}

//��˳���elements���ж�����,����Ϊ��С����
template<class T>
inline void heapsort(T* elements,u32 size){
	/*
	u32 i;
	//��elements[1...size]���ɴ󶥶�
	for(i=size/2;i>0;--i)
		heaprise(elements,i,size);
	//���߶Ѷ�Ԫ�أ�����δ����Ķ�βԪ���滻
	for(i=size;i>1;--i){
		T t=elements[1];
		elements[1]=elements[i];
		elements[i]=t;
		//��elements[1...(i-1)]���µ���Ϊ�󶥶�
		heaprise(elements,1,i-1);
	}*/

	if(size<=1)
		return;

	u32 i;
	T* virtualArray=elements-1;
	//��elements[0...size-1]���ɴ󶥶�
	for(i=size/2;i>0;--i)
		heaprise(virtualArray,i,size);
	//���߶Ѷ�Ԫ�أ�����δ����Ķ�βԪ���滻
	for(i=size;i>1;--i){
		T t=virtualArray[1];
		virtualArray[1]=virtualArray[i];
		virtualArray[i]=t;
		//��elements[0...(i-2)]���µ���Ϊ�󶥶�
		heaprise(virtualArray,1,i-1);
	}
}
}
}
#endif