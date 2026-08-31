#include "../../../include/esclib.h"

void SetCursorPosTester(void) {
	InitTui(60, TUI_DYNAMIC);

	while(1) {
		BeginFrame();
		ClearTui(TERMBLACK, TERMWHITE);

		if(IsKeyPressed('w')) { SetCursorPosition(GetCursorPos().x, GetCursorPos().y - 1); }
		if(IsKeyPressed('s')) { SetCursorPosition(GetCursorPos().x, GetCursorPos().y + 1); }
		if(IsKeyPressed('d')) { SetCursorPosition(GetCursorPos().x + 1, GetCursorPos().y); }
		if(IsKeyPressed('a')) { SetCursorPosition(GetCursorPos().x - 1, GetCursorPos().y); }

		EndFrame();
	}

	CloseTui();
}
