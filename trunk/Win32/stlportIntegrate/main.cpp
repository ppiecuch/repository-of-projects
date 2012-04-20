#include <stdio.h>
#include <vector>   
#include <hash_map>
#include <deque>
#include <list>
using namespace std;
//using namespace stdext;

int main(int argc, char* argv[]){
	vector<int>  v;
	v.push_back(0);

	hash_map<int,int> m;

	deque<int> q;

	list<int> l;

	printf("%d\n",v.size());
	getchar();
	return 0;
}