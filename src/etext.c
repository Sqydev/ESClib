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
#include "./private/common_utils.h"

#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

void DrawCharV(const char* character, Vector2i pos, Color color) {
	DrawChar(character, pos.x, pos.y, color);
}

void DrawChar(const char* character, int x, int y, Color color) {
	DrawCharEx(character, x, y, &color, NULL);
}

void DrawCharExV(const char* character, Vector2i pos, Color* fg, Color* bg) {
	DrawCharEx(character, pos.x, pos.y, fg, bg);
}

// TODO: LIBCSITTYFNSINDEPENDENCE
// memcpy, memset
void DrawCharEx(const char* character, int x, int y, Color* fg, Color* bg) {
	if(character == NULL) {
		return;
	}

	int len = 0;
	if((character[0] & 0x80) == 0) {
		len = 1;
	}
	else if((character[0] & 0xE0) == 0xC0) {
		len = 2;
	}
	else if((character[0] & 0xF0) == 0xE0) {
		len = 3;
	}
	else if((character[0] & 0xF8) == 0xF0) {
		len = 4;
	}
	else {
		TraceLog("[ESCLIB.DrawCharEx]: ERROR: Invalid UTF-8 character, bytes: %d, %d, %d, %d", character[0], character[1], character[2], character[3]);
		return;
	}

	if(len > 0 && character[len] != '\0') {
		TraceLog("[ESCLIB.DrawCharEx]: ERROR: String contains more than one UTF-8 character\n");
		return;
	}

	int vWidth = GetCharWidth(character);

	if(x < 0) {
		return;
	}
	if(x >= DATA.TuiData.tuidimm.x) {
		return;
	}

	if(y < 0) {
		return;
	}
	if(y >= DATA.TuiData.tuidimm.y) {
		return;
	}

	// NOTE: If last char is big than instead of ghosting it just force it into place
	if(x >= DATA.TuiData.tuidimm.x - (vWidth - 1)) {
		x -= vWidth - 1;
	}

	size_t index = y * DATA.TuiData.tuidimm.x + x;

	DATA.Buffers.backbuff[index].CharLen = len;
	memcpy(DATA.Buffers.backbuff[index].Char, character, len);

	if(fg != NULL) {
		DATA.Buffers.backbuff[index].fgColor = *fg;
	}
	if(bg != NULL) {
		DATA.Buffers.backbuff[index].bgColor = *bg;
	}

	for (int i = 1; i < vWidth; i++) {
		size_t nextIndex = index + i;
		
		if (x + i >= DATA.TuiData.tuidimm.x) break;

		DATA.Buffers.backbuff[nextIndex].CharLen = 0; 
		memset(DATA.Buffers.backbuff[nextIndex].Char, 0, 5);
		
		if(fg != NULL) DATA.Buffers.backbuff[nextIndex].fgColor = *fg;
		if(bg != NULL) DATA.Buffers.backbuff[nextIndex].bgColor = *bg;
		
		if(nextIndex > DATA.Buffers.backbuffOffset) {
			DATA.Buffers.backbuffOffset = nextIndex;
		}
	}

	if(index > DATA.Buffers.backbuffOffset) {
		DATA.Buffers.backbuffOffset = index;
	}
}

void DrawTextV(const char* text, Vector2i pos, Color color) {
	DrawChar(text, pos.x, pos.y, color);
}

void DrawText(const char* text, int x, int y, Color color) {
	DrawTextEx(text, x, y, &color, NULL);
}

void DrawTextExV(const char* text, Vector2i pos, Color* fg, Color* bg) {
	DrawTextEx(text, pos.x, pos.y, fg, bg);
}

void DrawTextEx(const char* text, int x, int y, Color* fg, Color* bg) {
	DrawTextPro(text, x, y, 0, 0, fg, bg, 0, 0);
}

void DrawTextProV(const char* text, Vector2i pos, Vector2i origin, Color* fg, Color* bg, int spaceing, float angle) {
	DrawTextPro(text, pos.x, pos.y, origin.x, origin.y, fg, bg, spaceing, angle);
}

