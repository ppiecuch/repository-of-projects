#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

/*******************************************
类名:HashMap(哈希表)
功能:
1、put/get/containKey/remove/clear/size/empty
2、目前KEY只支持int/string/HashMapKeyInterface
版本:2011-5-19
********************************************/

#include<string>

using namespace std;
#define HASH_MAP_CAPACITY 1024

class HashMapKeyInterface{
public:
	virtual int hashCode() const=0;
	virtual bool equal(const HashMapKeyInterface* const &i) const=0;
};
template <typename K,typename V> class HashMap;
template <typename K,typename V> class HashNode{
private:
	K key;		//键
	V value;	//值
	HashNode<K,V> *next;
	HashNode():next(NULL){}
	HashNode(const K &k,const V &v):key(k),value(v),next(NULL){}
	~HashNode()
	{
		if(next!=NULL)
			delete next;
	}
public:
	friend class HashMap<K,V>;
};
template <typename K,typename V> class HashMap{
protected:
	unsigned int elementSize;
	HashNode<K,V> **nodes;

	template<class KK> int hash(const KK &k){
		try{
			const HashMapKeyInterface* const &p = k;
			return hash(p);
		}catch(...){
			return hash(k);
		} 
	}
	template<class KK> bool compareKey(const KK &k,const K &nodeKey){
		try{
			const HashMapKeyInterface* const &p = k;
			const  HashMapKeyInterface* const &np=nodeKey;
			return compareKey(p,np);
		}catch(...){
			return k==nodeKey;
		}
	}
	int hash(const string &str)
	{
		unsigned long hashValue=0;
		int length=(int)str.length();
		for(int i=0;i<length;i++)
			hashValue=5*hashValue+str.at(i);
		return int(hashValue%HASH_MAP_CAPACITY);
	}
	bool compareKey(const string &str,const string &nodeKey)
	{
		return str==nodeKey;
	}
	int hash(const HashMapKeyInterface* const &i)
	{
		return i->hashCode()%HASH_MAP_CAPACITY;
	}
	bool compareKey(const HashMapKeyInterface* const &i,const HashMapKeyInterface* const &nodeKey)
	{
		return i->equal(nodeKey);
	}
public:
	HashMap()
	{
		this->elementSize=0;
		nodes=new HashNode<K,V>*[HASH_MAP_CAPACITY];
		for(int i=0;i<HASH_MAP_CAPACITY;i++)
			nodes[i]=NULL;
	}
	~HashMap()
	{
		delete[] nodes;
	}
	bool empty() const{return this->elementSize==0;}
	int size() const{return this->elementSize;}
	bool containKey(const K &k)
	{
		return this->get(k)!=NULL;
	}
	void put(const K &k,const V &v) 
	{
		int hashCode=hash(k);
		//指向上一个节点
		HashNode<K,V>* pre=NULL;
		HashNode<K,V>* node=nodes[hashCode];
		while(node!=NULL){
			//如果存在此key,使用之
			if(compareKey(k,node->key))
				break;
			pre=node;
			node=node->next;
		}
		//如果找不到key对应的记录,新建一个节点
		if(node==NULL)
		{
			node=new HashNode<K,V>();
			if(pre==NULL)
				nodes[hashCode]=node;
			else
				pre->next=node;
			//节点数加1
			this->elementSize++;
		}
		node->key=k;
		node->value=v;
	}
	const V& get(const K &k)
	{
		int hashCode=hash(k);
		HashNode<K,V>* node=nodes[hashCode];
		while(node!=NULL){
			if(compareKey(k,node->key))
				return node->value;
			node=node->next;
		}
		node=new HashNode<K,V>();
	}
	bool remove(const K &k)
	{
		int hashCode=hash(k);
		HashNode<K,V>* pre=NULL;
		HashNode<K,V>* node=nodes[hashCode];
		while(node!=NULL){
			if(compareKey(k,node->key))
			{
				//将首节点指向本节点的next指针
				if(pre==NULL)
					nodes[hashCode]=node->next;
				//将上个节点的next指针指向本节点的next指针
				else
					pre->next=node->next;
				//节点数减1
				this->elementSize--;
				//释放内存
				delete node;
				return true;
			}
			pre=node;
			node=node->next;
		}
		return false;
	}
	void clear()
	{
		for(int i=0;i<HASH_MAP_CAPACITY;i++)
		{
			//如果不空,析构之,并重置指针
			if(nodes[i]!=NULL)
			{
				delete nodes[i];
				nodes[i]=NULL;
			}
		}
		this->elementSize=0;
	}
};

