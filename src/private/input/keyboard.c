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

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>

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

Display* x11Dpy = NULL;
bool x11Pressed[KEY_MAX];
Atom x11NetWmPid;
 
pid_t x11AncestorPids[32];
int x11NAncestors = 0;
 
void CacheAncestorPids(void) {
    x11NAncestors = 0;
    pid_t p = getpid();
    while(p > 1 && x11NAncestors < 32) {
        x11AncestorPids[x11NAncestors++] = p;
        char path[64];
        snprintf(path, sizeof(path), "/proc/%d/status", (int)p);
        FILE* f = fopen(path, "r");
        if(!f) break;
        pid_t pp = -1;
        char line[256];
        while(fgets(line, sizeof(line), f)) {
            if(strncmp(line, "PPid:", 5) == 0) { pp = (pid_t)atol(line + 5); break; }
        }
        fclose(f);
        if(pp <= 1) break;
        p = pp;
    }
}
 
bool HasFocus(void) {
    Window focused;
    int revert;
    XGetInputFocus(x11Dpy, &focused, &revert);
 
    if(focused == None || focused == PointerRoot) return false;
 
    if(x11NetWmPid == None) return true;
 
    Window root = DefaultRootWindow(x11Dpy);
    Window w = focused;
 
    while(w != None && w != root) {
        Atom actual_type;
        int actual_format;
        unsigned long nitems, bytes_after;
        unsigned char* prop = NULL;
 
        if(XGetWindowProperty(x11Dpy, w, x11NetWmPid, 0, 1, False,
                XA_CARDINAL, &actual_type, &actual_format,
                &nitems, &bytes_after, &prop) == Success && prop && nitems > 0) {
            pid_t win_pid = (pid_t)(*(unsigned long*)prop);
            XFree(prop);
            for(int i = 0; i < x11NAncestors; i++) {
                if(x11AncestorPids[i] == win_pid) return true;
            }
        } else {
            if(prop) XFree(prop);
        }
 
        Window parent, dummy_root;
        Window* children  = NULL;
        unsigned int nch;
        if(!XQueryTree(x11Dpy, w, &dummy_root, &parent, &children, &nch)) break;
        if(children) XFree(children);
        if(parent == root || parent == None) break;
        w = parent;
    }
 
    return false;
}
 
