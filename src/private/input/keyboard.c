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

#include <linux/input.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

// NOTE: Wierd chatgbt keyboard finder
char* FindEvdevKeyboard(void) {
	static char bestPath[512];
	bestPath[0] = '\0';
 
	DIR* dr = opendir("/dev/input/");
	if(!dr) return NULL;
 
	struct dirent* de;
	while((de = readdir(dr)) != NULL) {
		if(strncmp(de->d_name, "event", 5) != 0) continue;
 
		char path[512];
		snprintf(path, sizeof(path), "/dev/input/%s", de->d_name);
 
		int fd = open(path, O_RDONLY | O_NONBLOCK);
		if(fd < 0) continue;
 
		unsigned long keybit[NBITS(KEY_MAX)];
		memset(keybit, 0, sizeof(keybit));
 
		char name[256] = "";
		ioctl(fd, EVIOCGNAME(sizeof(name)), name);
 
		if(ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybit)), keybit) >= 0) {
			if(keybit[BIT_WORD(KEY_ENTER)] & BIT_MASK(KEY_ENTER)) {
				if(strstr(name, "irtual")) {
					snprintf(bestPath, sizeof(bestPath), "%s", path);
					close(fd);
					closedir(dr);
					return bestPath;
				}
				if(bestPath[0] == '\0')
					snprintf(bestPath, sizeof(bestPath), "%s", path);
			}
		}
		close(fd);
	}
	closedir(dr);
	return bestPath[0] ? bestPath : NULL;
}

bool InitEvdev(void) {
	char* dev = FindEvdevKeyboard();
	if(!dev) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: No keyboard found! Fallback to typewriterMode\n");
		return false;
	}
	DATA.Input.Keyboard.evdevIndex = -1;
	DATA.Input.Keyboard.evdevIndex = open(dev, O_RDONLY | O_NONBLOCK);

	if(DATA.Input.Keyboard.evdevIndex < 0) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: open() for evdev failed. You need to be superuser or be in input group becouse you there is no way to get real type input without grafical interface\n");
		return false;
	}

	return true;
}

void EvdevStep(void) {
	struct input_event inputEvent;

	while(read(DATA.Input.Keyboard.evdevIndex, &inputEvent, sizeof(inputEvent)) > 0) {
		if(inputEvent.type != EV_KEY) { continue; }
 
		if(inputEvent.value == 1) {
			DATA.Input.Keyboard.keyStates[inputEvent.code] = true;
		}
		else if(inputEvent.value == 0) {
			DATA.Input.Keyboard.keyStates[inputEvent.code] = false;
		}
	}
}

void CloseEvdev(void) {
	if(DATA.Input.Keyboard.evdevIndex > 0) {
		close(DATA.Input.Keyboard.evdevIndex);
		DATA.Input.Keyboard.evdevIndex = -1;
	}
}

bool termiosPressedThisStep[KEY_MAX];

bool InitTermios(void) {
	if(!isatty(STDIN_FILENO)) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: STDIN IN NON EXISTENT\n");
		CloseTui();
		exit(EXIT_FAILURE);
	}
 
	memset(termiosPressedThisStep, 0, sizeof(termiosPressedThisStep));
	return true;
}

typedef struct { const char* seq; int code; } SeqEntry;

static const SeqEntry SEQ_TABLE[] = {
	{ "\033[11~", KEY_F1  }, { "\033[12~", KEY_F2  },
	{ "\033[13~", KEY_F3  }, { "\033[14~", KEY_F4  },
	{ "\033[15~", KEY_F5  }, { "\033[17~", KEY_F6  },
	{ "\033[18~", KEY_F7  }, { "\033[19~", KEY_F8  },
	{ "\033[20~", KEY_F9  }, { "\033[21~", KEY_F10 },
	{ "\033[23~", KEY_F11 }, { "\033[24~", KEY_F12 },
	{ "\033OP", KEY_F1 }, { "\033OQ", KEY_F2 },
	{ "\033OR", KEY_F3 }, { "\033OS", KEY_F4 },
	{ "\033[2~", KEY_INSERT   }, { "\033[3~", KEY_DELETE   },
	{ "\033[5~", KEY_PAGEUP   }, { "\033[6~", KEY_PAGEDOWN },
	{ "\033[H",  KEY_HOME     }, { "\033[F",  KEY_END      },
	{ "\033OH",  KEY_HOME     }, { "\033OF",  KEY_END      },
	{ "\033[A", KEY_UP    }, { "\033[B", KEY_DOWN  },
	{ "\033[C", KEY_RIGHT }, { "\033[D", KEY_LEFT  },
	{ NULL, 0 }
};

