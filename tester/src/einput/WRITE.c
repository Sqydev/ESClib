#include "../../../include/esclib.h"

void WRITETester() {
	InitTui(60, TUI_DYNAMIC);

	int x = 0;
	int y = 0;

	BeginDrawing();
	
	ClearTui(TERMBLACK, TERMWHITE);
	HideCursor();

	EndDrawing();

	while(1) {
		BeginDrawing();

		WaitForKeyPress();

		char ch[2] = { GetKeyPressed(), '\0' };

		DrawChar(ch, x, y, TERMWHITE);
		x++;
		if(x > GetLastTuiIndex().x) {
			y++;
			x = 0;
			if(y > GetLastTuiIndex().y) { y = 0; }
		}

		if(IsKeyPressed(KEY_ESCAPE)) { break; }

		EndDrawing();
	}

	CloseTui();
}
