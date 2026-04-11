#include "../../include/esclib.h"

int main() {
	InitTui(165, TUI_DYNAMIC, false);

	while(1) {
		BeginDrawing();

		EndDrawing();
	}

	CloseTui();
}
