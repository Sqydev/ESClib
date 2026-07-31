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

#include "./private/input/keyboard.h"

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#if defined(unix) || defined(__unix) || defined(__unix__)

	#include <unistd.h>
	#include <sys/select.h>

#elif defined(_WIN32) || defined(_WIN64)

#endif

void PressKey(EscKey key) {
	if(DATA.Input.Keyboard.keysArrCapasity <= DATA.Input.Keyboard.keysArrTaken) {
		EscKey* tmp = realloc(DATA.Input.Keyboard.keysArr, (DATA.Input.Keyboard.keysArrCapasity + 1)  * sizeof(EscKey));
		if(!tmp) { errno = ENOMEM; return; }

		DATA.Input.Keyboard.keysArr = tmp;
		DATA.Input.Keyboard.keysArrCapasity++;
	}

	DATA.Input.Keyboard.keysArr[DATA.Input.Keyboard.keysArrTaken] = key;
	DATA.Input.Keyboard.keysArrTaken++;
}

bool IsKeyPressed(EscKey key) {
	for(size_t i = 0; i < DATA.Input.Keyboard.keysArrTaken; i++) {
		if(DATA.Input.Keyboard.keysArr[i] == key) { return true; }
	}
	return false;
}

EscKey GetKeyPressed(void) {
	if(DATA.Input.Keyboard.getKeyPressedScanner >= DATA.Input.Keyboard.keysArrTaken) {
		return KEY_NULL;
	}
	return DATA.Input.Keyboard.keysArr[DATA.Input.Keyboard.getKeyPressedScanner++];
}

EscKey WaitForKeyPress(void) {
#if defined(unix) || defined(__unix) || defined(__unix__)

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	select(STDIN_FILENO + 1, &readfds, NULL, NULL, NULL);

	unsigned char buf[64];
	int n = (int)read(STDIN_FILENO, buf, sizeof(buf));
	if(n <= 0) { return KEY_NULL; }

	if(iscntrl((unsigned char)buf[0])) {
		EscKey code;
		if(TryMatchSeq(buf, 0, n, &code) > 0) { return code; }
	}

	return buf[0];

#elif defined(_WIN32) || defined(_WIN64)

#endif
}

void WaitForKeyPressAndRegister(void) {
#if defined(unix) || defined(__unix) || defined(__unix__)

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	select(STDIN_FILENO + 1, &readfds, NULL, NULL, NULL);

	KeyboardStep(true);

#elif defined(_WIN32) || defined(_WIN64)

#endif
}
