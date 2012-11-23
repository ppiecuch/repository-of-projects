#ifndef _YON_CORE_YONMAP_H_
#define _YON_CORE_YONMAP_H_

#include "yonTypes.h"

namespace yon{
namespace core{

	//			Average		Worst case
	//Space		O(n)		O(n)
	//Search	O(log n)	O(log n)
	//Insert	O(log n)	O(log n)
	//Delete	O(log n)	O(log n)

	//使用注意：K需要提供“<”、“==”操作符运算
	template<class K,class V>
	class map{
	private:
		//红黑树
		template<class TK,class TV>
		class rbtree{
		private:
			bool red :1;
			TK key;
			TV value;

			rbtree* parent;
			rbtree* lchild;
			rbtree* rchild;

			rbtree();

			inline void setKey(const TK& k){key=k;}
			friend class map<K,V>;
		public:
			rbtree(const TK& k,const TV& v)
				:red(true),key(k),value(v),parent(NULL),lchild(NULL),rchild(NULL){}
			inline rbtree* getParent(){return parent;}
			inline rbtree* getLeftChild(){return lchild;}
			inline rbtree* getRightChild(){return rchild;}
			inline rbtree* getSibling(){
				if(isRoot())
					return NULL;
				if(parent->getLeftChild()==this)
					return parent->getRightChild();
				if(parent->getRightChild()==this)
					return parent->getLeftChild();
				return NULL;
			}
			inline TK getKey(){return key;}
			inline TV getValue(){return value;}

			inline bool isRed(){return red;}
			inline bool isBlack(){return !red;}
			inline bool isRoot(){return parent==NULL;}
			inline bool isLeaf(){return lchild==NULL&&rchild==NULL;}
			inline bool isLeftChild(){return parent!=NULL&&this==parent->lchild;}
			inline bool isRightChild(){return parent!=NULL&&this==parent->rchild;}

			//获取层数，根的层数为1
			inline u32 getLevel(){
				rbtree* n=this;
				u32 num=1;
				while((n=n->getParent())!=NULL){
					++num;
				}
				return num;
			}

			inline void setParent(rbtree* p){parent=p;}
			inline void setLeftChild(rbtree* n){lchild=n;if(n)n->setParent(this);}
			inline void setRightChild(rbtree* n){rchild=n;if(n)n->setParent(this);}
			inline void setRed(){red=true;}
			inline void setBlack(){red=false;}
			inline void setValue(const TV& v){value=v;}
		};
	public:
		typedef rbtree<K,V> Node;

		//中序遍历，即按K从小到大进行访问
		class Iterator{
		private:
			Node* root;
			Node* current;

			Node* getMin(Node* n){
				while(n && n->getLeftChild())
					n = n->getLeftChild();
				return n;
			}
			Node* getMax(Node* n){
				while(n && n->getRightChild())
					n = n->getRightChild();
				return n;
			}
			void inc(){
				if(atEnd())
					return;

				if (current->getRightChild())
					current = getMin(current->getRightChild());
				else if (current->isLeftChild())
					current = current->getParent();
				else
				{
					while(current->isRightChild())
						current = current->getParent();
					current = current->getParent();
				}
			}
			void dec(){
				if(atEnd())
					return;
				if (current->getLeftChild())
					current = getMax(current->getLeftChild());
				else if (current->isRightChild())
					current = current->getParent();
				else
				{
					while(current->isLeftChild())
						current = current->getParent();
					current = current->getParent();
				}
			}
		public:
			Iterator():root(NULL),current(NULL){}
			explicit Iterator(Node* root):root(root),current(getMin(root)){}
			Iterator(const Iterator& it){
				*this=it;
			}

			inline bool atEnd() const{return current==NULL;}
			inline Node* getNode(){return current;}

			Iterator& operator=(const Iterator& src){root=src.root;current=src.current;return *this;}
			Iterator& operator++(){inc();return *this;}
			Iterator operator++(int){inc();return Iterator(root);}
			Iterator& operator--(){dec();return *this;}
			Iterator operator--(int){dec();return Iterator(root);}
			Node* operator->(){return getNode();}
			Node& operator*(){YON_DEBUG_BREAK_IF(atEnd());return *getNode();}
		};

		//后序遍历，注意此迭代器不支持“--”操作
		class PostorderIterator{
		private:
			Node* root;
			Node* current;

			Node* getMin(Node* n){
				while(n!=NULL && (n->getLeftChild()!=NULL || n->getRightChild()!=NULL))
				{
					if (n->getLeftChild())
						n = n->getLeftChild();
					else
						n = n->getRightChild();
				}
				return n;
			}
			void inc(){
				if(atEnd())
					return;
				if (current->isLeftChild() && current->getParent()->getRightChild())
				{
					current = getMin(current->getParent()->getRightChild());
				}
				else
					current = current->getParent();
			}
		public:
			PostorderIterator():root(NULL),current(NULL){}
			explicit PostorderIterator(Node* root):root(root),current(getMin(root)){}
			PostorderIterator(const PostorderIterator& it){
				*this=it;
			}

			inline bool atEnd() const{return current==NULL;}
			inline Node* getNode(){return current;}

