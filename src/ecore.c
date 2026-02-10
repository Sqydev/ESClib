#include <string.h>

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

static inline size_t GetBackbuffSize() {
	return 0;
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
