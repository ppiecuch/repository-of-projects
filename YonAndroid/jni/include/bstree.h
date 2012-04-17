#ifndef _YON_CORE_BSTREE_H_
#define _YON_CORE_BSTREE_H_

#include "btree.h"

namespace yon{
namespace core{

	template<class T>
	class bstreenode : public btreenode<T>{
	protected:
		virtual btreenode* generate(){
			return new btreenode();
		}
		btreenode* bsInsert(T t){
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
		//查找
		virtual btreenode* find(T t) {
			if(t==value)
				return this;
			if(t<value&&leftChild!=NULL)
				return leftChild->find(t);
			if(value<t&&rightChild!=NULL)
				return rightChild->find(t);
			return NULL;
		}
		//插入，如果已经存在t，返回NULL，否则返回插入的节点
		virtual btreenode* insert(T t) {
			return bsInsert(t);
		}
		//删除
		virtual bool remove(T t){
			return false;
		}

	};

	#define  bstree bstreenode
}
}
#endif