#include "../../../include/esclib.h"

void IsKeyDownTester() {
	InitTui(60, TUI_DYNAMIC);

	Vector2i pos = (Vector2i){ GetLastTuiIndex().x / 2, GetLastTuiIndex().y / 2 };

	while(1) {
		BeginDrawing();
		ClearTui((Color){ 0,0,0,0 }, (Color){ 255,255,255,1 });

		DrawCharEx(" ", pos.x, pos.y, NULL, &WHITE);

		if(IsKeyDown(KEY_W)) { pos.y -= 1; }
		if(IsKeyDown(KEY_S)) { pos.y += 1; }
		if(IsKeyDown(KEY_D)) { pos.x += 1; }
		if(IsKeyDown(KEY_A)) { pos.x -= 1; }

		EndDrawing();
	}

	CloseTui();
}
