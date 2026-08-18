#include "../../../include/esclib.h"

void StaticTuiTester() {
	InitTui(165, TUI_STATIC);

	// Should be cube
	SetTuiDimentions(60, 30);

	while(1) {
		BeginFrame();

		ClearTui((Color){ 0,0,0,0 }, (Color){ 255, 0, 0, 0});

		DrawChar("q", 0, 0, (Color){ 255, 255, 255, 1 });
		DrawChar("ą", GetLastTuiIndex().x, 0, (Color){ 255, 255, 255, 1 });
		DrawChar("→", 0, GetLastTuiIndex().y, (Color){ 255, 255, 255, 1 });
		DrawChar("🔥", GetLastTuiIndex().x, GetLastTuiIndex().y, (Color){ 255, 255, 255, 1 });
		DrawChar("ą", 0, GetLastTuiIndex().y / 2, (Color){ 255, 255, 255, 1 });
		DrawChar("q", GetLastTuiIndex().x / 2, 0, (Color){ 255, 255, 255, 1 });
		DrawChar("→", GetLastTuiIndex().x, GetLastTuiIndex().y / 2, (Color){ 255, 255, 255, 1 });
		DrawChar("⠟", GetLastTuiIndex().x / 2, GetLastTuiIndex().y / 2, (Color){ 255, 255, 255, 1 });
		DrawChar("🔥", GetLastTuiIndex().x / 2, GetLastTuiIndex().y, (Color){ 255, 255, 255, 1 });

		EndFrame();
	}

	CloseTui();
}
