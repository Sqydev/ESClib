#include "../../../include/esclib.h"
#include <stdio.h>

void FunnyRottatingRec() {
	InitTui(60, TUI_DYNAMIC);

	float rot = 0;
	while(1) {
		BeginDrawing();
		ClearTui((Color){ 0, 0, 0, 0 }, (Color){ 255, 255, 255, 1 });

		DrawRectanglePro(" ", GetLastTuiIndex().x / 2, GetLastTuiIndex().y / 2, 20, 10, 10, 5, NULL, &(Color){ 255, 255, 255, 1 }, rot, 0, false, 0, true);

		DrawRectanglePro("@", GetLastTuiIndex().x / 4, GetLastTuiIndex().y / 2, 20, 10, 10, 5, &(Color){ 255, 255, 255, 1 }, NULL, rot, 0, false, 0, true);

		DrawRectanglePro("🔥", GetLastTuiIndex().x - GetLastTuiIndex().x / 4, GetLastTuiIndex().y / 2, 20, 10, 10, 5, &(Color){ 255, 255, 255, 1 }, NULL, rot, 0, false, 0, true);

		EndDrawing();

		rot += 1 * DEG2RAD;
	}

	CloseTui();
}
