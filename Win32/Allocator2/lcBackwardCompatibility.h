#ifndef _LUCID_BACKWARDCOMPATIBILITY_H_
#define _LUCID_BACKWARDCOMPATIBILITY_H_

#include "lcObsolete.h"

namespace lc{

template<>
class Obsolete<MemoryTracer>
{
public:
	LC_OBSOLETE(" is deprecated")
	bool testDeprecated1() const;

	
	LC_OBSOLETE(" is deprecated")
	static bool testDeprecated2(const char* str);
};
}

#endif