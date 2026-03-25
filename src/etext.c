#include "../include/esclib.h"

#include "./private/coredata.h"
#include "./private/common_utils.h"

#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "Character is NULL\n");
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
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: Invalid UTF-8 character\n");
		return;
	}

	if(len > 0 && character[len] != '\0') {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "ERROR: String contains more than one UTF-8 character\n");
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
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "Text is NULL\n");
		return;
	}

	float cos = 0.0f;
	float sin = 0.0f;
	Vecrot2 dir = {0};

	// NOTE: Check if angle is multiplicity of PI / 2. Accounding for float errors
	float q = angle / (PI / 2.0f);
	int qi = (int)roundf(q);
	if(fabsf(q - qi) < 0.001f) {
		// NOTE: Somehow get dir
		int dir_idx = (qi % 4 + 4) % 4;

		switch(dir_idx) {
			case 0: { cos = 1.0f; sin = 0.0f; break; }
			case 1: { cos = 0.0f; sin = 1.0f; break; }
			case 2: { cos = -1.0f; sin = 0.0f; break; }
			case 3: { cos = 0.0f; sin = -1.0f; break; }
		}

		dir.x = cos;
		dir.y = sin;
	}
	else {
		cos = cosf(angle);
		sin = sinf(angle);

		dir.x = cos;
		dir.y = sin;

		float maxDir = fmaxf(fabsf(dir.x), fabsf(dir.y));
		if (maxDir > 0.0f) {
			dir.x /= maxDir;
			dir.y /= maxDir;
		}
	}

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

static void DrawTextfPro_va(const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spacing, double angle, va_list va) {
	DrawTextfCORE(text, x, y, originX, originY, fg, bg, spacing, angle, va);
}

static void DrawTextfEx_va(const char* text, int x, int y, Color* fg, Color* bg, va_list va) {
	DrawTextfPro_va(text, x, y, 0, 0, fg, bg, 0, 0, va);
}

static void DrawTextf_va(const char* text, int x, int y, Color color, va_list va) {
	DrawTextfEx_va(text, x, y, &color, NULL, va);
}

void DrawTextfV(const char* text, Vector2i pos, Color color, ...) {
	va_list va;
	va_start(va, color);
	DrawTextf_va(text, pos.x, pos.y, color, va);
	va_end(va);
}

void DrawTextf(const char* text, int x, int y, Color color, ...) {
	va_list va;
	va_start(va, color);
	DrawTextf_va(text, x, y, color, va);
	va_end(va);
}

void DrawTextfExV(const char* text, Vector2i pos, Color* fg, Color* bg, ...) {
	va_list va;
	va_start(va, bg);
	DrawTextfEx_va(text, pos.x, pos.y, fg, bg, va);
	va_end(va);
}

void DrawTextfEx(const char* text, int x, int y, Color* fg, Color* bg, ...) {
	va_list va;
	va_start(va, bg);
	DrawTextfEx_va(text, x, y, fg, bg, va);
	va_end(va);
}

void DrawTextfProV(const char* text, Vector2i pos, Vector2i origin, Color* fg, Color* bg, int spacing, double angle, ...) {
	va_list va;
	va_start(va, angle);
	DrawTextfPro_va(text, pos.x, pos.y, origin.x, origin.y, fg, bg, spacing, angle, va);
	va_end(va);
}

void DrawTextfPro(const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spacing, double angle, ...) {
	va_list va;
	va_start(va, angle);
	DrawTextfPro_va(text, x, y, originX, originY, fg, bg, spacing, angle, va);
	va_end(va);
}

// TODO: LIBCSITTYFNSINDEPENDENCE
// memcpy, sprintf
// TODO: MAKE IT BETTER AND FASTER
// TODO: Make the %fg %bg %smf codes
void DrawTextfCORE(const char* text, int x, int y, int originX, int originY, Color* fg, Color* bg, int spaceing, double angle, va_list va) {
	if(text == NULL) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "Text is NULL\n");
		return;
	}

	va_list va2;
	va_copy(va2, va);
	int len = vsnprintf(NULL, 0, text, va);
	char* buf = malloc(len + 1);
	vsnprintf(buf, len + 1, text, va2);
	va_end(va2);
	text = buf;

	float cos = 0.0f;
	float sin = 0.0f;
	Vecrot2 dir = {0};

	// NOTE: Check if angle is multiplicity of PI / 2. Accounding for float errors
	float q = angle / (PI / 2.0f);
	int qi = (int)roundf(q);
	if(fabsf(q - qi) < 0.001f) {
		// NOTE: Somehow get dir
		int dir_idx = (qi % 4 + 4) % 4;

		switch(dir_idx) {
			case 0: { cos = 1.0f; sin = 0.0f; break; }
			case 1: { cos = 0.0f; sin = 1.0f; break; }
			case 2: { cos = -1.0f; sin = 0.0f; break; }
			case 3: { cos = 0.0f; sin = -1.0f; break; }
		}

		dir.x = cos;
		dir.y = sin;
	}
	else {
		cos = cosf(angle);
		sin = sinf(angle);

		dir.x = cos;
		dir.y = sin;

		float maxDir = fmaxf(fabsf(dir.x), fabsf(dir.y));
		if (maxDir > 0.0f) {
			dir.x /= maxDir;
			dir.y /= maxDir;
		}
	}

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
