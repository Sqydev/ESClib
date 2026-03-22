#include "../../../include/esclib.h"

void StaticTuiTester() {
	InitTui(60, TUI_STATIC);

	// Should be cube
	SetTuiDimentions(60, 30);

	while(1) {
		BeginDrawing();

		ClearTui((Color){ 0,0,0,0 }, (Color){ 255, 0, 0, 0});

		DrawChar("q", 0, 0, (Color){ 255, 255, 255, 1 });
		DrawChar("ą", GetTuiDimensions().x, 0, (Color){ 255, 255, 255, 1 });
		DrawChar("→", 0, GetTuiDimensions().y, (Color){ 255, 255, 255, 1 });
		DrawChar("🔥", GetTuiDimensions().x, GetTuiDimensions().y, (Color){ 255, 255, 255, 1 });
		DrawChar("ą", 0, GetTuiDimensions().y / 2, (Color){ 255, 255, 255, 1 });
		DrawChar("q", GetTuiDimensions().x / 2, 0, (Color){ 255, 255, 255, 1 });
		DrawChar("→", GetTuiDimensions().x, GetTuiDimensions().y / 2, (Color){ 255, 255, 255, 1 });
		DrawChar("⠟", GetTuiDimensions().x / 2, GetTuiDimensions().y / 2, (Color){ 255, 255, 255, 1 });
		DrawChar("🔥", GetTuiDimensions().x / 2, GetTuiDimensions().y, (Color){ 255, 255, 255, 1 });

		EndDrawing();
	}

	CloseTui();
}
