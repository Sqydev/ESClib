#include "../include/esclib.h"

#include "./private/coredata.h"
#include "./private/common_utils.h"

#include <math.h>

#include <stdlib.h>
#include <string.h>

void DrawChar(const char* character, int x, int y, Color color) {
	DrawCharEx(character, x, y, &color, NULL);
}

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
	if(x >= DATA.TuiData.termdimm.x - (vWidth - 1)) {
		return;
	}

	if(y < 0) {
		return;
	}
	if(y >= DATA.TuiData.termdimm.y) {
		return;
	}

	size_t index = y * DATA.TuiData.termdimm.x + x;

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
        
        if (x + i >= DATA.TuiData.termdimm.x) break;

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

void DrawText(const char* text, int x, int y, Color color) {
	DrawTextEx(text, x, y, &color, NULL);
}

void DrawTextEx(const char* text, int x, int y, Color* fg, Color* bg) {
	DrawTextPro(text, x, y, fg, bg, 0, 0);
}

void DrawTextPro(const char* text, int x, int y, Color* fg, Color* bg, int spaceing, float angle) {
	if(text == NULL) {
		UniWriteLen(UNI_WRITE_TARGET_STDERR, "Text is NULL\n");
		return;
	}

	float curX = x;
	float curY = y;

	Vecrot2 dir = {0};

	// NOTE: Check if angle is multiplicity of PI / 2. Accounding for float errors
	float q = angle / (PI / 2.0f);
	int qi = (int)roundf(q);
	if(fabsf(q - qi) < 0.001f) {
		// NOTE: Somehow get dir
		int dir_idx = (qi % 4 + 4) % 4;

		switch(dir_idx) {
			case 0: { dir.x = 1.0f; dir.y = 0.0f; break; }
			case 1: { dir.x = 0.0f; dir.y = 1.0f; break; }
			case 2: { dir.x = -1.0f; dir.y = 0.0f; break; }
			case 4: { dir.x = 0.0f; dir.y = -1.0f; break; }
		}
	}
	else {
		dir.x = cosf(angle);
		dir.y = sinf(angle);

		float maxDir = fmaxf(fabsf(dir.x), fabsf(dir.y));
	    if (maxDir > 0.0f) {
	        dir.x /= maxDir;
	        dir.y /= maxDir;
	    }
	}

	const char* ptrr = text;

	while(*ptrr != '\0') {
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

// TODO: MAKE DRAWTEXTFORMAT
