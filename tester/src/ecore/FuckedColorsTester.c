#include "../../../include/esclib.h"

void FuckedUpColorsTest() {
	InitTui(60, TUI_DYNAMIC);

	while(1) {
		BeginFrame();
		ClearTui(TERMBLACK, TERMWHITE);

	 	DrawRectanglePro("$", 40, 55, 40, 10, 0, 0, &(Color){ 255, 0, 0, 255, true }, &(Color){ 0, 0, 255, 255, true }, 0, 5, false, 0, true);

		EndFrame();
	}

	CloseTui();
}
