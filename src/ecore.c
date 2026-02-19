#include <string.h>

#include "../include/esclib.h"

#include "./private/coredata.h"
#include "./private/signals/signals_actions.h"
#include "./private/common_utils.h"

#if defined(_WIN32) || defined(_WIN64)
#elif defined(__linux__) || defined(__APPLE__)
#endif

#include <stdio.h>
#include <stdlib.h>

CoreData DATA;

void InitTui(int targetFps) {
	atexit(CloseTui);

	SignalsSetup();

	DATA.TuiData.termdimm = GetTuiDimmentionsForReal();
	DATA.TuiData.termdimmInPixels = GetTuiDimmensionsInPixelsForReal();

	SetTargetFps(targetFps);

	// Start chainging things
	UniWrite(UNI_WRITE_TARGET_STDOUT, "\033[?1048h", 8);
}

void CloseTui(void) {
	printf("This is testing message from CloseTui() :)");
	fflush(stdout);
}

size_t GetBackbuffSize(void) {
	return (DATA.TuiData.termdimm.x * DATA.TuiData.termdimm.y) * sizeof(SBCell);
}

size_t GetBackbuffCellCount(void) {
	return DATA.TuiData.termdimm.x * DATA.TuiData.termdimm.y;
}

size_t WriteToBackbuff(const SBCell* content, size_t cellCount) {
	if(!content) { return 0; }

	size_t maxCells = GetBackbuffCellCount();

	if(DATA.Buffers.backbuffOffset >= maxCells) { return 0; }

	if(cellCount > maxCells - DATA.Buffers.backbuffOffset) {
		cellCount = maxCells - DATA.Buffers.backbuffOffset;
	}

	size_t bytesToWrite = cellCount * sizeof(SBCell);

	memcpy(DATA.Buffers.backbuff + DATA.Buffers.backbuffOffset, content, bytesToWrite);

	DATA.Buffers.backbuffOffset += cellCount;

	return bytesToWrite;
}
