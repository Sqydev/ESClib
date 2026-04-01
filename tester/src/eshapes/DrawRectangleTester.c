#include "../../../include/esclib.h"

#include <stdlib.h>

void DrawRecTester() {
	InitTui(60, TUI_DYNAMIC);

	while(1) {
		BeginDrawing();
		ClearTui((Color){ 0,0,0,0 }, (Color){ 255,255,255,1 });

		// NOTE: Squere
		DrawRectangle(2, 5, 10, 5, (Color){ 255, 255, 255, 1 });

		// NOTE: Purple rec made of @
		DrawRectangleEx("@", 15, 5, 12, 4, &(Color){ 255, 0, 255, 1 }, NULL);

		// NOTE: Rec with 45 DEG rotation that is $'s of color purple and background blue
	 	DrawRectanglePro("$", 40, 30, 40, 10, 0, 0, &(Color){ 255, 0, 0, 1 }, &(Color){ 0, 0, 255, 1 }, 45 * DEG2RAD, 0);

		// NOTE: Rec with 45 DEG rotation that is $'s of color purple and background blue BUT with roundness 5
	 	DrawRectanglePro("$", 85, 30, 40, 10, 0, 0, &(Color){ 255, 0, 0, 1 }, &(Color){ 0, 0, 255, 1 }, 45 * DEG2RAD, 50);

		// NOTE: Rec that is $'s of color purple and background blue BUT with roundness 70 BUT with 0 DEG
	 	DrawRectanglePro("$", 40, 55, 40, 10, 0, 0, &(Color){ 255, 0, 0, 1 }, &(Color){ 0, 0, 255, 1 }, 0, 70);

		// NOTE: Rec made of 🔥
		DrawRectangleEx("🔥", 70, 5, 12, 4, NULL, NULL);

		// NOTE: Rec with 45 DEG rotation that is made of 🔥 roundness 50
	 	DrawRectanglePro("🔥", 100, 20, 20, 10, 10, 5, NULL, NULL, 45 * DEG2RAD, 50);

		EndDrawing();
	}

	CloseTui();
}
