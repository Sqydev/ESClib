#include "./coredata.h"
#include "./common_utils.h"

#include <stdio.h>
#include <string.h>

// TODO: Make it not stdio.h dependent. becouse it's slow and yk
void RenderFrame(void) {
	if(DATA.Buffers.backbuff == NULL || DATA.Buffers.frontbuff == NULL || DATA.Buffers.charbuffer == NULL) { return; }

	if(DATA.TuiData.tuidimm.x <= 0 || DATA.TuiData.tuidimm.y <= 0 || DATA.TuiData.termdimm.x <= 0 || DATA.TuiData.termdimm.y <= 0) { return; }

	DATA.Buffers.charbufferOffset = 0;

	Color lastFg = {0, 0, 0, false};
	Color lastBg = {0, 0, 0, false};
	bool forceColorUpdate = true;

	int cursorX = -1;
	int cursorY = -1;

	for(int y = 0; y < DATA.TuiData.tuidimm.y; y++) {
		for(int x = 0; x < DATA.TuiData.tuidimm.x; x++) {
			size_t index = y * DATA.TuiData.tuidimm.x + x;

			SBCell* back = &DATA.Buffers.backbuff[index];
			SBCell* front = &DATA.Buffers.frontbuff[index];

			if (back->CharLen == 0 && x > 0) {
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

					cursorX += displayWidth;
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

	DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[%d;%dH", DATA.Cursor.pos.y + 1, DATA.Cursor.pos.x + 1);

	if(DATA.Buffers.charbufferOffset > 0) {
		UniWrite(UNI_WRITE_TARGET_STDOUT, DATA.Buffers.charbuffer, DATA.Buffers.charbufferOffset * sizeof(char));
	}
}
