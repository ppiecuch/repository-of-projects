#ifndef _LUCID_THROTTLE_H_
#define _LUCID_THROTTLE_H_

#include "lcTypes.h"
#include "lcMutex.h"

namespace lc{

class Throttle{
	u32 m_uThreshold;
	u32 m_uTransfered;
	u32 m_uStartTime;
	lc::mutex m_mutex;
public:
	Throttle(u32 threshold);

	void setThreshold(u32 threshold);

	void onTransfer(u32 size);
};
}
#endif