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

#ifdef ESCLIB_KEYIN_EXPERIMENT_ENABLE

#include "./private/coredata.h"
#include "./private/input/keyboard.h"

#include <stdbool.h>

#if defined(unix) || defined(__unix) || defined(__unix__)

	#include <unistd.h>
	#include <sys/select.h>

#elif defined(_WIN32) || defined(_WIN64)

#endif

void PressKey(int key) {
	if(key < 0 || key > ESC_KEYMAX) { return; }
	DATA.Input.Keyboard.keyStates[key] = true;
}

bool IsKeyPressed(int key) {
	if(key < 0 || key > ESC_KEYMAX) { return false; }
	return DATA.Input.Keyboard.keyStates[key];
}

bool IsKeyDown(int key) {
	return IsKeyPressed(key);
}

KeyboardKey GetKeyPressed(void) {
	static size_t scanner = 0;

	while(scanner <= ESC_KEYMAX) {
		if(DATA.Input.Keyboard.keyStates[scanner]) { return scanner; }

		scanner++;
	}

	return KEY_NULL;
}

void WaitForKeyPress(void) {
#if defined(unix) || defined(__unix) || defined(__unix__)

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	select(STDIN_FILENO + 1, &readfds, NULL, NULL, NULL);

	KeyboardStep(true);

#elif defined(_WIN32) || defined(_WIN64)

#endif
}

KeyboardKey SingleByteToKeycode(unsigned char c) {
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

        case '\r': case '\n': return KEY_ENTER;
        case '\x7f': case '\x08': return KEY_BACKSPACE;
        case '\t': return KEY_TAB;
        case ' ': return KEY_SPACE;
        case '\033': return KEY_ESCAPE;
        case '-': return KEY_MINUS;
        case '=': return KEY_EQUAL;
        case '[': return KEY_LEFT_BRACE;
        case ']': return KEY_RIGHT_BRACE;
        case ';': return KEY_SEMICOLON;
        case '\'': return KEY_APOSTROPHE;
        case ',': return KEY_COMMA;
        case '.': return KEY_DOT;
        case '/': return KEY_SLASH;
        case '\\': return KEY_BACKSLASH;
        case '`': return KEY_GRAVE;
        default: return -1;
    }
}

char KeycodeToSingleByte(KeyboardKey keyCode) {
    switch (keyCode) {
        case KEY_A: return 'A';
        case KEY_B: return 'B';
        case KEY_C: return 'C';
        case KEY_D: return 'D';
        case KEY_E: return 'E';
        case KEY_F: return 'F';
        case KEY_G: return 'G';
        case KEY_H: return 'H';
        case KEY_I: return 'I';
        case KEY_J: return 'J';
        case KEY_K: return 'K';
        case KEY_L: return 'L';
        case KEY_M: return 'M';
        case KEY_N: return 'N';
        case KEY_O: return 'O';
        case KEY_P: return 'P';
        case KEY_Q: return 'Q';
        case KEY_R: return 'R';
        case KEY_S: return 'S';
        case KEY_T: return 'T';
        case KEY_U: return 'U';
        case KEY_V: return 'V';
        case KEY_W: return 'W';
        case KEY_X: return 'X';
        case KEY_Y: return 'Y';
        case KEY_Z: return 'Z';

        case KEY_1: return '1';
        case KEY_2: return '2';
        case KEY_3: return '3';
        case KEY_4: return '4';
        case KEY_5: return '5';
        case KEY_6: return '6';
        case KEY_7: return '7';
        case KEY_8: return '8';
        case KEY_9: return '9';
        case KEY_0: return '0';

        case KEY_ENTER: return '\n';
        case KEY_BACKSPACE: return '\b';
        case KEY_TAB: return '\t';
        case KEY_SPACE: return ' ';
        case KEY_ESCAPE: return '\033';
        case KEY_MINUS: return '-';
        case KEY_EQUAL: return '=';
        case KEY_LEFT_BRACE: return '[';
        case KEY_RIGHT_BRACE: return ']';
        case KEY_SEMICOLON: return ';';
        case KEY_APOSTROPHE: return '\'';
        case KEY_COMMA: return ',';
        case KEY_DOT: return '.';
        case KEY_SLASH: return '/';
        case KEY_BACKSLASH: return '\\';
        case KEY_GRAVE: return '`';

        default: return '\0';
    }
}

#endif