			PostorderIterator& operator=(const PostorderIterator& src){root=src.root;current=src.current;return *this;}
			PostorderIterator& operator++(){inc();return *this;}
			PostorderIterator operator++(int){inc();return PostorderIterator(root);}
			Node* operator->(){return getNode();}
			Node& operator*(){YON_DEBUG_BREAK_IF(atEnd());return *getNode();}
		};

		// AccessClass is a temporary class used with the [] operator.
		// It makes it possible to have different behavior in situations like:
		// myTree["Foo"] = 32;
		// If "Foo" already exists update its value else insert a new element.
		// int i = myTree["Foo"]
		// If "Foo" exists return its value.
		class AccessClass
		{
			// Let map be the only one who can instantiate this class.
			friend class map<K, V>;
		public:
			// Assignment operator. Handles the myTree["Foo"] = 32; situation
			void operator=(const V& value)
			{
				// Just use the Set method, it handles already exist/not exist situation
				m_tree.set(m_key,value);
			}

			// ValueType operator
			operator V()
			{
				Node* node = m_tree.find(m_key);

				// Not found
				YON_DEBUG_BREAK_IF(node==0) // access violation
				return node->getValue();
			}
		private:
			AccessClass(map& tree, const K& key) : m_tree(tree), m_key(key) {}
			AccessClass();

			map& m_tree;
			const K& m_key;
		}; // AccessClass
	private:
		//禁止复制构造与赋值
		explicit map(const map& src);
		map& operator = (const map& src);

		void setRoot(Node* n){
			root = n;
			if (root != NULL)
			{
				root->setParent(NULL);
				root->setBlack();
			}
		}
		//插入节点，不进行平衡操作
		bool insert(Node* n){
			bool result=true; 
			if (root==NULL){
				setRoot(n);
				count = 1;
			}else{
				Node* p = root;
				K k = n->getKey();
				while (p)
				{
					K key(p->getKey());

					if (k == key){
						result = false;
						p = NULL;
					}else if (k < key){
						if (p->getLeftChild() == NULL){
							p->setLeftChild(n);
							p = NULL;
						}
						else
							p = p->getLeftChild();
					}else{
						if (p->getRightChild()==NULL){
							p->setRightChild(n);
							p = NULL;
						}else{
							p = p->getRightChild();
						}
					}
				}

				if (result)
					++count;
			}
			return result;
		}
		void rotateLeft(Node* n){
			Node* right = n->getRightChild();

			n->setRightChild(right->getLeftChild());

			if (n->isLeftChild())
				n->getParent()->setLeftChild(right);
			else if (n->isRightChild())
				n->getParent()->setRightChild(right);
			else
				setRoot(right);

			right->setLeftChild(n);
		}
		void rotateRight(Node* n){
			Node* left = n->getLeftChild();

			n->setLeftChild(left->getRightChild());

			if (n->isLeftChild())
				n->getParent()->setLeftChild(left);
			else if (n->isRightChild())
				n->getParent()->setRightChild(left);
			else
				setRoot(left);

			left->setRightChild(n);
		}

		Node* root;
		u32 count;
	public:
		map() : root(0), count(0) {}
		~map(){clear();}

		void preorderTraversal(Node* n){
			if(n->isRoot())
				printf("\npreorder:");
			printf("%d(%c)",n->getKey(),n->isRed()?'R':'B');
			if(n->getLeftChild())
				preorderTraversal(n->getLeftChild());
			else
				printf("-");
			if(n->getRightChild())
				preorderTraversal(n->getRightChild());
			else
				printf("-");
			if(n->isRoot())
				printf("\n");
		}


