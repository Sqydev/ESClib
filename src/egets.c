#include "../include/esclib.h"

#include "./private/coredata.h"

#if defined(_WIN32) || defined(_WIN64)
#elif defined(__linux__) || defined(__APPLE__)
	#include <unistd.h>
	#include <sys/ioctl.h>
#endif

Vector2i GetTuiDimmensions(void) { return DATA.TuiData.termdimm; }
Vector2i GetTuiDimmensionsInPixels(void) { return DATA.TuiData.termdimmInPixels; }
Vector2i* GetTuiDimmensionsPtr(void) { return &DATA.TuiData.termdimm; }
Vector2i* GetTuiDimmensionsPtrInPixels(void) { return &DATA.TuiData.termdimmInPixels; }

Vector2i GetTuiDimmentionsForReal(void) {
#if defined(_WIN32) || defined(_WIN64)	
#elif defined(__linux__) || defined(__APPLE__)

	struct winsize termdimm;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &termdimm);

	return (Vector2i){termdimm.ws_row, termdimm.ws_col};

#endif
}

Vector2i GetTuiDimmensionsInPixelsForReal(void) {
#if defined(_WIN32) || defined(_WIN64)
#elif defined(__linux__) || defined(__APPLE__)

	struct winsize termdimm;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &termdimm);

	return (Vector2i){termdimm.ws_xpixel, termdimm.ws_ypixel};

#endif
}