/*template <typename K,typename V> class HashMap{
protected:
	unsigned int elementSize;
	HashNode<K,V> **nodes;

	//int hash(const K &k){return 0;}
	//bool compareKey(const K &k,const HashNode<K,V>* const &node){return false;}
	//#include <typeinfo.h>if(typeid(k)==typeid(unsigned int))return hash(static_cast<int>k);
	//template<class KK> int hash(const KK &k){return k%HASH_MAP_CAPACITY;}
	//template<class KK> bool compareKey(const KK &k,const HashNode<K,V>* const &node){return k==node->key;}
	template<class KK> int hash(const KK &k){
		//try{
			const HashMapKeyInterface* const &p = k;
			//const HashMapKeyInterface* p = dynamic_cast<const HashMapKeyInterface*>(k);
			//HashMapKeyInterface* p = (HashMapKeyInterface*)(k);
			return hash(p);
		//}catch(...){
			//return k%HASH_MAP_CAPACITY;
		//} 
	}
	template<class KK> bool compareKey(const KK &k,const HashNode<K,V>* const &node){
		//try{
			const HashMapKeyInterface* const &p = k;
			//const HashMapKeyInterface* p = dynamic_cast<const HashMapKeyInterface*>(k);
			//HashMapKeyInterface* p = (HashMapKeyInterface*)(k);
			//const HashNode<HashMapKeyInterface*,V>* const &node2=node;
			return compareKey1(p,node);
		//}catch(...){
		//	return k==node->key;
		//}
	}
	int hash(const string &str)
	{
		unsigned long hashValue=0;
		int length=(int)str.length();
		for(int i=0;i<length;i++)
			hashValue=5*hashValue+str.at(i);
		return int(hashValue%HASH_MAP_CAPACITY);
	}
	bool compareKey(const string &str,const HashNode<string,V>* const &node)
	{
		return str==node->key;
	}
	int hash(const HashMapKeyInterface* const &i)
	{
		return i->hashCode()%HASH_MAP_CAPACITY;
	}
	bool compareKey1(const HashMapKeyInterface* const &i,const HashNode<HashMapKeyInterface*,V>* const &node)
	{
		return i->equal(node->key);
	}
public:
	HashMap()
	{
		this->elementSize=0;
		nodes=new HashNode<K,V>*[HASH_MAP_CAPACITY];
		for(int i=0;i<HASH_MAP_CAPACITY;i++)
			nodes[i]=NULL;
	}
	~HashMap()
	{
		delete[] nodes;
	}
	bool empty() const{return this->elementSize==0;}
	int size() const{return this->elementSize;}
	bool containKey(const K &k)
	{
		return this->get(k)!=NULL;
	}
	void put(const K &k,const V &v) 
	{
		int hashCode=hash(k);
		//指向上一个节点
		HashNode<K,V>* pre=NULL;
		HashNode<K,V>* node=nodes[hashCode];
		while(node!=NULL){
			//如果存在此key,使用之
			if(compareKey(k,node))
				break;
			pre=node;
			node=node->next;
		}
		//如果找不到key对应的记录,新建一个节点
		if(node==NULL)
		{
			node=new HashNode<K,V>();
			if(pre==NULL)
				nodes[hashCode]=node;
			else
				pre->next=node;
			//节点数加1
			this->elementSize++;
		}
		node->key=k;
		node->value=v;
	}
	const V& get(const K &k)
	{
		int hashCode=hash(k);
		HashNode<K,V>* node=nodes[hashCode];
		while(node!=NULL){
			if(compareKey(k,node))
				return node->value;
			node=node->next;
		}
		return NULL;
	}
	const V& remove(const K &k)
	{
		int hashCode=hash(k);
		HashNode<K,V>* pre=NULL;
		HashNode<K,V>* node=nodes[hashCode];
		while(node!=NULL){
			if(compareKey(k,node))
			{
				//将首节点指向本节点的next指针
				if(pre==NULL)
					nodes[hashCode]=node->next;
				//将上个节点的next指针指向本节点的next指针
				else
					pre->next=node->next;
				//节点数减1
				this->elementSize--;
				return node->value;
			}
			pre=node;
			node=node->next;
		}
		return NULL;
	}
	void clear()
	{
		for(int i=0;i<HASH_MAP_CAPACITY;i++)
		{
			//如果不空,析构之,并重置指针
			if(nodes[i]!=NULL)
			{
				delete nodes[i];
				nodes[i]=NULL;
			}
		}
		this->elementSize=0;
	}
};*/

