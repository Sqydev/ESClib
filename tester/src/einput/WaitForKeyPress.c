#include "../../../include/esclib.h"

void WaitForKeyPressTester() {
	InitTui(60, TUI_DYNAMIC);

	int x = 0;
	int y = 0;

	BeginFrame();
	
	ClearTui(TERMBLACK, TERMWHITE);
	HideCursor();

	EndFrame();

	while(1) {
		BeginFrame();

		WaitForKeyPress();

		DrawCharEx(" ", x, y, &TERMBLACK, &TERMWHITE);
		x++;
		if(x > GetLastTuiIndex().x) {
			y++;
			x = 0;
			if(y > GetLastTuiIndex().y) { y = 0; }
		}

		if(IsKeyPressed(KEY_ESCAPE)) { break; }

		EndFrame();
	}

	CloseTui();
}