// TODO: LIBCSITTYFNSINDEPENDENCE
// memcpy, mabey round and fmaxf
void DrawTextPro(const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spaceing, float angle) {
	if(text == NULL) {
		return;
	}

	float cos = 0.0f;
	float sin = 0.0f;
	Vector2d dir = {0};

	dir = EDir(angle);
	cos = dir.x;
	sin = dir.y;

	// NOTE: Calc startpos with origin!
	float curX = x - (originX * cos) + (originY * sin);
	float curY = y - (originX * sin) - (originY * cos);

	const char* ptrr = text;

	while(*ptrr != '\0') {
		if(curX < 0) {
			return;
		}
		if(curX >= DATA.TuiData.tuidimm.x) {
			return;
		}

		if(curY < 0) {
			return;
		}
		if(curY >= DATA.TuiData.tuidimm.y) {
			return;
		}
		
		int vWidth = GetCharWidth(ptrr);
		
		if(curX >= DATA.TuiData.tuidimm.x - (vWidth - 1)) {
			curX -= vWidth - 1;
		}

		int len = 0;
		if((*ptrr & 0x80) == 0) {
			len = 1;
		}
		else if((*ptrr & 0xE0) == 0xC0) {
			len = 2;
		}
		else if((*ptrr & 0xF0) == 0xE0) {
			len = 3;
		}
		else if((*ptrr & 0xF8) == 0xF0) {
			len = 4;
		}
		else {
			ptrr++;
			continue;
		}

		char tmpChar[5] = {0};
		memcpy(tmpChar, ptrr, len);

		DrawCharEx(tmpChar, (int)roundf(curX), (int)roundf(curY), fg, bg);

		curX += dir.x * (spaceing + 1);
		curY += dir.y * (spaceing + 1);

		ptrr += len;
	}
}

void DrawTextfV(const char* text, Vector2i pos, Color color, ...) {
	va_list va;
	va_start(va, color);
	vaDrawTextf(text, pos.x, pos.y, color, va);
	va_end(va);
}

void DrawTextf(const char* text, int x, int y, Color color, ...) {
	va_list va;
	va_start(va, color);
	vaDrawTextf(text, x, y, color, va);
	va_end(va);
}

void DrawTextfExV(const char* text, Vector2i pos, Color* fg, Color* bg, ...) {
	va_list va;
	va_start(va, bg);
	vaDrawTextfEx(text, pos.x, pos.y, fg, bg, va);
	va_end(va);
}

void DrawTextfEx(const char* text, int x, int y, Color* fg, Color* bg, ...) {
	va_list va;
	va_start(va, bg);
	vaDrawTextfEx(text, x, y, fg, bg, va);
	va_end(va);
}

void DrawTextfProV(const char* text, Vector2i pos, Vector2i origin, Color* fg, Color* bg, int spacing, double angle, ...) {
	va_list va;
	va_start(va, angle);
	vaDrawTextfPro(text, pos.x, pos.y, origin.x, origin.y, fg, bg, spacing, angle, va);
	va_end(va);
}

void DrawTextfPro(const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spacing, double angle, ...) {
	va_list va;
	va_start(va, angle);
	vaDrawTextfPro(text, x, y, originX, originY, fg, bg, spacing, angle, va);
	va_end(va);
}

static void calc_dir(double angle, Vector2 *dir) {
    float q  = (float)(angle / (PI / 2.0));
    int   qi = (int)roundf(q);
    if (fabsf(q - qi) < 0.001f) {
        switch ((qi % 4 + 4) % 4) {
        case 0: dir->x =  1.f; dir->y =  0.f; return;
        case 1: dir->x =  0.f; dir->y =  1.f; return;
        case 2: dir->x = -1.f; dir->y =  0.f; return;
        case 3: dir->x =  0.f; dir->y = -1.f; return;
        }
    }
    float fc = cosf((float)angle), fs = sinf((float)angle);
    float mx = fmaxf(fabsf(fc), fabsf(fs));
    dir->x = mx > 0.f ? fc / mx : fc;
    dir->y = mx > 0.f ? fs / mx : fs;
}

