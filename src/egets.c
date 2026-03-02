#include "../include/esclib.h"

#include "./private/coredata.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <unistd.h>
	#include <sys/ioctl.h>
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
