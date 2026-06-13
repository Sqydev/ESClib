#include "../../../include/esclib.h"

#include <stdlib.h>

#define brilearraycnt 10

const char *BrileArray1[brilearraycnt] = { "⠁", "⠂", "⠃", "⠄", "⠅", "⠆", "⠇", "⠈", "⠉", "⠊" };
const char *BrileArray2[brilearraycnt] = { "⠄", "⠅", "⠆", "⠇", "⠈", "⠉", "⠊", "⠋", "⠌", "⠍" };

size_t brilecurr = 0;

void MergBrileTester() {
	InitTui(60, TUI_DYNAMIC);

	while(1) {
		BeginDrawing();
		ClearTui(BLACK, WHITE);

		if(IsKeyPressed(KEY_ENTER)) {
			if(brilecurr == brilearraycnt - 1) { brilecurr = 0; }
			else { brilecurr++; }
		}

		char* outBrl = MergeBraille(BrileArray1[brilecurr], BrileArray2[brilecurr]);

		DrawChar(BrileArray1[brilecurr], GetLastTuiIndex().x / 2 - 2, GetLastTuiIndex().y / 2, WHITE);
		DrawChar(outBrl, GetLastTuiIndex().x / 2, GetLastTuiIndex().y / 2, WHITE);
		DrawChar(BrileArray2[brilecurr], GetLastTuiIndex().x / 2 + 2, GetLastTuiIndex().y / 2, WHITE);

		free(outBrl);

		EndDrawing();
	}
}
