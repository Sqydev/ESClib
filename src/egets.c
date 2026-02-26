#include "../include/esclib.h"

#include "./private/coredata.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <unistd.h>
	#include <sys/ioctl.h>
#elif defined(__WIN32) || defined(__WIN64)
#endif

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