/*

#define HASH_MAP_CAPACITY 1024

class HashMapKeyInterface{
public:
	virtual int hashCode();
	virtual bool equal(const HashMapKeyInterface* const &i);
};
template <class K,class V> class HashMap;
template <class K,class V> class HashNode{
private:
	K key;		//键
	V value;	//值
	HashNode<K,V> *next;
	HashNode():next(NULL){}
	HashNode(const K &k,const V &v):key(k),value(v),next(NULL){}
	~HashNode()
	{
		if(next!=NULL)
			delete next;
	}
public:
	friend class HashMap<K,V>;
};

template <class K,class V> class HashMap{
protected:
	unsigned int elementSize;
	HashNode<K,V> **nodes;

	//散列函数
	int hash(const K &k);*/
	/*template<class KK>  int hash(const KK &k);
	template<class KK>  int hash(const KK &k){return 0;}
    template<> int hash<int>(const int &i)
	{
		return i%HASH_MAP_CAPACITY;
	}
	//template<> int hash<const char*>(const char* const &str) {return 0;}
	template<> int hash<string>(const string &str) 
	{
		unsigned long hashValue=0;
		int length=(int)str.length();
		for(int i=0;i<length;i++)
			hashValue=5*hashValue+str.at(i);
		return int(hashValue%HASH_MAP_CAPACITY);
	}
	template<> int hash<HashMapKeyInterface>(const HashMapKeyInterface &i)
	{
		return i.hashCode()%HASH_MAP_CAPACITY;
	}*/

	//比较函数
	//bool compareKey(const K &k,const HashNode<K,V>* const &node);
	//template<class KK>  bool compareKey(const KK &k,const HashNode<K,V>* const &node){return false;}
	/*template<>  bool compareKey<int>(const int &i,const HashNode<K,V>* const &node)
	{
		return i==node->key;
	}
	template<>  bool compareKey<string>(const string &str,const HashNode<K,V>* const &node)
	{
		return str==node->key;
	}
	template<>  bool compareKey<HashMapKeyInterface>(const HashMapKeyInterface &i,const HashNode<K,V>* const &node)
	{
		return i.equal(node->key);
	}*//*
public:
	HashMap()
	{
		this->elementSize=0;
		nodes=new HashNode<K,V>*[HASH_MAP_CAPACITY];
		for(int i=0;i<HASH_MAP_CAPACITY;i++)
			nodes[i]=NULL;
	}
	~HashMap()
	{
		delete[] nodes;
	}
	bool empty() const{return this->elementSize==0;}
	int size() const{return this->elementSize;}
	bool containKey(const K &k)
	{
		return this->get(k)!=NULL;
	}
	void put(const K &k,const V &v) 
	{
		int hashCode=hash(k);
		//指向上一个节点
		HashNode<K,V>* pre=NULL;
		HashNode<K,V>* node=nodes[hashCode];
		while(node!=NULL){
			//如果存在此key,使用之
			if(compareKey(k,node))
				break;
			pre=node;
			node=node->next;
		}
		//如果找不到key对应的记录,新建一个节点
		if(node==NULL)
		{
			node=new HashNode<K,V>();
			if(pre==NULL)
				nodes[hashCode]=node;
			else
				pre->next=node;
			//节点数加1
			this->elementSize++;
		}
		node->key=k;
		node->value=v;
	}
	const V& get(const K &k)
	{
		int hashCode=hash(k);
		HashNode<K,V>* node=nodes[hashCode];
		while(node!=NULL){
			if(compareKey(k,node))
				return node->value;
			node=node->next;
		}
		return NULL;
	}
	const V& remove(const K &k)
	{
		int hashCode=hash(k);
		HashNode<K,V>* pre=NULL;
		HashNode<K,V>* node=nodes[hashCode];
		while(node!=NULL){
			if(compareKey(k,node))
			{
				//将首节点指向本节点的next指针
				if(pre==NULL)
					nodes[hashCode]=node->next;
				//将上个节点的next指针指向本节点的next指针
				else
					pre->next=node->next;
				//节点数减1
				this->elementSize--;
				return node->value;
			}
			pre=node;
			node=node->next;
		}
		return NULL;
	}
	void clear()
	{
		for(int i=0;i<HASH_MAP_CAPACITY;i++)
		{
			//如果不空,析构之,并重置指针
			if(nodes[i]!=NULL)
			{
				delete nodes[i];
				nodes[i]=NULL;
			}
		}
		this->elementSize=0;
	}
};

//template<>  int HashMap::hash(const K &k){return 0;}
template<int,class V> int HashMap<int,V>::hash(const int &i)
	{
		return i%HASH_MAP_CAPACITY;
	}
	//template<> int hash<const char*>(const char* const &str) {return 0;}
	template<> int HashMap::hash<string>(const string &str) 
	{
		unsigned long hashValue=0;
		int length=(int)str.length();
		for(int i=0;i<length;i++)
			hashValue=5*hashValue+str.at(i);
		return int(hashValue%HASH_MAP_CAPACITY);
	}
	template<> int HashMap::hash<HashMapKeyInterface>(const HashMapKeyInterface &i)
	{
		return i.hashCode()%HASH_MAP_CAPACITY;
	}
template<>  bool HashMap<int,class V>::compareKey(const int &i,const HashNode<int,V>* const &node)
	{
		return i==node->key;
	}*/
	/*template<>  bool HashMap::compareKey<string>(const string &str,const HashNode<K,V>* const &node)
	{
		return str==node->key;
	}
	template<>  bool HashMap::compareKey<HashMapKeyInterface>(const HashMapKeyInterface &i,const HashNode<K,V>* const &node)
	{
		return i.equal(node->key);
	}*/
