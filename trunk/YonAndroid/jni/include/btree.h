#ifndef _YON_CORE_BTREE_H_
#define _YON_CORE_BTREE_H_

namespace yon{
namespace core{

	template<class T>
	class btreenode{
	protected:
		btreenode* parent;

		btreenode* leftChild;
		btreenode* rightChild;
	public:

		inline btreenode* getLeftChild(){return leftChild;}
		inline btreenode* getRightChild(){return rightChild;}
		inline btreenode* getParent(){return parent;}
		
		inline bool isRoot(){return parent==NULL;}
		inline bool isLeftChild(){return !isRoot()&&parent->leftChild==this;}
		inline bool isRightChild(){return !isRoot()&&parent->rightChild==this;}
		inline bool isLeaf(){return leftChild==NULL&&rightChild==NULL;}

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