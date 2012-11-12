#ifndef _YON_CORE_COMPARATOR_H_
#define _YON_CORE_COMPARATOR_H_

namespace yon{
namespace core{

	template<typename T>
	struct yonComparator{
		int operator()(const T& t1,const T& t2) const{
			return t1<t2?-1:t2<t1?1:0;
		}
	};

	template<typename T>
	struct yonComparatorFast{
		int operator()(const T& t1,const T& t2) const{
			return t1-t2;
		}
	};
}
}
#endif