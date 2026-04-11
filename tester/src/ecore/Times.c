#include "../../../include/esclib.h"

void TestTimes(void) {
	InitTui(60, TUI_DYNAMIC, true);

	while(1) {
		BeginDrawing();
		EndDrawing();
	}

	CloseTui();
}
