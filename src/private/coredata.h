#ifndef ESCLIB_PRIVATE_COREDATA_H
#define ESCLIB_PRIVATE_COREDATA_H

#if defined(__WIN32) || defined(__WIN64)
#elif defined(__linux__) || defined(__APPLE__)
#endif

#include <signal.h>
#include <stdbool.h>

typedef struct {
	struct {
		struct {
			struct sigaction old;
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
