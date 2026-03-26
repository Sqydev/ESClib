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

#include "./private/coredata.h"

#if defined(unix) || defined(__unix) || defined(__unix__)
	#include <time.h>
	#include <wchar.h>
	#include <locale.h>

	extern int wcwidth (wchar_t __c);

#elif defined(_WIN32) || defined(_WIN64)
#endif

#include <errno.h>
#include <stdlib.h>

void ESleep(unsigned long sec, unsigned long ms, unsigned long ns) {
	struct timespec required, remaining;

	required.tv_sec = (time_t)(sec + (ms / 1000));

	unsigned long total_ns = (ms % 1000UL) * 1000000UL + ns;
	if (total_ns >= 1000000000UL) {
		required.tv_sec += (time_t)(total_ns / 1000000000UL);
		total_ns %= 1000000000UL;
	}
	required.tv_nsec = (long)total_ns;

	while(nanosleep(&required, &remaining) == -1) {
		if(errno == EINTR) {
			if(DATA.SignalData.SIG_INT.enabled && DATA.SignalData.SIG_INT.triggered) {
				break; 
			}
			required = remaining;
		}
		else {
			break;
		}
	}
}

int GetCharWidth(const char* character) {
#if defined(unix) || defined(__unix) || defined(__unix__)

    static int locale_initialized = 0;
    if(!locale_initialized) {
        setlocale(LC_CTYPE, ""); 
        locale_initialized = 1;
    }

    wchar_t wc;
    int len = mbtowc(&wc, character, 4);
    
    if(len <= 0) return 0;

    int width = wcwidth(wc);
    
    return (width < 0) ? 0 : width;

#elif defined(_WIN32) || defined(_WIN64)
#endif
}
