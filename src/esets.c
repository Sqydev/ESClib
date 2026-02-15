#include "../include/esclib.h"

#include "./private/coredata.h"

#if defined(_WIN32) || defined(_WIN64)
#elif defined(__linux__) || defined(__APPLE__)
#endif

void SetTargetFps(int targetFps) {
	DATA.Time.targetFps = targetFps;
}
