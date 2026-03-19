#include "../include/esclib.h"

#include "./private/coredata.h"
#include "./private/signals/signals_actions.h"
#include "./private/common_utils.h"
#include "./private/renderFrame.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
#elif defined(_WIN32) || defined(_WIN64)
#endif

#include <stdlib.h>
#include <string.h>

CoreData DATA;

void InitTui(int targetFps) {
	atexit(CloseTui);

	SignalsSetup();

	EnableRawMode();

	DATA.TuiData.termdimm = GetTuiDimmentionsForReal();
	DATA.TuiData.termdimmInPixels = GetTuiDimmensionsInPixelsForReal();

	DATA.TuiData.initiated = true;

	DATA.Buffers.backbuff = NULL;
	DATA.Buffers.backbuff = realloc(DATA.Buffers.backbuff, GetBackbuffSize());
	DATA.Buffers.backbuffOffset = 0;
	DATA.Buffers.frontbuff = NULL;
	DATA.Buffers.frontbuff = realloc(DATA.Buffers.frontbuff, GetBackbuffSize());
	DATA.Buffers.frontbuffOffset = 0;

	DATA.Buffers.charbuffer = NULL;
	DATA.Buffers.charbuffer = realloc(DATA.Buffers.charbuffer, GetCharbuffSize());

	SetTargetFps(targetFps);

	// Start chainging things
	UniWriteLen(UNI_WRITE_TARGET_STDOUT, "\033[?1049h");
}

void CloseTui(void) {
	if(!DATA.TuiData.initiated) {
		return;
	}
	
	DATA.TuiData.initiated = false;
	
	free(DATA.Buffers.backbuff);
	DATA.Buffers.backbuff = NULL;

	free(DATA.Buffers.frontbuff);
	DATA.Buffers.frontbuff = NULL;

	free(DATA.Buffers.charbuffer);
	DATA.Buffers.charbuffer = NULL;

	DisableRawMode();

	SignalsCleanup();

	UniWriteLen(UNI_WRITE_TARGET_STDOUT, "\033[?1049l");
}

void BeginDrawing(void) {
	SignalsStep();

	memcpy(DATA.Buffers.frontbuff, DATA.Buffers.backbuff, GetBackbuffSize());

	DATA.Buffers.backbuffOffset = 0;
	DATA.Buffers.frontbuffOffset = 0;
}

// TODO: Make here better error handling
void EndDrawing(void) {
	RenderFrame();

	DATA.Time.previous = DATA.Time.current;
	DATA.Time.current = GetTime();
	DATA.Time.delta = DATA.Time.current - DATA.Time.previous;

	if(DATA.Time.target > 0.0) {
		double timeForFuckingAround = DATA.Time.target - DATA.Time.delta;

		if(timeForFuckingAround > 0.0) {
#if defined(unix) || defined(__unix) || defined(__unix__)

			unsigned long s = (unsigned long)timeForFuckingAround;
			unsigned long ns = (unsigned long)((timeForFuckingAround - (double)s) * 1e9);
			ESleep(s, 0UL, ns);

#elif defined(_WIN32) || defined(_WIN64)

			double ms_d = timeForFuckingAround * 1000.0 + 0.5;
			DWORD ms = (ms_d >= (double)MAXDWORD) ? MAXDWORD : (DWORD)ms_d;
			if(ms > 0) Sleep(ms);

#endif

			DATA.Time.previous = DATA.Time.current;
			DATA.Time.current = GetTime();
			DATA.Time.delta = DATA.Time.current - DATA.Time.previous;
		}
	}
}

void ClearBackground(Color BgColor, Color FgColor) {
	for(size_t i = 0; i < GetBackbuffCellCount(); i++) {
		memset(DATA.Buffers.backbuff[i].Char, ' ', 4 * sizeof(char));
		DATA.Buffers.backbuff[i].CharLen = 1;
		DATA.Buffers.backbuff[i].fgColor = FgColor;
		DATA.Buffers.backbuff[i].bgColor = BgColor;
	}
}
