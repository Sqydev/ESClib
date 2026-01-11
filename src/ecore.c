#include "../include/esclib.h"

#include "./private/coredata.h"
#include "./private/signals/signals_actions.h"

#if defined(_WIN32) || defined(_WIN64)
#elif defined(__linux__) || defined(__APPLE__)
#endif

#include <stdio.h>
#include <stdlib.h>

// NOTE: Create CoreData(Defined in ./private/coredata.h)
CoreData DATA;

void InitTui(int width, int height, int targetFps, TuiType tuiType) {
	atexit(CloseTui);

	SignalsSetup();

	(void)width;
	(void)height;
	(void)targetFps;
	(void)tuiType;
}

void CloseTui(void) {
	printf("This is testing message from CloseTui() :)");
	fflush(stdout);
}
