/*
* Copyright (c) 2025-present Wojciech Kaptur ( _Sqyd_ / Sqydev )
* Github: https://github.com/Sqydev
* GPG Fingerprint: 6DC2516B0DFDA9C59661650722F7B8A777F33B56
* 
* This software is provided "as-is", without any express or implied warranty. In no event
* will the authors be held liable for any damages arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose, including commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
* 
* 1. Non-Misrepresentation: The origin of this software must not be misrepresented; 
*    you must not claim that you wrote the original software. An acknowledgment in 
*    product documentation is appreciated but not required.
* 
* 2. Source-Level Copyleft: Any altered versions (forks) of this software's source code, 
*    or files containing significant portions of this code, must be distributed under 
*    these same license terms. Such modified source code must be made publicly available 
*    to any recipient, even if used over a network (SaaS).
* 
* 3. Proprietary Integration: This software may be integrated into, linked with, or 
*    used as a component of proprietary and closed-source products. In such cases, 
*    the surrounding proprietary application code does not need to be disclosed, 
*    provided that the original or modified source code of THIS software remains 
*    available under the terms of Section 2.
* 
* 4. Persistent Metadata: All original credits, including those in the source code headers 
*    and binary metadata (e.g., ELF .comment section, PE StringFileInfo, or equivalent), 
*    must not be removed. You may add your own credits to forks, provided the original 
*    authorship remains clearly identified.
* 
* 5. Notice Retention: This license notice may not be removed or altered from any 
*    source or binary distribution.
*/

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

// TODO: LIBCSITTYFNSINDEPENDENCE
// memset
void InitTui(int targetFps, TuiType type) {
	atexit(CloseTui);

	SignalsSetup();

	EnableRawMode();

	DATA.TuiData.type = type;

	DATA.TuiData.termdimm = GetTerminalDimensionsForReal();
	DATA.TuiData.termdimmInPixels = GetTerminalDimensionsInPixelsForReal();
    DATA.TuiData.lastTermIndex.x = DATA.TuiData.termdimm.x - 1;
    DATA.TuiData.lastTermIndex.y = DATA.TuiData.termdimm.y - 1;

	DATA.TuiData.tuidimm = DATA.TuiData.termdimm;
	DATA.TuiData.tuidimmInPixels = DATA.TuiData.termdimmInPixels;
    DATA.TuiData.lastTuiIndex.x = DATA.TuiData.termdimm.x - 1;
    DATA.TuiData.lastTuiIndex.y = DATA.TuiData.termdimm.y - 1;

	DATA.TuiData.cellsDimms = GetCellSizeInPixelsForReal();
	DATA.TuiData.cellsProp = GetCellProportionsForReal();

	DATA.TuiData.initiated = true;

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

	DATA.Cursor.pos = (Vector2i){ 0, 0 };

	SetTargetFps(targetFps);

	// Start chainging things
	UniWriteLen(UNI_WRITE_TARGET_STDOUT, "\033[?1049h\033[H\033[?7l");
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

	UniWriteLen(UNI_WRITE_TARGET_STDOUT, "\033[2J\033[?1049l\033[?7h");
}
// TODO: LIBCSITTYFNSINDEPENDENCE
// memcpy
void BeginDrawing(void) {
	SignalsStep();
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

void ClearTui(Color BgColor, Color FgColor) {
	for(size_t i = 0; i < GetBackbuffCellCount(); i++) {
		memset(DATA.Buffers.backbuff[i].Char, 0, 4 * sizeof(char));
		DATA.Buffers.backbuff[i].Char[0] = ' ';
		DATA.Buffers.backbuff[i].CharLen = 1;
		DATA.Buffers.backbuff[i].fgColor = FgColor;
		DATA.Buffers.backbuff[i].bgColor = BgColor;
	}
}
