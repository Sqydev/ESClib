#include "./private/coredata.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <time.h>
#elif defined(_WIN32) || defined(_WIN64)
#endif

#include <errno.h>

void ESleep(int sec, int ms, long ns) {
    struct timespec required, remaining;

    required.tv_sec = sec + (ms / 1000);
    required.tv_nsec = (long)(ms % 1000) * 1000000L + ns;

    if(required.tv_nsec >= 1000000000L) {
        required.tv_sec += required.tv_nsec / 1000000000L;
        required.tv_nsec = required.tv_nsec % 1000000000L;
    }

    while(nanosleep(&required, &remaining) == -1) {
        if(errno == EINTR) {
            if(DATA.SignalData.SIG_INT.enabled && DATA.SignalData.SIG_INT.triggered) {
                break; 
            }
            required = remaining;
        } else {
            break;
        }
    }
}
