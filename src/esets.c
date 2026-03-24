#include "../include/esclib.h"

#include "./private/coredata.h"

#include <stdlib.h>
#include <string.h>

#if defined(unix) || defined(__unix) || defined(__unix__)
#elif defined(_WIN32) || defined(_WIN64)
#endif

void SetTargetFps(int targetFps) {
	if(targetFps < 0) {
		DATA.Time.targetFps = 0;
		DATA.Time.target = 0.0;
	}
	else {
		DATA.Time.targetFps = targetFps;
		DATA.Time.target = 1.0 / (double)targetFps;
	}
}

void SetTuiDimentions(int x, int y) {
	DATA.TuiData.tuidimm.x = x;
	DATA.TuiData.tuidimm.y = y;

    DATA.TuiData.lastTuiIndex.x = DATA.TuiData.tuidimm.x - 1;
    DATA.TuiData.lastTuiIndex.y = DATA.TuiData.tuidimm.y - 1;
	
	DATA.Buffers.backbuff = NULL;
	DATA.Buffers.backbuff = realloc(DATA.Buffers.backbuff, GetBackbuffSize());
	DATA.Buffers.backbuffOffset = 0;
	memset(DATA.Buffers.backbuff, 0, GetBackbuffSize());

	DATA.Buffers.frontbuff = NULL;
	DATA.Buffers.frontbuff = realloc(DATA.Buffers.frontbuff, GetBackbuffSize());
	DATA.Buffers.frontbuffOffset = 0;
	memset(DATA.Buffers.frontbuff, 0, GetBackbuffSize());

	DATA.Buffers.charbuffer = NULL;
	DATA.Buffers.charbuffer = realloc(DATA.Buffers.charbuffer, GetCharbuffSize());
}
