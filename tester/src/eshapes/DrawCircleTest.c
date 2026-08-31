#include "../../../include/esclib.h"

void DrawCircleTester(void) {
	InitTui(60, TUI_DYNAMIC);

	float rot = PI / 2;
	while(1) {
		BeginFrame();
		ClearTui(TRUEBLACK, TRUEWHITE);

		DrawCircle((((float)GetTuiDimensions().x / 2) * ECos(rot)) + ((float)GetTuiDimensions().x / 2), GetTuiDimensions().y / 2, 5, TRUEWHITE);

		DrawCircleEx("🔥", (((float)GetTuiDimensions().x / 2) * ECos(rot)) + ((float)GetTuiDimensions().x / 2), GetTuiDimensions().y / 3, 5, NULL, NULL, false, 0);

		DrawCircleEx("@", (((float)GetTuiDimensions().x / 2) * ECos(rot)) + ((float)GetTuiDimensions().x / 2), GetTuiDimensions().y - GetTuiDimensions().y / 3, 5, NULL, NULL, false, 0);

		DrawCirclePro("@", (((float)GetTuiDimensions().x / 2) * ECos(rot)) + ((float)GetTuiDimensions().x / 2), GetTuiDimensions().y / 6, 5, -PI / 2, PI / 2, NULL, NULL, false, 0, true);

		DrawCirclePro("🔥", (((float)GetTuiDimensions().x / 2) * ECos(rot)) + ((float)GetTuiDimensions().x / 2), GetTuiDimensions().y / 6, 5, PI / 2, -PI / 2, NULL, NULL, true, 1, true);

		DrawCirclePro("🔥", (((float)GetTuiDimensions().x / 2) * ECos(rot)) + ((float)GetTuiDimensions().x / 2), GetTuiDimensions().y  - (GetTuiDimensions().y / 6), 5, PI / 2, -PI / 2, NULL, NULL, false, 0, true);

		DrawCirclePro("@", (((float)GetTuiDimensions().x / 2) * ECos(rot)) + ((float)GetTuiDimensions().x / 2), GetTuiDimensions().y  - (GetTuiDimensions().y / 6), 5, -PI / 2, PI / 2, NULL, NULL, true, 1, true);
		
		EndFrame();
		
		rot += 1 * DEG2RAD;
	}

	CloseTui();
}
