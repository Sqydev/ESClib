#include "../include/esclib.h"

#include "./private/coredata.h"
#include "./private/signals/signals_actions.h"
#include "./private/common_utils.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
#elif defined(_WIN32) || defined(_WIN64)
#endif

#include <stdlib.h>
#include <string.h>

CoreData DATA;

void InitTui(int targetFps) {
	atexit(CloseTui);

	SignalsSetup();

	DATA.TuiData.termdimm = GetTuiDimmentionsForReal();
	DATA.TuiData.termdimmInPixels = GetTuiDimmensionsInPixelsForReal();

	DATA.TuiData.closed = false;

	DATA.Buffers.backbuff = NULL;
	DATA.Buffers.backbuff = realloc(DATA.Buffers.backbuff, GetBackbuffSize());
	DATA.Buffers.frontbuff = NULL;
	DATA.Buffers.frontbuff = realloc(DATA.Buffers.frontbuff, GetBackbuffSize());

	SetTargetFps(targetFps);

	// Start chainging things
	UniWriteLen(UNI_WRITE_TARGET_STDOUT, "\033[?1049h");
}

void CloseTui(void) {
	if(DATA.TuiData.closed) {
		return;
	}
	
	DATA.TuiData.closed = true;
	
	UniWriteLen(UNI_WRITE_TARGET_STDOUT, "Greetings from CloseTui()!\n");

	free(DATA.Buffers.backbuff);
	DATA.Buffers.backbuff = NULL;

	free(DATA.Buffers.frontbuff);
	DATA.Buffers.frontbuff = NULL;

	SignalsCleanup();

	UniWriteLen(UNI_WRITE_TARGET_STDOUT, "\033[?1049l");
}

