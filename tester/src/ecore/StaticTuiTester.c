#include "../../../include/esclib.h"

void StaticTuiTester(void) {
	InitTui(165, TUI_STATIC);

	// Should be cube
	SetTuiDimentions(60, 30);

	while(1) {
		BeginFrame();

		ClearTui(TERMBLACK, TERMWHITE);

		DrawChar("q", 0, 0, TRUEWHITE);
		DrawChar("ą", GetLastTuiIndex().x, 0, TRUEWHITE);
		DrawChar("→", 0, GetLastTuiIndex().y, TRUEWHITE);
		DrawChar("🔥", GetLastTuiIndex().x, GetLastTuiIndex().y, TRUEWHITE);
		DrawChar("ą", 0, GetLastTuiIndex().y / 2, TRUEWHITE);
		DrawChar("q", GetLastTuiIndex().x / 2, 0, TRUEWHITE);
		DrawChar("→", GetLastTuiIndex().x, GetLastTuiIndex().y / 2, TRUEWHITE);
		DrawChar("⠟", GetLastTuiIndex().x / 2, GetLastTuiIndex().y / 2, TRUEWHITE);
		DrawChar("🔥", GetLastTuiIndex().x / 2, GetLastTuiIndex().y, TRUEWHITE);

		EndFrame();
	}

	CloseTui();
}
