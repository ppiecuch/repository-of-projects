#ifndef _YON_CORE_RBTREE_H_
#define _YON_CORE_RBTREE_H_

#include "bstree.h"

namespace yon{
namespace core{

	template<class T>
	class rbtreenode : public bstreenode<T>{
	protected:
		bool red : 1;
		
		inline void setRed(){red=true;}
		inline void setBlack(){red=false;}
		inline void swapColor(rbtreenode* n){
			bool temp=red;
			red=n->red;
			n->red=temp;
		}
		//确保n1/n2同色，此函数不做检验
		inline void swapColor(rbtreenode* n1,rbtreenode* n2){
			bool temp=red;
			red=n1->red;
			n2->red=n1->red=temp;
		}

		virtual btreenode* generate(){
			rbtreenode* n=new rbtreenode();
			n->fill();
			return n;
		}
		//填充nil子节点
		inline void fill(){
			if(leftChild==NULL){
				leftChild=new rbtreenode();
				setLeftChild(leftChild);
			}
			if(rightChild==NULL){
				rightChild=new rbtreenode();
				setRightChild(rightChild);
			}
		}

		inline void postInsert1(){
			if(isRoot()){
				setBlack();
				return;
			}
			rbtreenode* p=static_cast<rbtreenode*>(getParent());
			if(p->isBlack())
				return;
			rbtreenode* u=static_cast<rbtreenode*>(getUncle());
			rbtreenode* g=static_cast<rbtreenode*>(getGrandfather());
			if(u&&u->isRed()){
				g->swapColor(p,u);
				g->postInsert1();
				return;
			}
			if(isRightChild()&&p->isLeftChild()){
				p->rotateLeft();
				p->postInsert2();
			}
			else if(isLeftChild()&&p->isRightChild()){
				p->rotateRight();
				p->postInsert2();
			}else{
				postInsert2();
			}
		}
		inline void postInsert2(){
			rbtreenode* p=static_cast<rbtreenode*>(getParent());
			rbtreenode* g=static_cast<rbtreenode*>(getGrandfather());
			p->setBlack();
			g->setRed();
			if(isLeftChild()&&p->isLeftChild())
				g->rotateRight();
			if(isRightChild()&&p->isRightChild())
				g->rotateLeft();
		}
	public:
		rbtreenode():bstreenode(),red(false){}
		rbtreenode(T value):bstreenode(value),red(false){fill();}
		virtual void setValue(T t){bstreenode::setValue(t);fill();}
		virtual void print(){
			switch(sizeof(value)){
			case 1:
				printf("%c(%c)",nil?'-':value,isRed()?'R':'B');
				return;
			case 4:
				printf("%d(%c)",nil?0:value,isRed()?'R':'B');
				return;
			}
		}
		inline bool isRed(){return red;}
		inline bool isBlack(){return !red;}

		//插入，如果已经存在t，返回NULL，否则返回插入的节点
		virtual btreenode* insert(T t) {
			rbtreenode* node=static_cast<rbtreenode*>(bstreenode::insert(t));
			if(node==NULL)
				return NULL;
			node->setRed();
			node->postInsert1();
			return node;
		}
		//删除
		virtual bool remove(T t){
			return false;
		}
	};

	#define rbtree rbtreenode
}
}

#endif