int KeysymToLinux(KeySym ks) {
    switch (ks) {
        case XK_a: case XK_A: return KEY_A;
        case XK_b: case XK_B: return KEY_B;
        case XK_c: case XK_C: return KEY_C;
        case XK_d: case XK_D: return KEY_D;
        case XK_e: case XK_E: return KEY_E;
        case XK_f: case XK_F: return KEY_F;
        case XK_g: case XK_G: return KEY_G;
        case XK_h: case XK_H: return KEY_H;
        case XK_i: case XK_I: return KEY_I;
        case XK_j: case XK_J: return KEY_J;
        case XK_k: case XK_K: return KEY_K;
        case XK_l: case XK_L: return KEY_L;
        case XK_m: case XK_M: return KEY_M;
        case XK_n: case XK_N: return KEY_N;
        case XK_o: case XK_O: return KEY_O;
        case XK_p: case XK_P: return KEY_P;
        case XK_q: case XK_Q: return KEY_Q;
        case XK_r: case XK_R: return KEY_R;
        case XK_s: case XK_S: return KEY_S;
        case XK_t: case XK_T: return KEY_T;
        case XK_u: case XK_U: return KEY_U;
        case XK_v: case XK_V: return KEY_V;
        case XK_w: case XK_W: return KEY_W;
        case XK_x: case XK_X: return KEY_X;
        case XK_y: case XK_Y: return KEY_Y;
        case XK_z: case XK_Z: return KEY_Z;
 
        case XK_1: case XK_exclam:     return KEY_1;
        case XK_2: case XK_at:         return KEY_2;
        case XK_3: case XK_numbersign:  return KEY_3;
        case XK_4: case XK_dollar:      return KEY_4;
        case XK_5: case XK_percent:     return KEY_5;
        case XK_6: case XK_asciicircum: return KEY_6;
        case XK_7: case XK_ampersand:   return KEY_7;
        case XK_8: case XK_asterisk:    return KEY_8;
        case XK_9: case XK_parenleft:   return KEY_9;
        case XK_0: case XK_parenright:  return KEY_0;
 
        case XK_F1:  return KEY_F1;
        case XK_F2:  return KEY_F2;
        case XK_F3:  return KEY_F3;
        case XK_F4:  return KEY_F4;
        case XK_F5:  return KEY_F5;
        case XK_F6:  return KEY_F6;
        case XK_F7:  return KEY_F7;
        case XK_F8:  return KEY_F8;
        case XK_F9:  return KEY_F9;
        case XK_F10: return KEY_F10;
        case XK_F11: return KEY_F11;
        case XK_F12: return KEY_F12;
 
        case XK_Up:        return KEY_UP;
        case XK_Down:      return KEY_DOWN;
        case XK_Left:      return KEY_LEFT;
        case XK_Right:     return KEY_RIGHT;
        case XK_Insert:    return KEY_INSERT;
        case XK_Delete:    return KEY_DELETE;
        case XK_Home:      return KEY_HOME;
        case XK_End:       return KEY_END;
        case XK_Page_Up:   return KEY_PAGEUP;
        case XK_Page_Down: return KEY_PAGEDOWN;
 
        case XK_Return:         return KEY_ENTER;
        case XK_Escape:         return KEY_ESC;
        case XK_BackSpace:      return KEY_BACKSPACE;
        case XK_Tab:            return KEY_TAB;
        case XK_space:          return KEY_SPACE;
        case XK_minus:
        case XK_underscore:     return KEY_MINUS;
        case XK_equal:
        case XK_plus:           return KEY_EQUAL;
        case XK_bracketleft:
        case XK_braceleft:      return KEY_LEFTBRACE;
        case XK_bracketright:
        case XK_braceright:     return KEY_RIGHTBRACE;
        case XK_semicolon:
        case XK_colon:          return KEY_SEMICOLON;
        case XK_apostrophe:
        case XK_quotedbl:       return KEY_APOSTROPHE;
        case XK_comma:
        case XK_less:           return KEY_COMMA;
        case XK_period:
        case XK_greater:        return KEY_DOT;
        case XK_slash:
        case XK_question:       return KEY_SLASH;
        case XK_backslash:
        case XK_bar:            return KEY_BACKSLASH;
        case XK_grave:
        case XK_asciitilde:     return KEY_GRAVE;
 
        case XK_Shift_L:    return KEY_LEFTSHIFT;
        case XK_Shift_R:    return KEY_RIGHTSHIFT;
        case XK_Control_L:  return KEY_LEFTCTRL;
        case XK_Control_R:  return KEY_RIGHTCTRL;
        case XK_Alt_L:      return KEY_LEFTALT;
        case XK_Alt_R:      return KEY_RIGHTALT;
        case XK_Super_L:    return KEY_LEFTMETA;
        case XK_Super_R:    return KEY_RIGHTMETA;
        case XK_Menu:       return KEY_COMPOSE;
        case XK_Caps_Lock:  return KEY_CAPSLOCK;
        case XK_Num_Lock:   return KEY_NUMLOCK;
        case XK_Scroll_Lock: return KEY_SCROLLLOCK;
        case XK_Print:      return KEY_SYSRQ;
        case XK_Pause:      return KEY_PAUSE;
 
        case XK_KP_0:       case XK_KP_Insert:   return KEY_KP0;
        case XK_KP_1:       case XK_KP_End:       return KEY_KP1;
        case XK_KP_2:       case XK_KP_Down:      return KEY_KP2;
        case XK_KP_3:       case XK_KP_Page_Down: return KEY_KP3;
        case XK_KP_4:       case XK_KP_Left:      return KEY_KP4;
        case XK_KP_5:       case XK_KP_Begin:     return KEY_KP5;
        case XK_KP_6:       case XK_KP_Right:     return KEY_KP6;
        case XK_KP_7:       case XK_KP_Home:      return KEY_KP7;
        case XK_KP_8:       case XK_KP_Up:        return KEY_KP8;
        case XK_KP_9:       case XK_KP_Page_Up:   return KEY_KP9;
        case XK_KP_Enter:   return KEY_KPENTER;
        case XK_KP_Add:     return KEY_KPPLUS;
        case XK_KP_Subtract: return KEY_KPMINUS;
        case XK_KP_Multiply: return KEY_KPASTERISK;
        case XK_KP_Divide:  return KEY_KPSLASH;
        case XK_KP_Decimal: case XK_KP_Delete: return KEY_KPDOT;
 
        default: return -1;
    }
}
 
