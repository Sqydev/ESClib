#include "../include/esclib.h"

#include "./private/coredata.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <unistd.h>
	#include <sys/ioctl.h>
	#include <time.h>
#elif defined(_WIN32) || defined(_WIN64)
#endif

#include <errno.h>

Vector2i GetTuiDimmensions(void) { return DATA.TuiData.termdimm; }
Vector2i GetTuiDimmensionsInPixels(void) { return DATA.TuiData.termdimmInPixels; }
Vector2i* GetTuiDimmensionsPtr(void) { return &DATA.TuiData.termdimm; }
Vector2i* GetTuiDimmensionsPtrInPixels(void) { return &DATA.TuiData.termdimmInPixels; }

Vector2i GetTuiDimmentionsForReal(void) {
#if defined(unix) || defined(__unix) || defined(__unix__)

	struct winsize termdimm;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &termdimm);

	return (Vector2i){termdimm.ws_row, termdimm.ws_col};

#elif defined(_WIN32) || defined(_WIN64)
#endif
}

Vector2i GetTuiDimmensionsInPixelsForReal(void) {
#if defined(unix) || defined(__unix) || defined(__unix__)

	struct winsize termdimm;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &termdimm);

	return (Vector2i){termdimm.ws_xpixel, termdimm.ws_ypixel};

#elif defined(_WIN32) || defined(_WIN64)	
#endif
}

size_t GetBackbuffSize(void) {
	return GetBackbuffCellCount() * sizeof(SBCell);
}

size_t GetBackbuffCellCount(void) {
	return DATA.TuiData.termdimm.x * DATA.TuiData.termdimm.y;
}

size_t GetCharbuffSize(void) {
	return GetCharbuffCellCount() * 64 + 1000;
}

size_t GetCharbuffCellCount(void) {
	return DATA.TuiData.termdimm.x * DATA.TuiData.termdimm.y;
}

void (**GetCustomSignalTasks(int signal))(void) {
	switch(signal) {
		case SIGINT: {
			return DATA.SignalData.SIG_INT.customTasks;
		}

		case SIGWINCH: {
			return DATA.SignalData.SIG_WINCH.customTasks;
		}

		default: {
			errno = EINVAL;
			return NULL;
		}
	}
}

int GetCustomSignalTasksCount(int signal) {
	switch(signal) {
		case SIGINT: {
			return DATA.SignalData.SIG_INT.customTasksNumber;
		}

		case SIGWINCH: {
			return DATA.SignalData.SIG_WINCH.customTasksNumber;
		}

		default: {
			errno = EINVAL;
			return -1;
		}
	}
}

size_t GetCustomSignalTasksSize(int signal) {
	switch(signal) {
		case SIGINT: {
			return sizeof(DATA.SignalData.SIG_INT.customTasks) * DATA.SignalData.SIG_INT.customTasksNumber;
		}

		case SIGWINCH: {
			return sizeof(DATA.SignalData.SIG_WINCH.customTasks) * DATA.SignalData.SIG_WINCH.customTasksNumber;
		}

		default: {
			errno = EINVAL;
			return -1;
		}
	}
}

double GetTime(void) {
#if defined(__APPLE__) || defined(__linux__)
	
	static struct timespec start = {0, 0};
	static int initialized = 0;

	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);

	if (!initialized) {
		start = now;
		initialized = 1;
	}

	double sec = (double)(now.tv_sec  - start.tv_sec);
	double nsec = (double)(now.tv_nsec - start.tv_nsec) / 1e9;
	return sec + nsec;

#elif defined(_WIN32) || defined(_WIN64)
	
	static LARGE_INTEGER frequency = {0};
	static LARGE_INTEGER start = {0};
	static int initialized = 0;

	if (!initialized) {
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);
		initialized = 1;
	}

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);

	LONGLONG elapsed = now.QuadPart - start.QuadPart;
	return (double)elapsed / (double)frequency.QuadPart;

#endif
}