void vaDrawTextfV(const char* text, Vector2i pos, Color color, va_list va) { vaDrawTextfPro(text, pos.x, pos.y, 0, 0, &color, NULL, 0, 0, va); }
void vaDrawTextf(const char* text, int x, int y, Color color, va_list va) { vaDrawTextfPro(text, x, y, 0, 0, &color, NULL, 0, 0, va); }
void vaDrawTextfExV(const char* text, Vector2i pos, Color* fg, Color* bg, va_list va) { vaDrawTextfPro(text, pos.x, pos.y, 0, 0, fg, bg, 0, 0, va); }
void vaDrawTextfEx(const char* text, int x, int y, Color* fg, Color* bg, va_list va) { vaDrawTextfPro(text, x, y, 0, 0, fg, bg, 0, 0, va); }
void vaDrawTextfProV(const char* text, Vector2i pos, Vector2i origin, Color* fg, Color* bg, int spacing, double angle, va_list va) { vaDrawTextfPro(text, pos.x, pos.y, origin.x, origin.y, fg, bg, spacing, angle, va); }
void vaDrawTextfPro(const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spacing, double angle, va_list va) {
    if(!text) {
        UniWriteLen(UNI_WRITE_TARGET_STDERR, "Text is NULL\n");
        return;
    }

    va_list va2;
    va_copy(va2, va);
    int need = vsnprintf(NULL, 0, text, va2);
    va_end(va2);
    if(need < 0) { errno = EINVAL; return; }

    char* buf = malloc((size_t)need + 1);
    if(!buf) { return; }
    vsnprintf(buf, (size_t)need + 1, text, va);

    Color local_fg_storage;
	Color local_bg_storage;

    Color* local_fg = NULL;
	Color* local_bg = NULL;
    if(fg) { local_fg_storage = *fg; local_fg = &local_fg_storage; }
    if(bg) { local_bg_storage = *bg; local_bg = &local_bg_storage; }

    Vector2 dir;
    calc_dir(angle, &dir);

    float curX = x - (originX * dir.x) + (originY * dir.y);
    float curY = y - (originX * dir.y) - (originY * dir.x);

    const char* p = buf;
    while(*p) {
        if(p[0] == '\\' && p[1] == '$') {
            DrawCharEx("$", (int)roundf(curX), (int)roundf(curY), local_fg, local_bg);
            curX += dir.x * (spacing + 1);
            curY += dir.y * (spacing + 1);
            p += 2;
            continue;
        }
        if(p[0] == '$') {
            if (strncmp(p, "$fg", 3) == 0) {
                Color c = va_arg(va, Color);
                local_fg_storage = c;
                local_fg = &local_fg_storage;

                p += 3;
				continue;
            }
            if(strncmp(p, "$bg", 3) == 0) {
                Color c = va_arg(va, Color);
                local_bg_storage = c;
                local_bg = &local_bg_storage;

                p += 3;
				continue;
            }
            if(strncmp(p, "$sp", 3) == 0) {
                spacing = va_arg(va, int);

                p += 3;
				continue;
            }
            if(strncmp(p, "$an", 3) == 0) {
                angle = va_arg(va, double);
                calc_dir(angle, &dir);

                p += 3;
				continue;
            }
        }

        if(curX < 0 || curX >= DATA.TuiData.tuidimm.x) { break; }
        if(curY < 0 || curY >= DATA.TuiData.tuidimm.y) { break; }

        int vWidth = GetCharWidth(p);
        float drawX = curX;
        if(drawX >= DATA.TuiData.tuidimm.x - (vWidth - 1)) { drawX -= vWidth - 1; }

        int clen;
        if((*p & 0x80) == 0x00) { clen = 1; }
        else if((*p & 0xE0) == 0xC0) { clen = 2; }
        else if((*p & 0xF0) == 0xE0) { clen = 3; }
        else if((*p & 0xF8) == 0xF0) { clen = 4; }
        else { p++; continue; }

        char tmp[5] = { 0 };
        memcpy(tmp, p, clen);
        DrawCharEx(tmp, (int)roundf(drawX), (int)roundf(curY), local_fg, local_bg);

        curX += dir.x * (spacing + 1);
        curY += dir.y * (spacing + 1);
        p += clen;
    }

    free(buf);
}
