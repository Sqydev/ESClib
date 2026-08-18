#include "../../../include/esclib.h"

void TestTimes(void) {
	InitTui(60, TUI_DYNAMIC);

	while(1) {
		BeginFrame();
		EndFrame();
	}

	CloseTui();
}
