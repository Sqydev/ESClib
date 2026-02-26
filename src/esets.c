#include "../include/esclib.h"

#include "./private/coredata.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
#elif defined(__WIN32) || defined(__WIN64)
#endif

void SetTargetFps(int targetFps) {
	DATA.Time.targetFps = targetFps;
}
