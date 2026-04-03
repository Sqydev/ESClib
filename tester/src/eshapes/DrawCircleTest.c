#include "../../../include/esclib.h"

void DrawCircleTester() {
	InitTui(60, TUI_DYNAMIC);

	float rot = PI / 2;
	while(1) {
		BeginDrawing();
		ClearTui((Color){ 0,0,0,0 }, (Color){ 255,255,255,1 });

		DrawCircle((((float)GetTuiDimensions().x / 2) * ECos(rot)) + ((float)GetTuiDimensions().x / 2), GetTuiDimensions().y / 2, 5, (Color){ 255, 255, 255, 1 });

		DrawCircleEx("🔥", (((float)GetTuiDimensions().x / 2) * ECos(rot)) + ((float)GetTuiDimensions().x / 2), GetTuiDimensions().y / 3, 5, NULL, NULL, false);

		DrawCircleEx("@", (((float)GetTuiDimensions().x / 2) * ECos(rot)) + ((float)GetTuiDimensions().x / 2), GetTuiDimensions().y - GetTuiDimensions().y / 3, 5, NULL, NULL, false);

		DrawCirclePro("@", (((float)GetTuiDimensions().x / 2) * ECos(rot)) + ((float)GetTuiDimensions().x / 2), GetTuiDimensions().y / 6, 5, -PI / 2, PI / 2, NULL, NULL, false);

		DrawCirclePro("🔥", (((float)GetTuiDimensions().x / 2) * ECos(rot)) + ((float)GetTuiDimensions().x / 2), GetTuiDimensions().y / 6, 5, PI / 2, -PI / 2, NULL, NULL, true);

		DrawCirclePro("🔥", (((float)GetTuiDimensions().x / 2) * ECos(rot)) + ((float)GetTuiDimensions().x / 2), GetTuiDimensions().y  - (GetTuiDimensions().y / 6), 5, PI / 2, -PI / 2, NULL, NULL, false);

		DrawCirclePro("@", (((float)GetTuiDimensions().x / 2) * ECos(rot)) + ((float)GetTuiDimensions().x / 2), GetTuiDimensions().y  - (GetTuiDimensions().y / 6), 5, -PI / 2, PI / 2, NULL, NULL, true);
		
		EndDrawing();
		
		//rot += 1 * DEG2RAD;
	}

	CloseTui();
}
