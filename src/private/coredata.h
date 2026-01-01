#ifndef ESCLIB_PRIVATE_COREDATA_H
#define ESCLIB_PRIVATE_COREDATA_H

#if defined(__WIN32) || defined(__WIN64)
#elif defined(__linux__) || defined(__APPLE__)
	#include <signal.h>
#endif

typedef struct {
	struct {
		struct sigaction sigint;
		volatile sig_atomic_t sigintOn;
	} SignalData;
} CoreData;

#endif
