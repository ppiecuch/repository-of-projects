#include <stdio.h>
#include <stdlib.h>

#include <sys/timeb.h>
#include <time.h>

#include "yonArray.h"

namespace yon{
	class CRandomizer{
	protected:
		s32 seed;
	public:
		CRandomizer();
		virtual void reset();
		virtual s32 rand();
		virtual s32 rand(s32 start,s32 end);
	};
}

namespace yon{
	CRandomizer::CRandomizer()
		:seed(0x0f0f0f0f){
	}
	void CRandomizer::reset(){
		seed = 0x0f0f0f0f;
	}
	s32 CRandomizer::rand(){
		const s32 m = 2147483399;	// a non-Mersenne prime
		const s32 a = 40692;		// another spectral success story
		const s32 q = m/a;
		const s32 r = m%a;		// again less than q

		seed = a * (seed%q) - r* (seed/q);
		if (seed<0) seed += m;

		return seed;
	}
	s32 CRandomizer::rand(s32 start,s32 end){
		YON_DEBUG_BREAK_IF(end<=start);
		return rand()%(end-start+1)+start;
	}
}

class Entry{
public:
	Entry(int a):a(a){}
	int a;

	bool operator < (const Entry& e){
		return a<e.a;
	}

	Entry& operator = (const Entry& e){
		a=e.a;
		return *this;
	}
};

int main(int argc, char* argv[])
{
#define COUNT 100000
	struct _timeb start;
	struct _timeb end;
	long long s;
	int ms;
	yon::CRandomizer randomizer;
	yon::core::array<Entry> arr;
	int buffer[COUNT];
	for(int i=0;i<COUNT;++i)
		buffer[i]=randomizer.rand(0,COUNT);
	_ftime64_s( &start ); 
	for(int i=0;i<COUNT;++i){
		Entry m(buffer[i]);
		arr.push_back(m);
	}
	arr.sort();
	_ftime64_s( &end ); 

	s=end.time-start.time;
	ms=end.millitm-start.millitm;
	if(ms<0){
		ms+=1000;
		--s;
	}
	printf("%d.",s);
	printf("%03d\n",ms);
	system("pause");
	return 0;
}