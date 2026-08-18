#include "../../../include/esclib.h"

void DrawLineTester() {
	InitTui(60, TUI_DYNAMIC);

	while(1) {
		BeginFrame();
		ClearTui(TRUEBLACK, TRUEWHITE);

		DrawLine(GetLastTuiIndex().x - GetLastTuiIndex().x / 6, GetLastTuiIndex().y / 2 - GetLastTuiIndex().y / 6, GetLastTuiIndex().x / 6, GetLastTuiIndex().y / 2 - GetLastTuiIndex().y / 6, WHITE, 1);

		DrawLine(GetLastTuiIndex().x - GetLastTuiIndex().x / 6, GetLastTuiIndex().y - GetLastTuiIndex().y / 6, GetLastTuiIndex().x / 6, GetLastTuiIndex().y - GetLastTuiIndex().y / 6, WHITE, 3);

		DrawLineEx("@", GetLastTuiIndex().x / 7, GetLastTuiIndex().y / 7, GetLastTuiIndex().x - GetLastTuiIndex().x / 7, GetLastTuiIndex().y - GetLastTuiIndex().y / 7, &WHITE, &(Color){ 255, 0, 0, 255, true }, 1);

		DrawLineEx("#", GetLastTuiIndex().x - GetLastTuiIndex().x / 7, GetLastTuiIndex().y / 7, GetLastTuiIndex().x / 7, GetLastTuiIndex().y - GetLastTuiIndex().y / 7, &WHITE, &(Color){ 0, 255, 0, 255, true }, 3);

		DrawLineEx("🔥", GetLastTuiIndex().x / 2, GetLastTuiIndex().y - GetLastTuiIndex().y / 4, GetLastTuiIndex().x / 2, GetLastTuiIndex().y / 4, NULL, NULL, 1);

		DrawLineEx("🔥", GetLastTuiIndex().x / 4, GetLastTuiIndex().y / 2, GetLastTuiIndex().x - GetLastTuiIndex().x / 4, GetLastTuiIndex().y / 2, NULL, NULL, 1);

		EndFrame();
	}

	CloseTui();
}
