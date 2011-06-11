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

/*
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
	}

	//比较函数
	template<class KK>  bool compareKey(const KK &k,const HashNode<K,V>* const &node){return false;}
	template<>  bool compareKey<int>(const int &i,const HashNode<K,V>* const &node)
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
				nodes[i]->~HashNode();
				nodes[i]=NULL;
			}
		}
		this->elementSize=0;
	}
};


class HashMapFontKey:HashMapKeyInterface{
protected:
	int color;
	int size;
	char c;
public:
	HashMapFontKey(char c,int size,int color)
	{
		this->color=color;
		this->c=c;
		this->size=size;
	}
	int hashCode()
	{
		return this->c;
	}
	bool equal(const HashMapKeyInterface* const &i)
	{
		try
		{
			HashMapFontKey* p = dynamic_cast<HashMapFontKey*>(const_cast<HashMapKeyInterface*>(i));
			return this->color==p->hashCode()&&this->size==p->size&&this->c==p->c;
		}
		catch(...)
		{
			return false;
		}
		
	}
};
*/

#endif