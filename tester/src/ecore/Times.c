#include "../../../include/esclib.h"

void TestTimes(void) {
	InitTui(60);

	while(1) {
		BeginDrawing();
		EndDrawing();
	}

	CloseTui();
}
