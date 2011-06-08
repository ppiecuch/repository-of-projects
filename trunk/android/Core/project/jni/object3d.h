#ifndef _OBJECT_3D_H_
#define _OBJECT_3D_H_
/*
#include <ext/hash_set>
#include <iostream>
using namespace std;
using namespace  __gnu_cxx;//需要引入

class Site 
{ 
public: 
	int no; 
	string name; 
	Site(int no,string name) { 
		this->no=no; 
		this->name=name; 
	} 
	bool operator==(const Site &s)const { 
		return this->no==s.no; 
	}
}; 
struct my_hash {
    size_t operator()(const Site& s) const {
       return(size_t)s.no;
    }  
};
struct my_equal_to {
    bool operator()(const Site& s1, const Site& s2) const {
       return s1 == s2; 
    }
};

*/
class Object3D{
public:
	//hash_set<Site,hash<Site>,equal_to<Site>> set;
	//hash_set<Site,my_hash,my_equal_to> set;
	Object3D();
};

#endif
