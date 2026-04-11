#include "../../include/esclib.h"

int main() {
	InitTui(165, TUI_DYNAMIC, false);

	float playerPos = (float)GetLastTuiIndex().y / 2;

	while(1) {
		BeginDrawing();
		ClearTui(TERMBLACK, TERMWHITE);

		if(IsKeyDown(KEY_W)) { playerPos -= 55 * GetDeltaTime(); }
		if(IsKeyDown(KEY_S)) { playerPos += 55 * GetDeltaTime(); }

		DrawLinePro(" ", 0, 0, 0, GetLastTuiIndex().y, NULL, &TERMWHITE, 2);
		DrawLinePro(" ", 1, 0, GetLastTuiIndex().x, 0, NULL, &TERMWHITE, 1);
		DrawLinePro(" ", GetLastTuiIndex().x, 0, GetLastTuiIndex().x, GetLastTuiIndex().y, NULL, &TERMWHITE, 2);
		DrawLinePro(" ", 0, GetLastTuiIndex().y, GetLastTuiIndex().x, GetLastTuiIndex().y, NULL, &TERMWHITE, 1);

		DrawRectangle(10, playerPos, 2, 5, TERMWHITE);

		EndDrawing();
	}

	CloseTui();
}
