#ifndef ESCLIB_PRIVATE_COREDATA_H
#define ESCLIB_PRIVATE_COREDATA_H

#if defined(unix) || defined(__unix) || defined(__unix__)
#elif defined(__WIN32) || defined(__WIN64)
#endif

#include "../../include/esclib.h"

#include <stdbool.h>
#include <signal.h>

typedef struct {
	struct {
		SBCell* frontbuff;
		
		SBCell* backbuff;
		size_t backbuffOffset;
	} Buffers;

	struct {
		Vector2i termdimm;
		Vector2i termdimmInPixels;
	} TuiData;

	struct {
		int targetFps;
	} Time;

	struct {
		struct {
#if defined(unix) || defined(__unix) || defined(__unix__)

			struct sigaction old;

#elif defined(__WIN32) || defined(__WIN64)
#endif
			volatile sig_atomic_t triggered;
			bool enabled;
			bool enabledESClibTasks;
			bool enabledCustomTasks;
			bool enabledBuildInTasks;
		} SIG_INT;
	} SignalData;
} CoreData;

extern CoreData DATA;

#endif
