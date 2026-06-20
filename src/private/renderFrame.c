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

#include "./coredata.h"
#include "./common_utils.h"

#include <stdio.h>
#include <string.h>

void RenderFrame(void) {
	if(DATA.Buffers.backbuff == NULL || DATA.Buffers.frontbuff == NULL || DATA.Buffers.charbuffer == NULL) { return; }

	if(DATA.TuiData.tuidimm.x <= 0 || DATA.TuiData.tuidimm.y <= 0 || DATA.TuiData.termdimm.x <= 0 || DATA.TuiData.termdimm.y <= 0) { return; }

	DATA.Buffers.charbufferOffset = 0;

	Color lastFg = {0, 0, 0, false};
	Color lastBg = {0, 0, 0, false};
	bool forceColorUpdate = true;

	int cursorX = -1;
	int cursorY = -1;

	DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[0m");

	for(int y = 0; y < DATA.TuiData.tuidimm.y; y++) {
		for(int x = 0; x < DATA.TuiData.tuidimm.x; x++) {
			size_t index = y * DATA.TuiData.tuidimm.x + x;

			SBCell* back = &DATA.Buffers.backbuff[index];
			SBCell* front = &DATA.Buffers.frontbuff[index];

			if(back->CharLen == 0 && x > 0) {
			    *front = *back;
			    continue; 
			}

			// NOTE: Or is it?
			bool different = (back->CharLen != front->CharLen || memcmp(back->Char, front->Char, back->CharLen) != 0 || memcmp(&back->fgColor, &front->fgColor, sizeof(Color)) != 0 || memcmp(&back->bgColor, &front->bgColor, sizeof(Color)) != 0);

			if(different) {
				if (cursorY != y || cursorX != x) {
					DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[%d;%dH", y + 1, x + 1);
				}

				if(forceColorUpdate || memcmp(&back->fgColor, &lastFg, sizeof(Color)) != 0) {
					if(back->fgColor.trueColor) {
						DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[38;2;%d;%d;%dm", back->fgColor.r, back->fgColor.g, back->fgColor.b);
					}
					else {
						int paletteIdx = (int)back->fgColor.r + back->fgColor.g + back->fgColor.b;
						DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[38;5;%dm", paletteIdx);
					}
					lastFg = back->fgColor;
				}

				if(forceColorUpdate || memcmp(&back->bgColor, &lastBg, sizeof(Color)) != 0) {
					if(back->bgColor.trueColor) {
						DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[48;2;%d;%d;%dm", back->bgColor.r, back->bgColor.g, back->bgColor.b);
					}
					else {
						int paletteIdx = (int)back->bgColor.r + back->bgColor.g + back->bgColor.b;
                        DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[48;5;%dm", paletteIdx);
					}
					lastBg = back->bgColor;
				}

				if(back->CharLen > 0) {
					int displayWidth = GetCharWidth(back->Char);
					memcpy(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, back->Char, (size_t)back->CharLen);
					DATA.Buffers.charbufferOffset += back->CharLen;

					cursorX = x + displayWidth;
					cursorY = y;
				}
				else {
					// NOTE: Failsafe
					DATA.Buffers.charbuffer[DATA.Buffers.charbufferOffset++] = ' ';
					cursorX = x + 1;
					cursorY = y;
				}

				forceColorUpdate = false;
				*front = *back;
			}
		}
	}

	bool shouldBeHidden = DATA.Cursor.hidden || DATA.Cursor.oob;
 
	if(!shouldBeHidden) { DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[%d;%dH", DATA.Cursor.pos.y + 1, DATA.Cursor.pos.x + 1); }

	if(shouldBeHidden && !DATA.Cursor.preHidden) {
		DATA.Cursor.preHidden = true;
		DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[?25l");
	}
	else if(!shouldBeHidden && DATA.Cursor.preHidden) {
		DATA.Cursor.preHidden = false;
		DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[?25h");
	}


	if(DATA.Buffers.charbufferOffset > 0) {
		UniWrite(UNI_WRITE_TARGET_STDOUT, DATA.Buffers.charbuffer, DATA.Buffers.charbufferOffset * sizeof(char));
	}
}
