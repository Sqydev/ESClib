#include "../../../include/esclib.h"

void IsKeyDownTester() {
	InitTui(60, TUI_DYNAMIC);

	Vector2i pos = (Vector2i){ GetLastTuiIndex().x / 2, GetLastTuiIndex().y / 2 };

	while(1) {
		BeginFrame();
		ClearTui(TRUEBLACK, TRUEWHITE);

		DrawCharEx(" ", pos.x, pos.y, NULL, &WHITE);

		if(IsKeyPressed('w')) { pos.y -= 1; }
		if(IsKeyPressed('s')) { pos.y += 1; }
		if(IsKeyPressed('d')) { pos.x += 1; }
		if(IsKeyPressed('a')) { pos.x -= 1; }

		EndFrame();
	}

	CloseTui();
}
