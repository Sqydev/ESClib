#include "../../../include/esclib.h"

void FuckedUpColorsTest() {
	InitTui(60, TUI_DYNAMIC);

	while(1) {
		BeginDrawing();
		ClearTui((Color){ 0,0,0,0 }, (Color){ 255,255,255,1 });

	 	DrawRectanglePro("$", 40, 55, 40, 10, 0, 0, &(Color){ 255, 0, 0, 1 }, &(Color){ 0, 0, 255, 1 }, 0, 5, false, 0, true);

		EndDrawing();
	}

	CloseTui();
}
