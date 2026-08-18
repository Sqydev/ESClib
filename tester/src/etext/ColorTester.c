#include "../../../include/esclib.h"

// TODO: TODO:
void ColorBlendingTester(void) {
	InitTui(60, TUI_DYNAMIC);

	while(1) {
		BeginFrame();
		ClearTui(TERMBLACK, TERMWHITE);

		DrawText("TODO", 0, 0, TERMWHITE);

		EndFrame();
	}

	CloseTui();
}
