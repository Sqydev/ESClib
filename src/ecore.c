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
#include "./private/input/input.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
#elif defined(_WIN32) || defined(_WIN64)
#endif

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

CoreData DATA;

// TODO: LIBCSITTYFNSINDEPENDENCE
// memset
void InitTui(int targetFps, TuiType type) {
	atexit(CloseTui);

	SignalsSetup();

	EnableRawMode();

#if defined(unix) || defined(__unix) || defined(__unix__)

	if(getenv("WAYLAND_DISPLAY")) {
		DATA.System.compositor = WAYLAND;
		
		TraceLog("[ESCLIB]: INFO: Detected Wayland");
	}
	else if(getenv("DISPLAY")) {
		DATA.System.compositor = X11;
		
		TraceLog("[ESCLIB]: INFO: Detected X11");
	}
	else {
		DATA.System.compositor = NONE;
		
		TraceLog("[ESCLIB]: INFO: Detected NO COMPOSITOR :0");
	}

#elif defined(_WIN32) || defined(_WIN64)

	DATA.SystemInfo.compositor = WINDOWS;	
	TraceLog("[ESCLIB]: INFO: WINDOWS >:(");

#endif

	InitInput();

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

	// Before InitOpenCl we set thoes two to make sure anything wierd isn't happening behind the scenes
	DATA.System.computeBackend.backend = COMPUTE_ESC;
	DATA.System.computeBackend.device = DEVICE_CPU;
	InitOpenCl();
}

void CloseTui(void) {
	if(!DATA.TuiData.initiated) {
		return;
	}

	CleanUpOpenCl();

	DATA.TuiData.initiated = false;
	
	free(DATA.Buffers.backbuff);
	DATA.Buffers.backbuff = NULL;

	free(DATA.Buffers.frontbuff);
	DATA.Buffers.frontbuff = NULL;

	free(DATA.Buffers.charbuffer);
	DATA.Buffers.charbuffer = NULL;

	if(DATA.Logging.enabled) { CloseInput(); }

	DisableRawMode();

	SignalsCleanup();

	if(DATA.Logging.file) { CloseLoggin(); }

	UniWriteLen(UNI_WRITE_TARGET_STDOUT, "\033[2J\033[?1049l\033[?7h");
}

// NOTE: In here becouse I have no idea where else put it :)
void (**panicTasks)(void) = NULL;
size_t panicTasksCount = 0;

void Panic(const char* message, int exitCode) {
	UniWriteLen(UNI_WRITE_TARGET_STDERR, message);

	for(size_t i = 0; i < panicTasksCount; i++) {
		if(panicTasks[i] != NULL) { panicTasks[i](); }
	}

	CloseTui();
	exit(exitCode);
}

int AddPanicTask(void (*task)(void)) {
	void (**tmp)(void) = realloc(panicTasks, sizeof(void (*)) * (panicTasksCount + 1));
	if(!tmp) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: Panic Tasks realloc failed somehow\n");
		errno = ENOMEM;
		return -1;
	}

	panicTasks = tmp;
	panicTasks[panicTasksCount] = task;
	panicTasksCount++;

	return panicTasksCount - 1;
}

int RemovePanicTask(size_t index) {
	if(index > panicTasksCount) {
		errno = EINVAL;
		return -1;
	}

	panicTasks[index] = NULL;

	while(panicTasksCount > 0 && panicTasks[panicTasksCount - 1] == NULL){
		void (**tmp)(void) = realloc(panicTasks, sizeof(void (*)) * (panicTasksCount - 1));
		if(!tmp) {
			UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: Panic Tasks realloc failed somehow\n");
			errno = ENOMEM;
			return -1;
		}

		panicTasksCount--;
	}

	return 0;
}

int InitLoggin(char* path) {
	if(DATA.Logging.enabled) { return -2; }

	DATA.Logging.enabled = true;
	DATA.Logging.path = path;

	DATA.Logging.file = fopen(path, "a");
	if(!DATA.Logging.file) {
		DATA.Logging.enabled = false;

		return -1;
	}

	return 0;
}

void TraceLog(const char* message, ...) {
	if(!DATA.Logging.enabled) { return; }
	if(!DATA.Logging.file) { return; }

	va_list va;
	va_start(va, message);

	vfprintf(DATA.Logging.file, message, va);
	fprintf(DATA.Logging.file, "\n");

	va_end(va);

	fflush(DATA.Logging.file);
}

void CloseLoggin() {
	if(!DATA.Logging.enabled) { return; }

	DATA.Logging.enabled = false;

	if(DATA.Logging.file) { fclose(DATA.Logging.file); }
}

// TODO: LIBCSITTYFNSINDEPENDENCE
// memcpy
void BeginDrawing(void) {
	SignalsStep();

	InputStep();

	DATA.Time.previous = GetTime();
}

// TODO: Make here better error handling
void EndDrawing(void) {
	RenderFrame();

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

			DATA.Time.current = GetTime();
			DATA.Time.delta = DATA.Time.current - DATA.Time.previous;
			DATA.Time.previous = DATA.Time.current;
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
