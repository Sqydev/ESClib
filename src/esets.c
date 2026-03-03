#include "../include/esclib.h"

#include "./private/coredata.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
#elif defined(_WIN32) || defined(_WIN64)
#endif

void SetTargetFps(int targetFps) {
	if(targetFps < 1) {
		DATA.Time.targetFps = 0;
		DATA.Time.target = 0.0;
	}
	else {
		DATA.Time.targetFps = targetFps;
		DATA.Time.target = 1.0 / (double)targetFps;
	}
}
