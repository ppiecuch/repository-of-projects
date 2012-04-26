#ifndef _YON_CORE_BSTREE_H_
#define _YON_CORE_BSTREE_H_

#include "btree.h"

namespace yon{
namespace core{

	//�˽ṹĿǰ������Խ׶Σ�����ֱ��ʹ�ã�ʹ�÷�����Test��Ŀ
	//cygwin�±��벻��
	template<class T>
	class bstreenode : public btreenode<T>{
	protected:
		virtual btreenode<T>* generate(){
			return new bstreenode();
		}
		btreenode<T>* bsInsert(T t){
			if(nil){
				setValue(t);
				return this;
			}
			if(t<value){
				if(leftChild==NULL){
					leftChild=generate();
					setLeftChild(leftChild);
				}
				return ((bstreenode*)leftChild)->bsInsert(t);
			}
			if(value<t){
				if(rightChild==NULL){
					rightChild=generate();
					setRightChild(rightChild);
				}
				return ((bstreenode*)rightChild)->bsInsert(t);
			}
			return NULL;
		}
	public:
		bstreenode():btreenode(){}
		bstreenode(T value):btreenode(value){}
		virtual btreenode<T>* getMin(){
			while(leftChild)
				return leftChild->getMin();
			return this;
		}
		virtual btreenode<T>* getMax(){
			while(rightChild)
				return rightChild->getMin();
			return this;
		}
		//����
		virtual btreenode<T>* find(T t) {
			if(t==value)
				return this;
			if(t<value&&leftChild!=NULL)
				return leftChild->find(t);
			if(value<t&&rightChild!=NULL)
				return rightChild->find(t);
			return NULL;
		}
		//���룬����Ѿ�����t������NULL�����򷵻ز���Ľڵ�
		virtual btreenode<T>* insert(T t) {
			return bsInsert(t);
		}
		//ɾ�������ɾ���ɹ�����true�����򷵻�false
		virtual bool remove(T t){
			return false;
		}
	};

	#define  bstree bstreenode
}
}
#endif