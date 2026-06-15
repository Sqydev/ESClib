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

#include <math.h>
#include <string.h>

void DrawCharVP(Panel panel, const char* character, Vector2i pos, Color color) {
	if(panel.x + panel.width <= panel.x + pos.x || pos.x < 0) { return; }
	if(panel.y + panel.height <= panel.y + pos.y || pos.y < 0) { return; }

	DrawChar(character, panel.x + pos.x, panel.y + pos.y, color);
}

void DrawCharP(Panel panel, const char* character, int x, int y, Color color) {
	if(panel.x + panel.width <= panel.x + x || x < 0) { return; }
	if(panel.y + panel.height <= panel.y + y || y < 0) { return; }

	DrawChar(character, panel.x + x, panel.y + y, color);
}

void DrawCharExVP(Panel panel, const char* character, Vector2i pos, Color* fg, Color* bg) {
	if(panel.x + panel.width <= panel.x + pos.x || pos.x < 0) { return; }
	if(panel.y + panel.height <= panel.y + pos.y || pos.y < 0) { return; }

	DrawCharEx(character, panel.x + pos.x, panel.y + pos.y, fg, bg);
}

void DrawCharExP(Panel panel, const char* character, int x, int y, Color* fg, Color* bg) {
	if(panel.x + panel.width <= panel.x + x || x < 0) { return; }
	if(panel.y + panel.height <= panel.y + y || y < 0) { return; }

	DrawCharEx(character, panel.x + x, panel.y + y, fg, bg);
}



void DrawTextVP(Panel panel, const char* text, Vector2i pos, Color color) {
	DrawTextP(panel, text, pos.x, pos.y, color);
}

void DrawTextP(Panel panel, const char* text, int x, int y, Color color) {
	DrawTextExP(panel, text, x, y, &color, NULL);
}

void DrawTextExVP(Panel panel, const char* text, Vector2i pos, Color* fg, Color* bg) {
	DrawTextExP(panel, text, pos.x, pos.y, fg, bg);
}

void DrawTextExP(Panel panel, const char* text, int x, int y, Color* fg, Color* bg) {
	DrawTextProP(panel, text, x, y, 0, 0, fg, bg, 0, 0);
}

void DrawTextProVP(Panel panel, const char* text, Vector2i pos, Vector2i origin, Color* fg, Color* bg, int spaceing, float angle) {
	DrawTextProP(panel, text, pos.x, pos.y, origin.x, origin.y, fg, bg, spaceing, angle);
}

void DrawTextProP(Panel panel, const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spaceing, float angle) {
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
		if(curX >= panel.x + panel.width) {
			return;
		}
		if(curY >= panel.y + panel.height) {
			return;
		}
		
		int vWidth = GetCharWidth(ptrr);
		
		if(curX >= panel.x + panel.width - (vWidth - 1)) {
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

		if(curX < panel.x || curY < panel.y) {
			ptrr += len;
			curX += dir.x * (spaceing + 1);
			curY += dir.y * (spaceing + 1);
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
