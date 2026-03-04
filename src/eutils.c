#include "./private/coredata.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <time.h>
#elif defined(_WIN32) || defined(_WIN64)
#endif

#include <errno.h>

void ESleep(unsigned long sec, unsigned long ms, unsigned long ns) {
	struct timespec required, remaining;

	required.tv_sec = (time_t)(sec + (ms / 1000));

	unsigned long total_ns = (ms % 1000UL) * 1000000UL + ns;
	if (total_ns >= 1000000000UL) {
		required.tv_sec += (time_t)(total_ns / 1000000000UL);
		total_ns %= 1000000000UL;
	}
	required.tv_nsec = (long)total_ns;

	while(nanosleep(&required, &remaining) == -1) {
		if(errno == EINTR) {
			if(DATA.SignalData.SIG_INT.enabled && DATA.SignalData.SIG_INT.triggered) {
				break; 
			}
			required = remaining;
		}
		else {
			break;
		}
	}
}
