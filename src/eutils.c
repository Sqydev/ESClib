#include "./private/coredata.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <time.h>
	#include <wchar.h>
	#include <locale.h>

	extern int wcwidth (wchar_t __c);

#elif defined(_WIN32) || defined(_WIN64)
#endif

#include <errno.h>
#include <stdlib.h>

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

int GetCharWidth(const char* character) {
#if defined(unix) || defined(__unix) || defined(__unix__)

    static int locale_initialized = 0;
    if(!locale_initialized) {
        setlocale(LC_CTYPE, ""); 
        locale_initialized = 1;
    }

    wchar_t wc;
    int len = mbtowc(&wc, character, 4);
    
    if(len <= 0) return 0;

    int width = wcwidth(wc);
    
    return (width < 0) ? 0 : width;

#elif defined(_WIN32) || defined(_WIN64)
#endif
}
