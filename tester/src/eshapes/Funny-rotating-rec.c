#include "../../../include/esclib.h"
#include <stdio.h>

void FunnyRottatingRec() {
	InitTui(60, TUI_DYNAMIC);

	float rot = 0;
	while(1) {
		BeginFrame();
		ClearTui(TRUEBLACK, TRUEWHITE);

		DrawRectanglePro(" ", GetLastTuiIndex().x / 2, GetLastTuiIndex().y / 2, 20, 10, 10, 5, NULL, &TRUEWHITE, rot, 0, false, 0, true);

		DrawRectanglePro("@", GetLastTuiIndex().x / 4, GetLastTuiIndex().y / 2, 20, 10, 10, 5, &TRUEWHITE, NULL, rot, 0, false, 0, true);

		DrawRectanglePro("🔥", GetLastTuiIndex().x - GetLastTuiIndex().x / 4, GetLastTuiIndex().y / 2, 20, 10, 10, 5, &TRUEWHITE, NULL, rot, 0, false, 0, true);

		EndFrame();

		rot += 1 * DEG2RAD;
	}

	CloseTui();
}