		//插入，如果已经存在k，返回false，否则返回true
		bool insert(const K& k,const V& v){
			Node* n = new Node(k,v);
			if (!insert(n)){
				delete n;
				return false;
			}
			while (!n->isRoot() && (n->getParent()->isRed()))
			{
				if (n->getParent()->isLeftChild())
				{
					Node* u = n->getParent()->getParent()->getRightChild();
					if ( u!=NULL && u->isRed()){
						n->getParent()->setBlack();
						u->setBlack();
						n->getParent()->getParent()->setRed();
						n = n->getParent()->getParent();
					}else{
						if ( n->isRightChild())
						{
							n = n->getParent();
							rotateLeft(n);
						}
						n->getParent()->setBlack();
						n->getParent()->getParent()->setRed();
						rotateRight(n->getParent()->getParent());
					}
				}
				else
				{
					Node* u = n->getParent()->getParent()->getLeftChild();
					if ( u!=0 && u->isRed()){
						n->getParent()->setBlack();
						u->setBlack();
						n->getParent()->getParent()->setRed();
						n = n->getParent()->getParent();
					}else{
						if (n->isLeftChild()){
							n = n->getParent();
							rotateRight(n);
						}
						n->getParent()->setBlack();
						n->getParent()->getParent()->setRed();
						rotateLeft(n->getParent()->getParent());
					}
				}
			}
			root->setBlack();
			return true;
		}
		//设置，如果已经存在k，替换之，否则插入新节点
		void set(const K& k,const V& v){
			Node* p = find(k);
			if (p)
				p->setValue(v);
			else
				insert(k,v);
		}
		//如果存在k对应的节点，删除之，返回true，否则返回false
		bool remove(const K& k,V* vp=NULL){
			Node* n = find(k);
			if (n == NULL)
				return false;

			if(vp!=NULL)
				*vp=n->getValue();
			Node* tmp=n;
			if(tmp->getLeftChild()&&tmp->getRightChild()){
				tmp=tmp->getRightChild();
				while(tmp->getLeftChild())
					tmp=tmp->getLeftChild();
				n->setValue(tmp->getValue());
				n->setKey(tmp->getKey());
				n=tmp;
			}

			Node* c=n->getLeftChild();
			if(c==NULL)
				c=n->getRightChild();

			if(n->isBlack()){
				if(c&&c->isRed())
				{
					c->setBlack();
				}
				else
				{
					Node *s,*p;
					bool slr,srr;
					while(n->isRoot()==false)
					{
						p=n->getParent();
						if(n->isLeftChild()){
							s=p->getRightChild();
							if(s->isRed()){
								s->setBlack();
								p->setRed();
								rotateLeft(p);
								s=p->getRightChild();
							}
							slr=s->getLeftChild()?s->getLeftChild()->isRed():false;
							srr=s->getRightChild()?s->getRightChild()->isRed():false;
							if(!slr&&!srr){
								s->setRed();
								if(p->isBlack()){
									n=p;
									continue;
								}else{
									p->setBlack();
								}
							}else{
								if(slr){
									if(s->getLeftChild())s->getLeftChild()->setBlack();
									s->setRed();
									rotateRight(s);
									s=p->getRightChild();
									slr=s->getLeftChild()?s->getLeftChild()->isRed():false;
									srr=s->getRightChild()?s->getRightChild()->isRed():false;
								}
								if(p->isRed())
									s->setRed();
								else
									s->setBlack();
								p->setBlack();
								s->getRightChild()->setBlack();
								rotateLeft(p);
							}
						}else{
							s=p->getLeftChild();
							if(s->isRed()){
								s->setBlack();
								p->setRed();
								rotateRight(p);
								s=p->getLeftChild();
							}
							slr=s->getLeftChild()?s->getLeftChild()->isRed():false;
							srr=s->getRightChild()?s->getRightChild()->isRed():false;
							if(!slr&&!srr){
								s->setRed();
								if(p->isBlack()){
									n=p;
									continue;
								}else{
									p->setBlack();
								}
							}else{
								if(srr){
									if(s->getRightChild())s->getRightChild()->setBlack();
									s->setRed();
									rotateLeft(s);
									s=p->getLeftChild();
									slr=s->getLeftChild()?s->getLeftChild()->isRed():false;
									srr=s->getRightChild()?s->getRightChild()->isRed():false;
								}
								if(p->isRed())
									s->setRed();
								else
									s->setBlack();
								p->setBlack();
								s->getLeftChild()->setBlack();
								rotateRight(p);
							}
						}
						break;
					}
				}
			}

			n=tmp;

			if(c)
				c->setParent(n->getParent());
			if(n->isLeftChild())
				n->getParent()->setLeftChild(c);
			else if(n->isRightChild())
				n->getParent()->setRightChild(c);
			else if(n->isRoot())
				setRoot(c);

			delete n;
			--count;
	
			/*
			//疑问：满足性质5？
			//   u
			//  / \
			// p   z
			//remove(u)
			//经测试证明，此算法破坏了红黑树的性质
			while(p->getRightChild())
				rotateLeft(p);
			Node* left = p->getLeftChild();

			if (p->isLeftChild())
				p->getParent()->setLeftChild(left);
			else if (p->isRightChild())
				p->getParent()->setRightChild(left);
			else
				setRoot(left);

			delete p;
			--count;
			*/
			return true;
		}
		//清空集合
		void clear(){
			PostorderIterator i(getPostorderIterator());

			while(!i.atEnd())
			{
				Node* p = i.getNode();
				//i++; // Increment it before it is deleted
				// else iterator will get quite confused.
				++i;
				delete p;
			}
			root = 0;
			count= 0;
		}
		//TODO交换内容
		//void swap(map<K,V>& other){}

		
		//搜索节点，如果存在返回之，否则返回NULL
		Node* find(const K& k) const{
			Node* p = root;

			while(p!=NULL)
			{
				K key(p->getKey());

				if (k == key)
					return p;
				else if (k < key)
					p = p->getLeftChild();
				else
					p = p->getRightChild();
			}

			return NULL;
		}

		//集合是否为空
		bool empty() const{return root==NULL;}
		//集合的根节点
		Node* getRoot() const{return root;}
		//集合大小
		u32 size() const{return count;}
		//获取迭代器
		Iterator getIterator(){Iterator it(getRoot());return it;}
		PostorderIterator getPostorderIterator(){PostorderIterator it(getRoot());return it;}

		

		//! operator [] for access to elements
		/** for example myMap["key"] */
		AccessClass operator[](const K& k)
		{
			return AccessClass(*this, k);
		}

	};
}
}
#endif