#include "lcBackwardCompatibility.h"


namespace lc{

	bool Obsolete<MemoryTracer>::testDeprecated1() const{return false;}

	bool Obsolete<MemoryTracer>::testDeprecated2(const char* str){return false;}

}
