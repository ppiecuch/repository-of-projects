#ifndef _YON_CORE_BSTREE_H_
#define _YON_CORE_BSTREE_H_

#include "btree.h"

namespace yon{
namespace core{

	template<class T>
	class bstreenode : public btreenode<T>{
	public:
		//查找
		virtual btreenode* find(T& t) const {
			if(t==value)
				return this;
			if(t<value&&leftChild!=NULL)
				return leftChild->find(t);
			if(value<t&&rightChild!=NULL)
				return rightChild->find(t);
			return NULL;
		}
		//插入，如果已经存在t，返回false，否则返回true
		virtual bool insert(T& t) {
			return false;
		}
		//删除
		virtual bool remove(T& t){
			return false;
		}

	};

	#define  bstree bstreenode
}
}
#endif