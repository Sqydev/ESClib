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

#include "../../../include/esclib.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#endif

#include "./signals_actions.h"
#include "./handlers.h"
#include "../coredata.h"
#include "../common_utils.h"

#include <signal.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

void SignalFatal(const char* msg) {

#if defined(unix) || defined(__unix) || defined(__unix__)

    write(STDERR_FILENO, msg, strlen(msg));

#elif defined(_WIN32) || defined(_WIN64)

	UniWrite(UNI_WRITE_TARGET_STDERR, msg, strlen(msg));

#endif
    exit(EXIT_FAILURE);
}

#if defined(unix) || defined(__unix) || defined(__unix__)

void Install(int sig, struct sigaction* old, void (*handler)(int)) {
    struct sigaction sa = {0};
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);

    if (sigaction(sig, NULL, old) == -1) {
        SignalFatal("ERROR: Sigaction has messed up save\n");
	}

    if (sigaction(sig, &sa, NULL) == -1) {
        SignalFatal("ERROR: Sigaction has messed up install\n");
	}
}

#elif defined(_WIN32) || defined(_WIN64)

BOOL WINAPI ConsoleHandler(DWORD type) {
    switch (type) {
        case CTRL_C_EVENT: {
            DATA.SignalData.SIG_INT.triggered = 1;
            return TRUE;
		}
	
        case WINDOW_BUFFER_SIZE_EVENT: {
            DATA.SignalData.SIG_WINCH.triggered = 1;
            return TRUE;
		}

	    default: {
            return FALSE;
		}
    }
}

#endif

void SignalsSetup(void) {
    DATA.SignalData.SIG_INT.triggered = 0;
    DATA.SignalData.SIG_INT.enabled = 1;
    DATA.SignalData.SIG_INT.enabledESClibTasks = 1;
    DATA.SignalData.SIG_INT.enabledCustomTasks = 1;
	DATA.SignalData.SIG_INT.enabledBuildInTasks = 1;
	DATA.SignalData.SIG_INT.customTasksNumber = 0;
	DATA.SignalData.SIG_INT.customTasks = NULL;

    DATA.SignalData.SIG_WINCH.triggered = 0;
    DATA.SignalData.SIG_WINCH.enabled = 1;
    DATA.SignalData.SIG_WINCH.enabledESClibTasks = 1;
    DATA.SignalData.SIG_WINCH.enabledCustomTasks = 1;
	DATA.SignalData.SIG_WINCH.enabledBuildInTasks = 1;
	DATA.SignalData.SIG_WINCH.customTasksNumber = 0;
	DATA.SignalData.SIG_WINCH.customTasks = NULL;

#if defined(unix) || defined(__unix) || defined(__unix__)

    Install(SIGINT, &DATA.SignalData.SIG_INT.old, sigint_handler);

    Install(SIGWINCH, &DATA.SignalData.SIG_WINCH.old, sigwinch_handler);

#elif defined(_WIN32) || defined(_WIN64)

	SetConsoleCtrlHandler(ConsoleHandler, TRUE);

#endif
}

