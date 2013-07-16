#ifndef _LUCID_TIMER_H_
#define _LUCID_TIMER_H_

#include "lcSingleton.h"
#include "lcUnderiable.h"

#ifdef LC_CMPL_MSVC
#include <windows.h>
#endif

namespace lc{

class timer : public core::Singleton<timer>, public virtual Underiable<timer> {
private:
#ifdef LC_CMPL_MSVC
	BOOL m_bHighAccurateSupport;
	bool m_bMultiCore;
	LARGE_INTEGER m_performanceFreq;
#endif
	timer();
	friend class core::Singleton<timer>;
public:
	u32 getTime() const;
};


}
#endif