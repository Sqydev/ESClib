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

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <unistd.h>
	#include <sys/ioctl.h>
	#include <time.h>
#elif defined(_WIN32) || defined(_WIN64)
#endif

#include <errno.h>

Vector2i GetTuiDimensions(void) { return DATA.TuiData.tuidimm; }
Vector2i GetTuiDimensionsInPixels(void) { return DATA.TuiData.tuidimmInPixels; }
Vector2i* GetTuiDimensionsPtr(void) { return &DATA.TuiData.tuidimm; }
Vector2i* GetTuiDimensionsPtrInPixels(void) { return &DATA.TuiData.tuidimmInPixels; }

Vector2i GetLastTuiIndex(void) { return DATA.TuiData.lastTuiIndex; }
Vector2i* GetLastTuiIndexPtr(void) { return &DATA.TuiData.lastTuiIndex; }

Vector2i GetTerminalDimensions(void) { return DATA.TuiData.termdimm; }
Vector2i GetTerminalDimensionsInPixels(void) { return DATA.TuiData.termdimmInPixels; }
Vector2i* GetTerminalDimensionsPtr(void) { return &DATA.TuiData.termdimm; }
Vector2i* GetTerminalDimensionsPtrInPixels(void) { return &DATA.TuiData.termdimmInPixels; }

Vector2i GetLastTerminalIndex(void) { return DATA.TuiData.lastTermIndex; }
Vector2i* GetLastTerminalIndexPtr(void) { return &DATA.TuiData.lastTermIndex; }

Vector2i GetTerminalDimensionsForReal(void) {
#if defined(unix) || defined(__unix) || defined(__unix__)

	struct winsize termdimm;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &termdimm);

	return (Vector2i){termdimm.ws_col, termdimm.ws_row};

#elif defined(_WIN32) || defined(_WIN64)
#endif
}

Vector2i GetTerminalDimensionsInPixelsForReal(void) {
#if defined(unix) || defined(__unix) || defined(__unix__)

	struct winsize termdimm;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &termdimm);

	return (Vector2i){termdimm.ws_xpixel, termdimm.ws_ypixel};

#elif defined(_WIN32) || defined(_WIN64)	
#endif
}

size_t GetBackbuffSize(void) {
	return GetBackbuffCellCount() * sizeof(SBCell);
}

size_t GetBackbuffCellCount(void) {
	return DATA.TuiData.tuidimm.x * DATA.TuiData.tuidimm.y;
}

size_t GetCharbuffSize(void) {
	return GetCharbuffCellCount() * 64 + 1000;
}

size_t GetCharbuffCellCount(void) {
	return DATA.TuiData.tuidimm.x * DATA.TuiData.tuidimm.y;
}

void (**GetCustomSignalTasks(int signal))(void) {
	switch(signal) {
		case SIGINT: {
			return DATA.SignalData.SIG_INT.customTasks;
		}

		case SIGWINCH: {
			return DATA.SignalData.SIG_WINCH.customTasks;
		}

		default: {
			errno = EINVAL;
			return NULL;
		}
	}
}

int GetCustomSignalTasksCount(int signal) {
	switch(signal) {
		case SIGINT: {
			return DATA.SignalData.SIG_INT.customTasksNumber;
		}

		case SIGWINCH: {
			return DATA.SignalData.SIG_WINCH.customTasksNumber;
		}

		default: {
			errno = EINVAL;
			return -1;
		}
	}
}

size_t GetCustomSignalTasksSize(int signal) {
	switch(signal) {
		case SIGINT: {
			return sizeof(DATA.SignalData.SIG_INT.customTasks) * DATA.SignalData.SIG_INT.customTasksNumber;
		}

		case SIGWINCH: {
			return sizeof(DATA.SignalData.SIG_WINCH.customTasks) * DATA.SignalData.SIG_WINCH.customTasksNumber;
		}

		default: {
			errno = EINVAL;
			return -1;
		}
	}
}

double GetTime(void) {
#if defined(__APPLE__) || defined(__linux__)
	
	static struct timespec start = {0, 0};
	static int initialized = 0;

	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);

	if (!initialized) {
		start = now;
		initialized = 1;
	}

	double sec = (double)(now.tv_sec  - start.tv_sec);
	double nsec = (double)(now.tv_nsec - start.tv_nsec) / 1e9;
	return sec + nsec;

#elif defined(_WIN32) || defined(_WIN64)
	
	static LARGE_INTEGER frequency = {0};
	static LARGE_INTEGER start = {0};
	static int initialized = 0;

	if (!initialized) {
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);
		initialized = 1;
	}

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);

	LONGLONG elapsed = now.QuadPart - start.QuadPart;
	return (double)elapsed / (double)frequency.QuadPart;

#endif
}
