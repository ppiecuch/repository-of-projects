#include "lcLock.h"


CriticalSectionLock::CriticalSectionLock(){
	//InitializeCriticalSectionAndSpinCount()
	//refer to:http://hi.baidu.com/sei_zhouyu/item/6c2e22fd5e4e2c1cfe358294
	//实际上对 CRITICAL_SECTION 的操作非常轻量，为什么还要加上旋转锁的动作呢？其实这个函数在单cpu的电脑上是不起作用的，只有当电脑上存在不止一个cpu，或者一个cpu但多核的时候，才管用。 
	//如果临界区用来保护的操作耗时非常短暂，比如就是保护一个reference counter，或者某一个flag，那么几个时钟周期以后就会离开临界区。
	//可是当这个thread还没有离开临界区之前，另外一个thread试图进入此临界区——这种情况只会发生在多核或者smp的系统上——发现无法进入，于是这个thread会进入睡眠，然后会发生一次上下文切换。
	//我们知道 context switch是一个比较耗时的操作，据说需要数千个时钟周期，那么其实我们只要再等多几个时钟周期就能够进入临界区，现在却多了数千个时钟周期的开销，真是是可忍孰不可忍。 
	//所以就引入了InitializeCriticalSectionAndSpinCount函数，它的第一个参数是指向cs的指针，第二个参数是旋转的次数。
	//我的理解就是一个循环次数，比如说N，那么就是说此时EnterCriticalSection()函数会内部循环判断此临界区是否可以进入，直到可以进入或者N次满。
	//我们增加的开销是最多N次循环，我们可能获得的红利是数千个时钟周期。对于临界区内很短的操作来讲，这样做的好处是大大的。 
	//MSDN上说，他们对于堆管理器使用了N=4000的旋转锁，然后“This gives great performance and scalability in almost all worst-case scenarios.” 可见还是很有用的:-)
	::InitializeCriticalSectionAndSpinCount(&m_cs, 4000);
}
CriticalSectionLock::~CriticalSectionLock(){
	::DeleteCriticalSection(&m_cs);
}
void CriticalSectionLock::lock(){
	::EnterCriticalSection(&m_cs);
}
void CriticalSectionLock::unlock(){
	::LeaveCriticalSection(&m_cs);
}
bool CriticalSectionLock::canCrossProcess() const{
	return false;
}
Lock::Type CriticalSectionLock::getType() const{
	return CRITICAL_SECTION;
}


