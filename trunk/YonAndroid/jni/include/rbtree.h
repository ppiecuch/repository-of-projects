#ifndef _YON_CORE_RBTREE_H_
#define _YON_CORE_RBTREE_H_

#include "bstree.h"

namespace yon{
namespace core{

	//此结构目前处理测试阶段，不能直接使用，使用方法见Test项目
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

		virtual btreenode<T>* generate(){
			rbtreenode<T>* n=new rbtreenode();
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

		inline void poseRemove1(){
			if(isRed()){
				executeRemove();
				return;
			}
			rbtreenode* c=static_cast<rbtreenode*>(leftChild->isEmpty()?rightChild:leftChild);
			if(c->isRed()){
				executeRemove();
				setBlack();
				return;
			}
			executeRemove();
			postRemove2();
		}

		inline void postRemove2(){
			rbtreenode* p=static_cast<rbtreenode*>(getParent());
			if(p==NULL)
				return;
			rbtreenode* s=static_cast<rbtreenode*>(getSibling());
			if(s->isRed()){
				p->swapColor(s);
				if(isLeftChild())
					p->rotateLeft();
				else
					p->rotateRight();
				s=static_cast<rbtreenode*>(getSibling());
			}
			rbtreenode* sl=static_cast<rbtreenode*>(s->leftChild);
			rbtreenode* sr=static_cast<rbtreenode*>(s->rightChild);
			if(sr->isBlack()&&sl->isBlack()){
				if(p->isBlack()){
					s->setRed();
					p->postRemove2();
				}else{
					p->swapColor(s);
				}
				return;
			}
			if(isLeftChild()){
				if(sl->isRed()&&sr->isBlack()){
					s->swapColor(sl);
					s->rotateRight();
				}
			}else if(sl->isBlack()&&sr->isRed()){
				s->swapColor(sr);
				s->rotateLeft();
			}
			s=static_cast<rbtreenode*>(getSibling());
			sl=static_cast<rbtreenode*>(s->leftChild);
			sr=static_cast<rbtreenode*>(s->rightChild);

			p->swapColor(s);
			if(isLeftChild()){
				if(sl->isBlack()&&sr->isRed()){
					sr->setBlack();
					p->rotateLeft();
				}
			}else if(sl->isRed()&&sr->isBlack()){
				sl->setBlack();
				p->rotateRight();
			}
 			/*if(sr->isBlack()){
				if(sl->isBlack()){
					if(p->isBlack()){
						s->setRed();
						p->postRemove2();
					}else{
						p->swapColor(s);
					}
					return;
				}else{
					if(isLeftChild()){
						s->swapColor(sl);
						s->rotateRight();
					}
					else{
						s->swapColor(sr);
						s->rotateLeft();
					}
					s=static_cast<rbtreenode*>(getSibling());
					sl=static_cast<rbtreenode*>(s->leftChild);
					sr=static_cast<rbtreenode*>(s->rightChild);
				}
			}
			if(sr->isRed()){
				p->swapColor(s);
				if(isLeftChild()){
					sr->setBlack();
					p->rotateLeft();
				}else{
					sl->setBlack();
					p->rotateRight();
				}
			}*/
		}

		inline void executeRemove(){
			//删除，将单儿子（如果是两个nil儿子，使用任意一个）替换删除节点
			if(leftChild->isEmpty()==false){
				rbtreenode* c=static_cast<rbtreenode*>(leftChild);
				delete rightChild;
				setExceptParent(*c);
				c->setLeftChild(NULL);
				c->setRightChild(NULL);
				delete c;
			}
			else{
				rbtreenode* c=static_cast<rbtreenode*>(rightChild);
				delete leftChild;
				setExceptParent(*c);
				c->setLeftChild(NULL);
				c->setRightChild(NULL);
				delete c;
			}
		}

		inline void setExceptParent(rbtreenode& node){
			value=node.value;
			red=node.red;
			nil=node.nil;
			leftChild=node.leftChild;
			rightChild=node.rightChild;
		}
	public:
		rbtreenode():bstreenode(),red(false){}
		rbtreenode(T value):bstreenode(value),red(false){fill();}
		virtual btreenode<T>* getMin(){
			while(leftChild->isEmpty()==false)
				return leftChild->getMin();
			return this;
		}
		virtual btreenode<T>* getMax(){
			while(rightChild->isEmpty()==false)
				return rightChild->getMin();
			return this;
		}
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
		//删除，如果删除成功返回true，否则返回false
		virtual bool remove(T t){
			rbtreenode* node=static_cast<rbtreenode*>(bstreenode::find(t));
			if(node==NULL)
				return false;
			//如果存在双儿子，则转换为单儿子
			if(node->isLeftEmpty()==false&&node->isRightEmpty()==false){
				rbtreenode* m=static_cast<rbtreenode*>(node->rightChild->getMin());
				node->setValue(m->getValue());
				m->fill();
				node=m;
			}
			node->poseRemove1();
			return true;
		}
	};

	#define rbtree rbtreenode
}
}

#endif
