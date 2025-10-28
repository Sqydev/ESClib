#include "../../include/esclib.h"

int TargetFps = 0;
color DefaultColor = (color){0, 0, 0};
color CurrentColor;

// TODO: WHEN DOING LIB WHEN INITING MAKE IT SO IT SETS DEAFOULT COLOR AND THEN IN FILLSCREEN YOU CAN MAKE IT FULLY WORK
void InitTui(int fps) {
	TargetFps = fps;

	CurrentColor = DefaultColor;

	TuiEnableBuffMode();

	EnableRawMode();

	TuiClearScreen();

	FillScreen(CurrentColor);
}
