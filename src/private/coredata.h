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

#ifndef ESCLIB_PRIVATE_COREDATA_H
#define ESCLIB_PRIVATE_COREDATA_H

#if defined(unix) || defined(__unix) || defined(__unix__)

	#include <termios.h>

#elif defined(_WIN32) || defined(__WIN64)
#endif

#include "../../include/esclib.h"

#include <stdbool.h>
#include <signal.h>

typedef enum {
	WAYLAND,
	X11,
	NONE,
	WINDOWS
} Compositor;

typedef struct {
	struct {
		SBCell* frontbuff;
		size_t frontbuffOffset;
		
		SBCell* backbuff;
		size_t backbuffOffset;

		char* charbuffer;
		size_t charbufferOffset;
	} Buffers;

	struct {
		Compositor compositor;
	} SystemInfo;

	struct {
		TuiType type;

		Vector2i lastTuiIndex;
		Vector2i tuidimm;
		Vector2i tuidimmInPixels;

		Vector2i lastTermIndex;
		Vector2i termdimm;
		Vector2i termdimmInPixels;

		Vector2i cellsDimms;
		Vector2i cellsProp;
		
		bool initiated; // TUI MUST DIE
		
#if defined(unix) || defined(__unix) || defined(__unix__)

		struct termios oldTerminalConfig;
		struct termios escTerminalConfig;

#elif defined(_WIN32) || defined(_WIN64)

		DWORD oldConsoleMode;
		HANDLE hStdin;

#endif
	} TuiData;

	struct {
		int targetFps;
		double target;

		double current;
		double previous;
		double delta;
	} Time;

	struct {
		bool typewriterMode;

		struct {
			int evdevIndex;

			bool* keyStates;
			bool* prevKeyStates;
		} Keyboard;
	} Input;

	struct {
		struct {
#if defined(unix) || defined(__unix) || defined(__unix__)

			struct sigaction old;

#elif defined(_WIN32) || defined(_WIN64)
#endif
			volatile sig_atomic_t triggered;
			bool enabled;
			bool enabledESClibTasks;
			bool enabledCustomTasks;
			bool enabledBuildInTasks;
			void (**customTasks)(void);
			int customTasksNumber;
		} SIG_INT;

		struct {
#if defined(unix) || defined(__unix) || defined(__unix__)

			struct sigaction old;

#elif defined(_WIN32) || defined(_WIN64)
#endif
			volatile sig_atomic_t triggered;
			bool enabled;
			bool enabledESClibTasks;
			bool enabledCustomTasks;
			bool enabledBuildInTasks;
			void (**customTasks)(void);
			int customTasksNumber;
		} SIG_WINCH;
	} SignalData;

	struct {
		Vector2i pos;
	} Cursor;
} CoreData;

extern CoreData DATA;

#endif