bool InitX11Keyboard(void) {
    x11Dpy = XOpenDisplay(NULL);
    if(!x11Dpy) {
        UniWriteLen(UNI_WRITE_TARGET_STDERR, "WARNING: XOpenDisplay() failed fallback to evdev\n");
        return false;
    }
    x11NetWmPid = XInternAtom(x11Dpy, "_NET_WM_PID", True);
    CacheAncestorPids();
    memset(x11Pressed, 0, sizeof(x11Pressed));
    return true;
}
 
void X11KeyboardStep(void) {
    if(!x11Dpy) return;
 
    for(int i = 0; i < KEY_MAX; i++) {
        if(x11Pressed[i]) {
            DATA.Input.Keyboard.keyStates[i] = false;
            x11Pressed[i] = false;
        }
    }
 
    if(!HasFocus()) return;
 
    char rawmap[32];
    XQueryKeymap(x11Dpy, rawmap);
 
    for(int xkc = 8; xkc < 256; xkc++) {
        bool down = (rawmap[xkc >> 3] >> (xkc & 7)) & 1;
        if(!down) continue;
 
        KeySym ks = XkbKeycodeToKeysym(x11Dpy, (KeyCode)xkc, 0, 0);
        if(ks == NoSymbol) continue;
 
        int lkc = KeysymToLinux(ks);
        if(lkc < 0 || lkc >= KEY_MAX) continue;
 
        DATA.Input.Keyboard.keyStates[lkc] = true;
        x11Pressed[lkc] = true;
    }
}
 
void CloseX11Keyboard(void) {
    if(x11Dpy) {
        XCloseDisplay(x11Dpy);
        x11Dpy = NULL;
    }
    memset(x11Pressed, 0, sizeof(x11Pressed));
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
		if(DATA.SystemInfo.compositor == WAYLAND && InitWaylandKeyboard()) {
			DATA.Input.InputBackend = COMP_WAYLAND;
		}
		else if(DATA.SystemInfo.compositor == X11 && InitX11Keyboard()) {
			DATA.Input.InputBackend = COMP_X11;
		}
		else if(InitEvdev()) {
			DATA.Input.InputBackend = EVDEV;
		}
		else {
			DATA.Input.typewriterMode = true;
			DATA.Input.InputBackend = TERMIOS;
			InitTermios();
		}
	}
	else {
		DATA.Input.InputBackend = TERMIOS;
		InitTermios();
	}
}

void KeyboardStep(void) {
	if(!DATA.Input.Keyboard.keyStates) { return; }

	memcpy(DATA.Input.Keyboard.prevKeyStates, DATA.Input.Keyboard.keyStates, KEY_MAX * sizeof(bool));

	switch(DATA.Input.InputBackend) {
		case EVDEV: { EvdevStep(); break; }
		case COMP_X11: { X11KeyboardStep(); break; }
		case COMP_WAYLAND: { WaylandKeyboardStep(); break; }
		default: { TermiosStep(); break; }
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
