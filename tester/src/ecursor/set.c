#include "../../../include/esclib.h"

void SetCursorPosTester() {
	InitTui(60, TUI_DYNAMIC);

	while(1) {
		BeginDrawing();
		ClearTui((Color){ 0,0,0,0 }, (Color){ 255,255,255,1 });

		if(IsKeyDown(KEY_W)) { SetCursorPosition(GetCursorPos().x, GetCursorPos().y - 1); }
		if(IsKeyDown(KEY_S)) { SetCursorPosition(GetCursorPos().x, GetCursorPos().y + 1); }
		if(IsKeyDown(KEY_D)) { SetCursorPosition(GetCursorPos().x + 1, GetCursorPos().y); }
		if(IsKeyDown(KEY_A)) { SetCursorPosition(GetCursorPos().x - 1, GetCursorPos().y); }

		EndDrawing();
	}

	CloseTui();
}