/*
#define HASH_MAP_CAPACITY 1024

class HashMapKeyInterface{
public:
	virtual int hashCode() const;
	virtual bool equal(const HashMapKeyInterface* const &i) const;
};

template <class K,class V> class HashMap;
template <class K,class V> class HashNode{
private:
	K key;		//键
	const V* value;	//值
	HashNode<K,V> *next;
	HashNode():next(NULL),value(NULL){}
	//HashNode(const K &k,const V* const &v):key(k),value(v),next(NULL){}
	~HashNode()
	{
		if(next!=NULL)
			delete next;
	}
public:
	friend class HashMap<K,V>; 
};
template <class K,class V>
class HashMap
{
protected:
	unsigned int elementSize;
	HashNode<K,V> **nodes;
	//散列函数
    template<class KK> int hashCode(const KK &k);
    //比较函数
	template<class KK> bool compareKey(const KK &k,const HashNode<K,V>* const &node);
public:
	HashMap()
	{
		this->elementSize=0;
		nodes=new HashNode<K,V>*[HASH_MAP_CAPACITY];
		for(int i=0;i<HASH_MAP_CAPACITY;i++)
			nodes[i]=NULL;
	}
	~HashMap()
	{
		delete[] nodes;
	}
	bool empty() const{return this->elementSize==0;}
	int size() const{return this->elementSize;}
	bool containKey(const K &k)
	{
		return this->get(k)!=NULL;
	}
	void put(const K &k,const V &v) 
	{
		int hashCode=this->hashCode(k);
		//指向上一个节点
		HashNode<K,V>* pre=NULL;
		HashNode<K,V>* node=nodes[hashCode];
		while(node!=NULL){
			//如果存在此key,使用之
			if(compareKey(k,node))
				break;
			pre=node;
			node=node->next;
		}
		//如果找不到key对应的记录,新建一个节点
		if(node==NULL)
		{
			node=new HashNode<K,V>();
			if(pre==NULL)
				nodes[hashCode]=node;
			else
				pre->next=node;
			//节点数加1
			this->elementSize++;
		}
		node->key=k;
		node->value=&v;
	}
	const V* get(const K &k)
	{
		int hashCode=this->hashCode(k);
		HashNode<K,V>* node=nodes[hashCode];
		while(node!=NULL){
			if(compareKey(k,node))
				return node->value;
			node=node->next;
		}
		return NULL;
	}
	const V* remove(const K &k)
	{
		int hashCode=this->hashCode(k);
		HashNode<K,V>* pre=NULL;
		HashNode<K,V>* node=nodes[hashCode];
		while(node!=NULL){
			if(compareKey(k,node))
			{
				//将首节点指向本节点的next指针
				if(pre==NULL)
					nodes[hashCode]=node->next;
				//将上个节点的next指针指向本节点的next指针
				else
					pre->next=node->next;
				//节点数减1
				this->elementSize--;
				return node->value;
			}
			pre=node;
			node=node->next;
		}
		return NULL;
	}
	void clear()
	{
		for(int i=0;i<HASH_MAP_CAPACITY;i++)
		{
			//如果不空,析构之,并重置指针
			if(nodes[i]!=NULL)
			{
				delete nodes[i];
				nodes[i]=NULL;
			}
		}
		this->elementSize=0;
	}
};
template<class K,class V> int HashMap<K,V>::hashCode(const K &k){return 0;}
template<class K,class V> bool HashMap::compareKey(const K &k,const HashNode<K,V>* const &node){return false;}
template<> int HashMap<int,class V>::hashCode(const int &i){return i%HASH_MAP_CAPACITY;}
//template<> int HashMap<int,class V>::hashCode(const int &i){return i%HASH_MAP_CAPACITY;}
template<> bool HashMap<int,class V>::compareKey(const int &i,const HashNode<int,V>* const &node){return i==node->key;}

template<> int HashMap<string,class V>::hashCode(const string &str) 
{
	unsigned long hashValue=0;
	int length=(int)str.length();
	for(int i=0;i<length;i++)
		hashValue=5*hashValue+str.at(i);
	return int(hashValue%HASH_MAP_CAPACITY);
}
template<>  bool HashMap<string,class V>::compareKey(const string &str,const HashNode<string,V>* const &node){return str==node->key;}

//template<> int HashMap<HashMapKeyInterface,class V>::hashCode(const HashMapKeyInterface &i){return i.hashCode()%HASH_MAP_CAPACITY;}
//template<>  bool HashMap<HashMapKeyInterface,class V>::compareKey(const HashMapKeyInterface &i,const HashNode<HashMapKeyInterface,V>* const &node){return i.equal(&node->key);}
//template<> bool HashMap<int,class V>::compareKey(const int &i,const HashNode<int,V> &node){return i==node.key;}
/*template<> void A<char>::test(char input);
template<> void A<short>::test(short input);
template<> void A<double>::test(double input);*/
//template<class K,class V> int HashMap<K,V>::hashCode(const K &k){return 0;}
/*
template <class T,class V>
class mypair {
    T a, b;
  public:
    mypair (T first, T second)
      {a=first; b=second;}
    T getmax ();
};

template <class T,class V>
T mypair<T,V>::getmax ()
{
  T retval;
  retval = a>b? a : b;
  return retval;
}


// class template:
template <class T>
class mycontainer {
    T element;
  public:
    mycontainer (T arg) {element=arg;}
    T increase () {return ++element;}
};

// class template specialization:
template <>
class mycontainer <char> {
    char element;
  public:
    mycontainer (char arg) {element=arg;}
    char uppercase ()
    {
      if ((element>='a')&&(element<='z'))
      element+='A'-'a';
      return element;
    }
};*/


#endif
