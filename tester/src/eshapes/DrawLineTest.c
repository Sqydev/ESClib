#include "../../../include/esclib.h"

void DrawLineTester() {
	InitTui(60, TUI_DYNAMIC);

	while(1) {
		BeginDrawing();
		ClearTui((Color){ 0,0,0,0 }, (Color){ 255,255,255,1 });

		DrawLinePro("@", GetLastTuiIndex().x / 4, GetLastTuiIndex().y / 4, GetLastTuiIndex().x - GetLastTuiIndex().x / 4, GetLastTuiIndex().y - GetLastTuiIndex().y / 4, NULL, &WHITE, 1, true);

		DrawLinePro("@", GetLastTuiIndex().x / 3, GetLastTuiIndex().y / 3, GetLastTuiIndex().x - GetLastTuiIndex().x / 3, GetLastTuiIndex().y - GetLastTuiIndex().y / 3, NULL, &(Color){ 200, 200, 200, 1 }, 3, true);
		
		EndDrawing();
		
	}

	CloseTui();
}
