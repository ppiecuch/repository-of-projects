#ifndef _YON_CORE_BTREE_H_
#define _YON_CORE_BTREE_H_

#include <stdio.h>

namespace yon{
namespace core{

	template<class T>
	class btreenode{
	protected:
		btreenode* parent;

		btreenode* leftChild;
		btreenode* rightChild;

		bool nil : 1;
		T value;
	public:
		btreenode():parent(NULL),leftChild(NULL),rightChild(NULL),value(0),nil(true){}
		btreenode(T value):parent(NULL),leftChild(NULL),rightChild(NULL),value(value),nil(false){}


		virtual ~btreenode(){
			if(leftChild)
				delete leftChild;
			if(rightChild)
				delete rightChild;
		}
		inline btreenode* getLeftChild(){return leftChild;}
		inline btreenode* getRightChild(){return rightChild;}
		inline btreenode* getParent(){return parent;}
		inline btreenode* getGrandfather(){return parent?parent->parent:NULL;}
		inline btreenode* getUncle(){return parent?parent->getSibling():NULL;}
		inline btreenode* getSibling(){
			if(isRoot())
				return NULL;
			if(parent->leftChild==this)
				return parent->rightChild;
			if(parent->rightChild==this)
				return parent->leftChild;
			return NULL;
		}
		virtual btreenode* getMin(){return NULL;}
		virtual btreenode* getMax(){return NULL;}
		inline T& getValue(){return value;}
		virtual void setValue(T t){this->value=t;nil=false;}

		unsigned int getLevel() const
		{
			if (isRoot())
				return 1;
			else
				return getParent()->getLevel() + 1;
		}

		//前序遍历
		inline void preorderTraversal(){
			if(isRoot())
				printf("\npreorder:");
			print();
			if(leftChild)
				leftChild->preorderTraversal();
			if(rightChild)
				rightChild->preorderTraversal();
			if(isRoot())
				printf("\n");
		}

		//中序遍历
		inline void inorderTraversal(){
			if(isRoot())
				printf("\ninorder:");
			if(leftChild)
				leftChild->inorderTraversal();
			print();
			if(rightChild)
				rightChild->inorderTraversal();
			if(isRoot())
				printf("\n");
		}
		//后序遍历
		inline void postorderTraversal(){
			if(isRoot())
				printf("\npostorder:");
			if(leftChild)
				leftChild->postorderTraversal();
			if(rightChild)
				rightChild->postorderTraversal();
			print();
			if(isRoot())
				printf("\n");
		}
		virtual void print(){
			switch(sizeof(value)){
			case 1:
				printf("%c",value);
				return;
			case 4:
				printf("%d",value);
				return;
			}
		}
		
		inline bool isEmpty(){return nil;}
		inline bool isLeftEmpty(){return leftChild==NULL||leftChild->isEmpty();}
		inline bool isRightEmpty(){return rightChild==NULL||rightChild->isEmpty();}
		inline bool isRoot(){return parent==NULL;}
		inline bool isLeftChild(){return !isRoot()&&parent->leftChild==this;}
		inline bool isRightChild(){return !isRoot()&&parent->rightChild==this;}
		inline bool isLeaf(){return leftChild==NULL&&rightChild==NULL;}

		//查找
		virtual btreenode* find(T t) {
			return NULL;
		}
		//插入，如果已经存在t，返回NULL，否则返回插入的节点
		virtual btreenode* insert(T t) {
			return false;
		}
		//删除，如果删除成功返回true，否则返回false
		virtual bool remove(T t){
			return false;
		}

		inline void makeRoot(){parent=NULL;}
		inline void setLeftChild(btreenode* node){
			leftChild=node;
			if(node)
				node->parent=this;
		}
		inline void setRightChild(btreenode* node){
			rightChild=node;
			if(node)
				node->parent=this;
		}

		inline void rotateLeft(){
			btreenode* right=rightChild;
			setRightChild(right->leftChild);

			if(isLeftChild())
				parent->setLeftChild(right);
			else if(isRightChild())
				parent->setRightChild(right);
			else
				right->makeRoot();

			right->setLeftChild(this);
		}

		inline void rotateRight(){
			btreenode* left=leftChild;
			setLeftChild(left->rightChild);

			if(isLeftChild())
				parent->setLeftChild(left);
			else if(isRightChild())
				parent->setRightChild(left);
			else
				left->makeRoot();

			left->setRightChild(this);
		}
	};

	#define btree btreenode
	
}
}
#endif