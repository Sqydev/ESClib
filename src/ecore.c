#include "../include/esclib.h"
#include "private/coredata.h"
#include "private/utils.h"

#include <stdlib.h>

// NOTE: Create CoreData(Defined in ./private/coredata.h)
CoreData DATA;

void InitTui(int width, int height, int targetFps, TuiType tuiType) {
	atexit(CloseTui);

	SignalsSetup(&DATA);

	(void)width;
	(void)height;
	(void)targetFps;
	(void)tuiType;
}

void CloseTui(void) {
}
