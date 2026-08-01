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

		int ch = WaitForKeyPress();
		char cha[2] = { ch, '\0' };

		DrawChar(cha, x, y, TERMWHITE);
		x++;
		if(x > GetLastTuiIndex().x) {
			y++;
			x = 0;
			if(y > GetLastTuiIndex().y) { y = 0; }
		}

		if(ch == KEY_ESCAPE) { break; }

		EndDrawing();
	}

	CloseTui();
}
