#include "../../../include/esclib.h"

void SetCursorPosTester() {
	InitTui(60, TUI_DYNAMIC);

	while(1) {
		BeginDrawing();
		ClearTui((Color){ 0,0,0,0 }, (Color){ 255,255,255,1 });

		if(IsKeyPressed('w')) { SetCursorPosition(GetCursorPos().x, GetCursorPos().y - 1); }
		if(IsKeyPressed('s')) { SetCursorPosition(GetCursorPos().x, GetCursorPos().y + 1); }
		if(IsKeyPressed('d')) { SetCursorPosition(GetCursorPos().x + 1, GetCursorPos().y); }
		if(IsKeyPressed('a')) { SetCursorPosition(GetCursorPos().x - 1, GetCursorPos().y); }

		EndDrawing();
	}

	CloseTui();
}