void SignalsStep(void) {
#if defined(unix) || defined(__unix) || defined(__unix__)

    // NOTE: SIGINT
	
    if (DATA.SignalData.SIG_INT.triggered == 1 && DATA.SignalData.SIG_INT.enabled == true) {
        DATA.SignalData.SIG_INT.triggered = 0;

        // NOTE: Retrive the old functions of SIGINT
        if (sigaction(SIGINT, &DATA.SignalData.SIG_INT.old, NULL) == -1) {
            write(STDERR_FILENO,
                  "ERROR: Signalaction messed up SIGINT state retreat!\n",
                  sizeof("ERROR: Signalaction messed up SIGINT state retreat!\n") - 1);
            exit(EXIT_FAILURE);
        }

		if(DATA.SignalData.SIG_INT.enabledCustomTasks) {
			for(int i = 0; i < DATA.SignalData.SIG_INT.customTasksNumber; i++) {
				if(DATA.SignalData.SIG_INT.customTasks[i] != NULL) {
					DATA.SignalData.SIG_INT.customTasks[i]();
				}
			}
		}

        if (DATA.SignalData.SIG_INT.enabledESClibTasks) {
            CloseTui();
        }

        if (DATA.SignalData.SIG_INT.enabledBuildInTasks) {
			raise(SIGINT);
		}
    }

    // NOTE: SIGWINCH
	
    if (DATA.SignalData.SIG_WINCH.triggered == 1 && DATA.SignalData.SIG_WINCH.enabled == true) {
        DATA.SignalData.SIG_WINCH.triggered = 0;

		if(DATA.SignalData.SIG_WINCH.enabledCustomTasks) {
			for(int i = 0; i < DATA.SignalData.SIG_WINCH.customTasksNumber; i++) {
				if(DATA.SignalData.SIG_WINCH.customTasks[i] != NULL) {
					DATA.SignalData.SIG_WINCH.customTasks[i]();
				}
			}
		}

        if (DATA.SignalData.SIG_WINCH.enabledESClibTasks) {
        	DATA.TuiData.termdimm = GetTerminalDimensionsForReal();
        	DATA.TuiData.termdimmInPixels = GetTerminalDimensionsInPixelsForReal();
        	DATA.TuiData.lastTermIndex.x = DATA.TuiData.termdimm.x - 1;
        	DATA.TuiData.lastTermIndex.y = DATA.TuiData.termdimm.y - 1;

			if(DATA.TuiData.type != TUI_STATIC) {
        		DATA.TuiData.tuidimm = DATA.TuiData.termdimm;
	        	DATA.TuiData.tuidimmInPixels = DATA.TuiData.termdimmInPixels;
    	    	DATA.TuiData.lastTuiIndex.x = DATA.TuiData.tuidimm.x - 1;
        		DATA.TuiData.lastTuiIndex.y = DATA.TuiData.tuidimm.y - 1;

				DATA.TuiData.cellsDimms = GetCellSizeInPixelsForReal();
				DATA.TuiData.cellsProp = GetCellProportionsForReal();

				DATA.Buffers.backbuff = realloc(DATA.Buffers.backbuff, GetBackbuffSize());
				DATA.Buffers.frontbuff = realloc(DATA.Buffers.frontbuff, GetBackbuffSize());
	
				DATA.Buffers.charbuffer = realloc(DATA.Buffers.charbuffer, GetCharbuffSize());
			}
	
			memset(DATA.Buffers.backbuff, 0, GetBackbuffSize());
			memset(DATA.Buffers.frontbuff, 0, GetBackbuffSize());
	
			UniWriteLen(UNI_WRITE_TARGET_STDOUT, "\033[0m\033[2J");
		}

        if (DATA.SignalData.SIG_WINCH.enabledBuildInTasks) {
        	if(sigaction(SIGWINCH, &DATA.SignalData.SIG_WINCH.old, NULL)) {
            	write(STDERR_FILENO,
                  "ERROR: Signalaction messed up SIGWINCH state retreat!\n",
                  sizeof("ERROR: Signalaction messed up SIGWINCH state retreat!\n") - 1);
            	exit(EXIT_FAILURE);
			}

        	if (DATA.SignalData.SIG_WINCH.old.sa_handler != SIG_DFL && DATA.SignalData.SIG_WINCH.old.sa_handler != SIG_IGN) {
            	DATA.SignalData.SIG_WINCH.old.sa_handler(SIGWINCH);
        	}

        	Install(SIGWINCH, &DATA.SignalData.SIG_WINCH.old, sigwinch_handler);
		}
    }

#elif defined(_WIN32) || defined(_WIN64)

	// NOTE: I'm not doing this(for now :) )

#endif
}

void SignalsCleanup(void) {
	if(DATA.SignalData.SIG_INT.customTasks != NULL) {
		free(DATA.SignalData.SIG_INT.customTasks);
		DATA.SignalData.SIG_INT.customTasks = NULL;
	}
	if(DATA.SignalData.SIG_WINCH.customTasks != NULL) {
		free(DATA.SignalData.SIG_WINCH.customTasks);
		DATA.SignalData.SIG_WINCH.customTasks = NULL;
	}

#if defined(unix) || defined(__unix) || defined(__unix__)
	
    sigaction(SIGINT, &DATA.SignalData.SIG_INT.old, NULL);
    sigaction(SIGWINCH, &DATA.SignalData.SIG_WINCH.old, NULL);

#elif defined(_WIN32) || defined(_WIN64)

	SetConsoleCtrlHandler(ConsoleHandler, FALSE);

#endif
}

__asm__(
	".pushsection .comment\n"
	".asciz \" Library credits for ESClib(lib for tui) by Sqyd/_Sqyd_/Sqydev, github: https://github.com/Sqydev GPG key: 6DC2516B0DFDA9C59661650722F7B8A777F33B56\"\n"
	".popsection\n"
);
