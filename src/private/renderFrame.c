#include "./coredata.h"
#include "./common_utils.h"

#include <stdio.h>
#include <string.h>

// TODO: Make it not stdio.h dependent
// Make it take trueColor into considiration
void RenderFrame(void) {
	if(DATA.Buffers.backbuff == NULL || DATA.Buffers.frontbuff == NULL || DATA.Buffers.charbuffer == NULL) { return; }

	if(DATA.TuiData.termdimm.x <= 0 || DATA.TuiData.termdimm.y <= 0) { return; }

	DATA.Buffers.charbufferOffset = 0;

	Color lastFg = {0, 0, 0, false};
	Color lastBg = {0, 0, 0, false};
	bool firstPixel = true;

	DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[J\033[H");

	for(int y = 0; y < DATA.TuiData.termdimm.y; y++) {
		for(int x = 0; x < DATA.TuiData.termdimm.x; x++) {
			size_t index = y * DATA.TuiData.termdimm.x + x;

			SBCell* back = &DATA.Buffers.backbuff[index];
			SBCell* front = &DATA.Buffers.frontbuff[index];

			// NOTE: Is it?
			bool different = false;

			DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[%d;%dH", y + 1, x + 1);

			if(back->CharLen != front->CharLen || memcmp(back->Char, front->Char, back->CharLen) != 0 || memcmp(&back->fgColor, &front->fgColor, sizeof(Color)) != 0 || memcmp(&back->bgColor, &front->bgColor, sizeof(Color)) != 0) {
				different = true;
			}

			if(different) {
				if(firstPixel || memcmp(&back->fgColor, &lastFg, sizeof(Color)) != 0) {
					DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[38;2;%d;%d;%dm", back->fgColor.r, back->fgColor.g, back->fgColor.b);
					lastFg = back->fgColor;
				}

				if(firstPixel || memcmp(&back->bgColor, &lastBg, sizeof(Color)) != 0) {
					DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[48;2;%d;%d;%dm", back->bgColor.r, back->bgColor.g, back->bgColor.b);
					lastBg = back->bgColor;
				}

				if(back->CharLen > 0) {
					memcpy(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, back->Char, (size_t)back->CharLen);
					DATA.Buffers.charbufferOffset += back->CharLen;
				}

				firstPixel = false;
				*front = *back;
			}
		}
	}

	DATA.Buffers.charbufferOffset += sprintf(DATA.Buffers.charbuffer + DATA.Buffers.charbufferOffset, "\033[%d;%dH", DATA.TuiData.termdimm.y, DATA.TuiData.termdimm.x);

	if(DATA.Buffers.charbufferOffset > 0) {
		UniWrite(UNI_WRITE_TARGET_STDOUT, DATA.Buffers.charbuffer, DATA.Buffers.charbufferOffset * sizeof(char));
	}
}