int SingleByteToKeycode(unsigned char c) {
    if(c >= 'a' && c <= 'z') { c -= 32; }

    switch(c) {
        case 'A': return KEY_A;
        case 'B': return KEY_B;
        case 'C': return KEY_C;
        case 'D': return KEY_D;
        case 'E': return KEY_E;
        case 'F': return KEY_F;
        case 'G': return KEY_G;
        case 'H': return KEY_H;
        case 'I': return KEY_I;
        case 'J': return KEY_J;
        case 'K': return KEY_K;
        case 'L': return KEY_L;
        case 'M': return KEY_M;
        case 'N': return KEY_N;
        case 'O': return KEY_O;
        case 'P': return KEY_P;
        case 'Q': return KEY_Q;
        case 'R': return KEY_R;
        case 'S': return KEY_S;
        case 'T': return KEY_T;
        case 'U': return KEY_U;
        case 'V': return KEY_V;
        case 'W': return KEY_W;
        case 'X': return KEY_X;
        case 'Y': return KEY_Y;
        case 'Z': return KEY_Z;

        case '1': return KEY_1;
        case '2': return KEY_2;
        case '3': return KEY_3;
        case '4': return KEY_4;
        case '5': return KEY_5;
        case '6': return KEY_6;
        case '7': return KEY_7;
        case '8': return KEY_8;
        case '9': return KEY_9;
        case '0': return KEY_0;

        case '\r': case '\n':     return KEY_ENTER;
        case '\x7f': case '\x08': return KEY_BACKSPACE;
        case '\t':                return KEY_TAB;
        case ' ':                 return KEY_SPACE;
        case '\033':              return KEY_ESC;
        case '-':                 return KEY_MINUS;
        case '=':                 return KEY_EQUAL;
        case '[':                 return KEY_LEFTBRACE;
        case ']':                 return KEY_RIGHTBRACE;
        case ';':                 return KEY_SEMICOLON;
        case '\'':                return KEY_APOSTROPHE;
        case ',':                 return KEY_COMMA;
        case '.':                 return KEY_DOT;
        case '/':                 return KEY_SLASH;
        case '\\':                return KEY_BACKSLASH;
        case '`':                 return KEY_GRAVE;
        default:                  return -1;
    }
}

void PressKey(int code) {
	if(code < 0 || code >= KEY_MAX) { return; }
	DATA.Input.Keyboard.keyStates[code] = true;
	termiosPressedThisStep[code] = true;
}

void TermiosStep(void) {
	for(int i = 0; i < KEY_MAX; i++) {
		if(termiosPressedThisStep[i]) {
			DATA.Input.Keyboard.keyStates[i] = false;
			termiosPressedThisStep[i]       = false;
		}
	}
 
	// NOTE: Some chatgbt shenanegance
	unsigned char buf[64];
	int n;
	while((n = (int)read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
		int i = 0;
		while(i < n) {
			if(buf[i] == '\033' && i + 1 < n) {
				bool matched = false;
				for(const SeqEntry* e = SEQ_TABLE; e->seq; e++) {
					int slen = (int)strlen(e->seq);
					if(i + slen <= n && memcmp(buf + i, e->seq, (size_t)slen) == 0) {
						PressKey(e->code);
						i += slen;
						matched = true;
						break;
					}
				}
				if(matched) continue;
			}
 
			PressKey(SingleByteToKeycode(buf[i]));
			i++;
		}
	}
}

bool InitX11Keyboard() {
	return false;
}

void X11KeyboardStep() {
	return;
}

void CloseX11Keyboard() {
	return;
}

bool InitWaylandKeyboard() {
	return false;
}

void WaylandKeyboardStep() {
	return;
}

void CloseWaylandKeyboard() {
	return;
}

void InitKeyboard(void) {
	if(!DATA.Input.Keyboard.keyStates) {
		DATA.Input.Keyboard.keyStates = (bool*)calloc(KEY_MAX, sizeof(bool));
		DATA.Input.Keyboard.prevKeyStates = (bool*)calloc(KEY_MAX, sizeof(bool));
	}

	if(!DATA.Input.typewriterMode) {
		if(!InitEvdev()) {
			DATA.Input.typewriterMode = true;
			InitTermios();
		}
	}
	else {
		InitTermios();
	}
}

void KeyboardStep(void) {
	if(!DATA.Input.Keyboard.keyStates) { return; }

	memcpy(DATA.Input.Keyboard.prevKeyStates, DATA.Input.Keyboard.keyStates, KEY_MAX * sizeof(bool));

	if(!DATA.Input.typewriterMode) {
		EvdevStep();
	}
	else {
		TermiosStep();
	}
}

void CloseKeyboard(void) {
	if(!DATA.Input.typewriterMode) {
		CloseEvdev();
	}

	free(DATA.Input.Keyboard.keyStates);
	free(DATA.Input.Keyboard.prevKeyStates);
	DATA.Input.Keyboard.keyStates = NULL;
	DATA.Input.Keyboard.prevKeyStates = NULL;
}

#elif defined(_WIN32) || defined(_WIN64)

void InitKeyboard(void) {
	return;
}

void CloseKeyboard(void) {
	return;
}

void KeyboardStep(void) {
	return;
}

#endif
