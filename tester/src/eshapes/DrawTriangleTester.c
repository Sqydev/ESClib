#include "../../../include/esclib.h"

void DrawTriangleTester(void) {
    InitTui(60, TUI_DYNAMIC);

    double angle = 0.0;

    while(1) {
        BeginFrame();
        ClearTui(TRUEBLACK, TRUEWHITE);

        angle += 1 * DEG2RAD;
        if(angle > 2 * PI) { angle = -2 * PI; }

        int width = GetTuiDimensions().x;
        int height = GetTuiDimensions().y;

        int Ax1 = width * 1 / 8;
        int Ay1 = height * 3 / 4;

        int Bx1 = width * 3 / 8;
        int By1 = height * 3 / 4;

        int Cx1 = width * 1 / 4;
        int Cy1 = height * 1 / 4;

        DrawTrianglePro("@", Ax1, Ay1, Bx1, By1, Cx1, Cy1, (Ax1 + Bx1 + Cx1) / 3, (Ay1 + By1 + Cy1) / 3, &(Color){255, 255, 255, 255, true}, NULL, angle, false, 0, true);

        int Ax2 = width * 5 / 8;
        int Ay2 = height * 3 / 4;

        int Bx2 = width * 7 / 8;
        int By2 = height * 3 / 4;

        int Cx2 = width * 3 / 4;
        int Cy2 = height * 1 / 4;

        DrawTrianglePro("A", Ax2, Ay2, Bx2, By2, Cx2, Cy2, (Ax2 + Bx2 + Cx2) / 3, (Ay2 + By2 + Cy2) / 3, &(Color){255, 255, 255, 255, true}, NULL, angle, true, 1, true);

        EndFrame();
    }

    CloseTui();
}
