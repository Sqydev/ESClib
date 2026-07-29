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

#if defined(unix) || defined(__unix) || defined(__unix__)
	
#include "../common_utils.h"
#include "../coredata.h"

#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <ctype.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct { const char* seq; int code; } SeqEntry;
static const SeqEntry SEQ_TABLE[] = {
	{ "\n", KEY_ENTER },
    { "\r", KEY_ENTER },
    { "\t", KEY_TAB },
    { "\033", KEY_ESCAPE },

	{ "\033[11~", KEY_F1 }, { "\033[12~", KEY_F2 },
	{ "\033[13~", KEY_F3 }, { "\033[14~", KEY_F4 },
	{ "\033[15~", KEY_F5 }, { "\033[17~", KEY_F6 },
	{ "\033[18~", KEY_F7 }, { "\033[19~", KEY_F8 },
	{ "\033[20~", KEY_F9 }, { "\033[21~", KEY_F10 },
	{ "\033[23~", KEY_F11 }, { "\033[24~", KEY_F12 },
	{ "\033OP", KEY_F1 }, { "\033OQ", KEY_F2 },
	{ "\033OR", KEY_F3 }, { "\033OS", KEY_F4 },
	{ "\033[2~", KEY_INSERT }, { "\033[3~", KEY_DELETE },
	{ "\033[5~", KEY_PAGE_UP }, { "\033[6~", KEY_PAGE_DOWN },
	{ "\033[H",  KEY_HOME }, { "\033[F",  KEY_END },
	{ "\033OH",  KEY_HOME }, { "\033OF",  KEY_END },
	{ "\033[A", KEY_UP }, { "\033[B", KEY_DOWN },
	{ "\033[C", KEY_RIGHT }, { "\033[D", KEY_LEFT },
	{ NULL, 0 }
};

void InitKeyboard(void) {
	if(!isatty(STDIN_FILENO)) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: STDIN IN NON EXISTENT\n");
		CloseTui();
		exit(EXIT_FAILURE);
	}
 
	if(DATA.Input.Keyboard.keysArr) { free(DATA.Input.Keyboard.keysArr); DATA.Input.Keyboard.keysArr = NULL; }
	DATA.Input.Keyboard.keysArrCapasity = 1;
	DATA.Input.Keyboard.keysArrTaken = 0;
	DATA.Input.Keyboard.keysArr = malloc(sizeof(EscKey));

	DATA.Input.Keyboard.getKeyPressedScanner = 0;
}

void KeyboardStep(bool saveLastState) {
	if(!saveLastState) {
		for(int i = 0; i < DATA.Input.Keyboard.keysArrTaken; i++) {
			if(DATA.Input.Keyboard.keysArr[i]) {
				DATA.Input.Keyboard.keysArr[i] = KEY_NULL;
			}
		}

		DATA.Input.Keyboard.getKeyPressedScanner = 0;
	}
 
	unsigned char buf[64];
	int n;
	while((n = (int)read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
		int i = 0;

		while(i < n) {
			if(iscntrl((unsigned char)buf[i])) {
				bool matched = false;
				for(const SeqEntry* entrie = SEQ_TABLE; entrie->seq; entrie++) {
					int seqlen = (int)strlen(entrie->seq);

					if(i + seqlen <= n && memcmp(buf + i, entrie->seq, (size_t)seqlen) == 0) {
						PressKey(entrie->code);
						i += seqlen;
						matched = true;
						break;
					}
				}
				if(matched) { continue; }
			}
 
			PressKey(buf[i]);
			i++;
		}
	}
}

void CloseKeyboard(void) {
	if(DATA.Input.Keyboard.keysArr) { free(DATA.Input.Keyboard.keysArr); DATA.Input.Keyboard.keysArr = NULL; }
	DATA.Input.Keyboard.keysArrCapasity = 0;
	DATA.Input.Keyboard.keysArrTaken = 0;
	DATA.Input.Keyboard.getKeyPressedScanner = 0;
}

#elif defined(_WIN32) || defined(_WIN64)

void InitKeyboard(void) {
	return;
}

void KeyboardStep(void) {
	return;
}

#endif
