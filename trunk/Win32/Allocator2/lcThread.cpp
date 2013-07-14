#include "lcThread.h"
#include "lcNewAndDelete.h"
#include "lcThreadWindow.h"

namespace lc{

	thread* thread::createInstance(IRunnable* r){
		return LC_NEW TheadWindow(r);
	}
}