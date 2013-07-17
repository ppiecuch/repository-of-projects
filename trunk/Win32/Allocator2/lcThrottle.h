#ifndef _LUCID_THROTTLE_H_
#define _LUCID_THROTTLE_H_

namespace lc{

class Throttle{
	u32 m_uThreshold;
public:
	Throttle(u32 threshold);

	void setThreshold(u32 threshold);

	void onTransfer(u32 size, u32 time);
};
}
#endif