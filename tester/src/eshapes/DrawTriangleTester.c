#include "../../../include/esclib.h"

void DrawTriangleTester() {
    InitTui(60, TUI_DYNAMIC);

    double angle = 0.0;

    while(1) {
        BeginDrawing();
        ClearTui((Color){ 0, 0, 0, 255 }, (Color){ 255, 255, 255, 1 });

        angle += 1 * DEG2RAD;
        if(angle > 2*PI) angle = -2*PI;

		// Make good origin here
        DrawTrianglePro("@", 20, 40, 30, 40, 20, 20, 20, 20, &(Color){255, 255, 255, true}, NULL, angle, false, 1, true);

        DrawTrianglePro("A", 50, 40, 70, 40, 60, 20, 60, 10, &(Color){255, 255, 255, true}, NULL, angle, false, 1, true);

        EndDrawing();
    }

    CloseTui();
}
