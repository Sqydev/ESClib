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
	return DATA.TuiData.termdimm.x * DATA.TuiData.termdimm.y;
}

size_t WriteToBackbuff(const char* content) {
	size_t len = strlen(DATA.backbuff);

	if(len + 1/* \0 */ >= GetBackbuffSize()) { return 0; }
	if(!content) { return 0; }

	size_t freeSpace = GetBackbuffSize() - len - 1; // -1 == \0
	size_t content_len = strlen(content);

	if(content_len > freeSpace) {
		content_len = freeSpace;
	}

	memcpy(DATA.backbuff + len, content, content_len);
	DATA.backbuff[len + content_len] = '\0';

	return content_len;
}
